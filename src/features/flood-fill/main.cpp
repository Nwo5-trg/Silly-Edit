#include <features/miscellaneous/include.hpp>
#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;

static constexpr float MINIMUM_SPECIAL_MOUSE_DISTANCE = 5.0f;

static void showNotification(ZStringView pStr, NotificationIcon pIcon) {
    if (FF::notifications) {
        geode::Notification::create(pStr, pIcon)->show();
    }
}

void FF::EditorUI::rectFill(CCArray* pObjs) {
    const auto aPos = static_cast<GameObject*>(pObjs->firstObject())->getRealPosition();
    const auto bPos = static_cast<GameObject*>(pObjs->lastObject())->getRealPosition();

    if (aPos == bPos) {
        return showNotification("objects overlapped", NotificationIcon::Warning);
    }

    auto base = static_cast<GameObject*>(pObjs->firstObject());

    const std::string str{base->getSaveString(m_editorLayer)};

    const auto size = CCSize{base->m_scaleX, base->m_scaleY} * editor::object::size(base);
    const CCPoint min{
        std::min(aPos.x, bPos.x), std::min(aPos.y, bPos.y)
    };
    const CCPoint max{
        std::max(aPos.x, bPos.x), std::max(aPos.y, bPos.y)
    };

    auto placedObjs = CCArray::create();

    for (auto x = min.x; x <= max.x; x += size.width) {
        for (auto y = min.y; y <= max.y; y += size.height) {
            if (auto res = m_editorLayer->createObjectsFromString(str, true, true); res && res->count()) {
                auto obj = static_cast<GameObject*>(res->firstObject());

                editor::object::move(obj, {x, y});

                placedObjs->addObject(obj);
            }
        }
    }

    editor::object::remove(pObjs, true);

    if (FF::selectFill) {
        editor::selection::set(placedObjs, false, true);
    }

    m_editorLayer->m_undoObjects->addObject(
        UndoObject::createWithArray(placedObjs, UndoCommand::Paste)
    );

    editor::update();

    showNotification("successfully rect filled !", NotificationIcon::Info);
}
void FF::EditorUI::createFromRects(const std::vector<FF::Rect>& pRects, CCArray* pBoundry, GameObject* pBase) {
    auto placedObjs = CCArray::create();

    const std::string str{pBase->getSaveString(m_editorLayer)};
    const auto size = editor::object::size(pBase);

    for (const auto& rect : pRects) {
        if (auto res = m_editorLayer->createObjectsFromString(str, true, true); res && res->count()) {
            auto obj = static_cast<GameObject*>(res->firstObject());
            
            editor::object::move(obj, rect.center());
            editor::object::scale(obj, rect.width() / size, rect.height() / size);

            placedObjs->addObject(obj);
        }
    }

    if (placedObjs->count()) {
        editor::selection::clear();

        if (FF::selectFill) {
            editor::selection::add(pBase, false, true);
            editor::selection::add(placedObjs, false, true);
        }
        if (FF::selectBoundry) {
            editor::selection::add(pBoundry, false, true);
        }

        m_editorLayer->m_undoObjects->addObject(
            UndoObject::createWithArray(placedObjs, UndoCommand::Paste)
        );
    }
    
    editor::update();

    showNotification("successfully flood filled !", NotificationIcon::Info);
}

void FF::EditorUI::quickFill() {
    if (const auto count = editor::selection::count(); count < 2) {
        return showNotification("no (or too little) objs selected !", NotificationIcon::Warning);
    }
    else if (count == 2) {
        return this->rectFill(editor::selection::get());
    }

    auto objs = editor::selection::get();
    
    std::optional<int> mainID;
    GameObject* center = nullptr;

    for (auto obj : CCArrayExt<GameObject*>(objs)) {
        if (!mainID.has_value()) {
            mainID = obj->m_objectID;
        }
        else if (obj->m_objectID != mainID.value()) {
            if (!center) {
                center = obj;
            }
            else {
                mainID = std::nullopt;

                break;
            }
        }
    }

    if (!mainID.has_value()) {
        return showNotification("quickfill unable to resolve fill type !", NotificationIcon::Warning);
    }

    if (center) {
        objs->removeObject(center, false);

        this->createFromRects(
            FF::gridFloodFill(std::move(FF::rectsFromObjects(objs)), FF::rectFromObject(center), false),
            objs, center
        );
    }
    else {
        this->createFromRects(
            FF::gridFloodFill(
                std::move(FF::rectsFromObjects(objs)), 
                FF::rectFromObject(static_cast<GameObject*>(objs->firstObject()), editor::object::center(objs, true)), false
            ),
            objs, static_cast<GameObject*>(objs->firstObject())
        );
    }
}

void FF::EditorUI::keyDown(enumKeyCodes key, double timestamp) {
    auto fields = m_fields.self();

    if (fields->specialHold && key == enumKeyCodes::KEY_Escape) {
        fields->specialHold = false;

        return;
    }
    
    GD::EditorUI::keyDown(key, timestamp);
}

void FF::Feature::onEditor() {
    auto ui = editor::ui<FF::EditorUI>();

    editor::conditionallyRegisterEditTabButtonFrame(
        FF::enabled() && FF::quickFillButton,
        "quickfill.png"_spr, "quick-fill-button"_spr, 2, [ui] (auto) {
            if (FF::enabled()) {
                ui->quickFill();
            }
        }
    );

    nwo5::utils::setupKeybind(ui, "flood-fill-special-key", [ui] (const Keybind&, bool pDown, bool pRepeat, double) {
        if (!FF::enabled()) {
            return;
        }

        if (FF::specialAsButton) {
            if (pDown && !pRepeat) {
                ui->quickFill();
            }

            return;
        }

        auto fields = ui->m_fields.self();
        
        if (pDown && !pRepeat) {
            fields->specialHold = true;
            fields->specialStart = ui->m_editorLayer->m_objectLayer->convertToNodeSpace(cocos::getMousePos());
        }
        else if (!pDown && fields->specialHold) {
            fields->specialHold = false;

            const auto mouse = ui->m_editorLayer->m_objectLayer->convertToNodeSpace(cocos::getMousePos());

            if (fields->specialStart.getDistance(mouse) <= MINIMUM_SPECIAL_MOUSE_DISTANCE) {
                return;
            }

            const auto size = editor::object::size(ui->m_selectedObjectIndex);
            const auto min = ui->getGridSnappedPos({
                std::min(fields->specialStart.x, mouse.x), std::min(fields->specialStart.y, mouse.y)
            }) + ui->offsetForKey(ui->m_selectedObjectIndex);
            const auto max = ui->getGridSnappedPos({
                std::max(fields->specialStart.x, mouse.x), std::max(fields->specialStart.y, mouse.y)
            }) + ui->offsetForKey(ui->m_selectedObjectIndex);

            // i dont think i even need this but idek anymore
            Miscellaneous::removePreviewObject();

            auto placedObjs = CCArray::create();

            for (auto x = min.x; x <= max.x; x += size) {
                for (auto y = min.y; y <= max.y; y += size) {
                    if (auto obj = ui->m_editorLayer->createObject(ui->m_selectedObjectIndex, {x, y}, true)) {
                        placedObjs->addObject(obj);
                    }
                }
            }
                
            if (FF::selectFill && FF::selectSpecialFill) {
                editor::selection::set(placedObjs, false, true);
            }

            ui->m_editorLayer->m_undoObjects->addObject(
                UndoObject::createWithArray(placedObjs, UndoCommand::Paste)
            );

            editor::update();

            showNotification("successfully rect filled !", NotificationIcon::Info);
        }

    });
}

void FF::Feature::onUpdate() {
    auto ui = editor::ui<FF::EditorUI>();

    Miscellaneous::shouldHidePreviewObject() = false;

    if (!FF::enabled()) {
        return;
    }

    auto fields = ui->m_fields.self();

    if (ui->m_isPaused || ui->m_editorLayer->m_playbackMode != PlaybackMode::Not || !ui->m_selectedObjectIndex || ui->m_selectedMode != 2) {
        fields->specialHold = false;
    }

    if (!fields->specialHold) {
        return;
    }

    const auto mouse = ui->m_editorLayer->m_objectLayer->convertToNodeSpace(cocos::getMousePos());

    if (fields->specialStart.getDistance(mouse) <= MINIMUM_SPECIAL_MOUSE_DISTANCE) {
        return;
    }

    Miscellaneous::shouldHidePreviewObject() = true;

    const auto size = editor::object::size(ui->m_selectedObjectIndex);

    const auto start = ui->getGridSnappedPos(CCPoint{
        std::min(fields->specialStart.x, mouse.x), std::min(fields->specialStart.y, mouse.y)
    }) - ccp(size, size) / 2;
    const auto end = ui->getGridSnappedPos(CCPoint{
        std::max(fields->specialStart.x, mouse.x), std::max(fields->specialStart.y, mouse.y)
    }) + ccp(size, size) / 2;
    
    const auto col = FF::chroma 
        ? nwo5::utils::getChroma<ccColor4F>(Shared::ChromaNode::Default) 
        : color_cast<ccColor4F>(FF::specialPreviewColor.get());

    Shared::getGridDraw()->drawRect(
        start, end, nwo5::utils::setOpacity(col, FF::specialPreviewFill.get()),
        FF::specialPreviewThickness / (FF::scaleWithZoom ? editor::zoom() : 1.0f), col
    );
}