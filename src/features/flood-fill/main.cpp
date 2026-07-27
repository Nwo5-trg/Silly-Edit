#include <Geode/modify/EditorUI.hpp>
#include <internal/utils/utils.hpp>
#include <features/shared.hpp>
#include <features/miscellaneous/include.hpp>
#include "settings.hpp"
#include "include.hpp"

using namespace geode::prelude;

constexpr float MINIMUM_SPECIAL_MOUSE_DISTANCE = 5.0f;

class $modify(FloodFillEditorUI, EditorUI) {
    struct Fields {
        bool specialHold = false;
        CCPoint specialStart = CCPointZero;
    };

    static void showNotification(ZStringView pStr, NotificationIcon pIcon) {
        if (Settings::FloodFill::notifications.get()) {
            Notification::create(pStr, pIcon)->show();
        }
    }

    void rectFill(CCArray* pObjs) {
        const auto aPos = static_cast<GameObject*>(pObjs->firstObject())->getRealPosition();
        const auto bPos = static_cast<GameObject*>(pObjs->lastObject())->getRealPosition();

        if (aPos == bPos) {
            return this->showNotification("objects overlapped", NotificationIcon::Warning);
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

        if (Settings::FloodFill::selectFill.get()) {
            editor::selection::add(placedObjs, false, true);
        }

        m_editorLayer->m_undoObjects->addObject(
            UndoObject::createWithArray(placedObjs, UndoCommand::Paste)
        );

        editor::update();

        this->showNotification("successfully rect filled !", NotificationIcon::Info);
    }
    void createFromRects(const std::vector<FF::Rect>& pRects, CCArray* pBoundry, GameObject* pBase) {
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
            if (Settings::FloodFill::selectFill.get()) {
                editor::selection::add(pBase, false, true);
                editor::selection::add(placedObjs, false, true);
            }
            if (Settings::FloodFill::selectBoundry.get()) {
                editor::selection::add(pBoundry, false, true);
            }

            m_editorLayer->m_undoObjects->addObject(
                UndoObject::createWithArray(placedObjs, UndoCommand::Paste)
            );
        }
        
        editor::update();

        this->showNotification("successfully flood filled !", NotificationIcon::Info);
    }

    void quickFill() {
        if (const auto count = editor::selection::count(); count < 2) {
            return this->showNotification("no (or too little) objs selected !", NotificationIcon::Warning);
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
            return this->showNotification("quickfill unable to resolve fill type !", NotificationIcon::Warning);
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

    void keyDown(enumKeyCodes key, double timestamp) {
        auto fields = m_fields.self();

        if (fields->specialHold && key == enumKeyCodes::KEY_Escape) {
            fields->specialHold = false;

            return;
        }
        
        EditorUI::keyDown(key, timestamp);
    }

    bool init(LevelEditorLayer* editorLayer) {
        editor::conditionallyRegisterEditTabButtonFrame(
            Settings::FloodFill::enabled.get() && Settings::FloodFill::quickFillButton.get(),
            "quickfill.png"_spr, "quick-fill-button"_spr, 2, [this] (auto) {
                if (Settings::FloodFill::enabled.get()) {
                    this->quickFill();
                }
            }
        );
        
        if (!EditorUI::init(editorLayer)) {
            return false;
        }

        Shared::addUpdateFunc([this] {
            Miscellaneous::shouldHidePreviewObject() = false;

            if (!Settings::FloodFill::enabled.get()) {
                return;
            }

            auto fields = this->m_fields.self();

            if (this->m_isPaused || this->m_editorLayer->m_playbackMode != PlaybackMode::Not || !this->m_selectedObjectIndex || this->m_selectedMode != 2) {
                fields->specialHold = false;
            }

            if (!fields->specialHold) {
                return;
            }

            const auto mouse = this->m_editorLayer->m_objectLayer->convertToNodeSpace(cocos::getMousePos());

            if (fields->specialStart.getDistance(mouse) <= MINIMUM_SPECIAL_MOUSE_DISTANCE) {
                return;
            }

            Miscellaneous::shouldHidePreviewObject() = true;

            const auto size = editor::object::size(this->m_selectedObjectIndex);

            const auto start = this->getGridSnappedPos(CCPoint{
                std::min(fields->specialStart.x, mouse.x), std::min(fields->specialStart.y, mouse.y)
            }) - ccp(size, size) / 2;
            const auto end = this->getGridSnappedPos(CCPoint{
                std::max(fields->specialStart.x, mouse.x), std::max(fields->specialStart.y, mouse.y)
            }) + ccp(size, size) / 2;
            
            const auto col = Settings::FloodFill::chroma.get() 
                ? nwo5::utils::getChroma<ccColor4F>(Shared::ChromaNode::Default) 
                : color_cast<ccColor4F>(Settings::FloodFill::specialPreviewColor.get());

            Shared::getGridDraw()->drawRect(
                start, end, nwo5::utils::setOpacity(col, Settings::FloodFill::specialPreviewFill.get()),
                Settings::FloodFill::specialPreviewThickness.get() / (Settings::FloodFill::scaleWithZoom.get() ? editor::zoom() : 1.0f), col
            );
        });

        nwo5::utils::setupKeybind(this, "flood-fill-special-key", [this] (const Keybind&, bool pDown, bool pRepeat, double) {
            if (!Settings::FloodFill::enabled.get()) {
                return;
            }

            if (Settings::FloodFill::specialAsButton.get()) {
                if (pDown && !pRepeat) {
                    this->quickFill();
                }

                return;
            }

            auto fields = this->m_fields.self();
            
            if (pDown && !pRepeat) {
                fields->specialHold = true;
                fields->specialStart = this->m_editorLayer->m_objectLayer->convertToNodeSpace(cocos::getMousePos());
            }
            else if (!pDown && fields->specialHold) {
                fields->specialHold = false;

                const auto mouse = this->m_editorLayer->m_objectLayer->convertToNodeSpace(cocos::getMousePos());

                if (fields->specialStart.getDistance(mouse) <= MINIMUM_SPECIAL_MOUSE_DISTANCE) {
                    return;
                }

                const auto size = editor::object::size(this->m_selectedObjectIndex);
                const auto min = this->getGridSnappedPos({
                    std::min(fields->specialStart.x, mouse.x), std::min(fields->specialStart.y, mouse.y)
                }) + offsetForKey(this->m_selectedObjectIndex);
                const auto max = this->getGridSnappedPos({
                    std::max(fields->specialStart.x, mouse.x), std::max(fields->specialStart.y, mouse.y)
                }) + offsetForKey(this->m_selectedObjectIndex);

                // i dont think i even need this but idek anymore
                Miscellaneous::removePreviewObject();

                auto placedObjs = CCArray::create();

                for (auto x = min.x; x <= max.x; x += size) {
                    for (auto y = min.y; y <= max.y; y += size) {
                        if (auto obj = this->m_editorLayer->createObject(this->m_selectedObjectIndex, {x, y}, true)) {
                            placedObjs->addObject(obj);
                        }
                    }
                }
                    
                if (Settings::FloodFill::selectFill.get() && Settings::FloodFill::selectSpecialFill.get()) {
                    editor::selection::add(placedObjs, false, true);
                }

                this->m_editorLayer->m_undoObjects->addObject(
                    UndoObject::createWithArray(placedObjs, UndoCommand::Paste)
                );

                editor::update();

                this->showNotification("successfully rect filled !", NotificationIcon::Info);
            }

        });

        return true;
    }
};