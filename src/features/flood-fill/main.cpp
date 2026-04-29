#include <internal/utils/utils.hpp>
#include <Geode/modify/EditorUI.hpp>
#include "settings.hpp"
#include "include.hpp"

using namespace geode::prelude;

class $modify(FloodFillEditorUI, EditorUI) {
    FloodFill::Rect rectFromObject(GameObject* pObj, std::optional<CCPoint> pCenter = std::nullopt) {
        return {
            pCenter.has_value() ? pCenter.value() : pObj->getRealPosition(), 
            CCSize(pObj->m_scaleX, pObj->m_scaleY) * editor::object::size(pObj)
        };
    }
    std::vector<FloodFill::Rect> rectsFromObjects(CCArray* pObjs) {
        std::vector<FloodFill::Rect> out;

        for (auto obj : CCArrayExt<GameObject*>(pObjs)) {
            out.emplace_back(obj->getRealPosition(), CCSize(obj->m_scaleX, obj->m_scaleY) * editor::object::size(obj));
        }

        return out;
    }

    void showNotification(ZStringView pStr, NotificationIcon pIcon) {
        if (Settings::FloodFill::notifications.get()) {
            Notification::create(pStr, pIcon)->show();
        }
    }

    void rectFill(CCArray* pObjs) {
        const auto aPos = static_cast<GameObject*>(pObjs->firstObject())->getRealPosition();
        const auto bPos = static_cast<GameObject*>(pObjs->lastObject())->getRealPosition();

        if (aPos == bPos) {
            return showNotification("objects overlapped", NotificationIcon::Warning);
        }

        auto base =static_cast<GameObject*>(pObjs->firstObject());

        const auto str = copyObjects(CCArray::createWithObject(base), false, false);

        CCSize gridSize = CCSize(base->m_scaleX, base->m_scaleY) * editor::object::size(base);
        const auto min = ccp(
            std::min(aPos.x, bPos.x), std::min(aPos.y, bPos.y)
        );
        const auto max = ccp(
            std::max(aPos.x, bPos.x), std::max(aPos.y, bPos.y)
        );

        auto placedObjs = CCArray::create();

        for (auto x = min.x; x <= max.x; x += gridSize.width) {
            for (auto y = min.y; y <= max.y; y += gridSize.height) {
                auto obj = static_cast<GameObject*>(m_editorLayer->createObjectsFromString(str, true, true)->firstObject());

                editor::object::move(obj, {x, y});

                placedObjs->addObject(obj);
            }
        }

        editor::object::remove(pObjs, true);

        m_editorLayer->addToUndoList(UndoObject::createWithArray(placedObjs, UndoCommand::Paste), true);

        if (Settings::FloodFill::selectFill.get()) {
            editor::selection::add(placedObjs, true, true);
        }

        editor::update();

        showNotification("successfully rect filled !", NotificationIcon::Info);
    }
    void createFromRects(const std::vector<FloodFill::Rect>& pRects, CCArray* pBoundry, GameObject* pBase) {
        auto placed = CCArray::create();

        const auto str = copyObjects(CCArray::createWithObject(pBase), false, false);
        const auto size = editor::object::size(pBase);

        for (auto rect : pRects) {
            log::error("[{}], [{}]", rect.start, rect.end);
        }

        for (const auto& rect : pRects) {
            auto obj = static_cast<GameObject*>(m_editorLayer->createObjectsFromString(str, true, true)->firstObject());
            
            editor::object::move(obj, rect.center());
            editor::object::scale(obj, rect.width() / size, rect.height() / size);

            placed->addObject(obj);
        }

        if (placed->count()) {
            m_editorLayer->addToUndoList(UndoObject::createWithArray(placed, UndoCommand::Paste), true);
        }

        if (Settings::FloodFill::selectFill.get()) {
            editor::selection::add(pBase, true, true);
            editor::selection::add(placed, true, true);
        }
        if (Settings::FloodFill::selectBoundry.get()) {
            editor::selection::add(pBoundry, true, true);
        }
        
        editor::update();

        showNotification("successfully flood filled !", NotificationIcon::Info);
    }

    void quickFill() {
        if (const auto count = editor::selection::count(); count < 2) {
            return showNotification("no (or too little) objs selected !", NotificationIcon::Warning);
        }
        else if (count == 2) {
            return rectFill(editor::selection::get());
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
            return showNotification("quickfill unable to resolve fill type ! (mayb use floodfill)", NotificationIcon::Warning);
        }

        if (center) {
            objs->removeObject(center, false);
        }

        createFromRects(
            FloodFill::gridFloodFill(
                std::move(rectsFromObjects(objs)), 
                center 
                    ? rectFromObject(center) 
                    : rectFromObject(static_cast<GameObject*>(objs->firstObject()), editor::object::center(objs, true)), 
                false
            ),
            objs, center ? center : static_cast<GameObject*>(objs->firstObject())
        );
    }

    bool init(LevelEditorLayer* editorLayer) {
        if (!Settings::FloodFill::enabled.get() || !Settings::FloodFill::quickFillButton.get()) {
            editor::unregisterEditTabButton("quick-fill-button"_spr);
        }
        else {
            editor::registerEditTabButtonFrame("quickfill.png"_spr, "quick-fill-button"_spr, 2, [this] (auto) {
                if (Settings::FloodFill::enabled.get()) {
                    quickFill();
                }
            });
        }
        
        if (!EditorUI::init(editorLayer)) {
            return false;
        }

        nwo5::utils::setupKeybind(this, "flood-fill-quick-fill-key", [this] (const Keybind&, bool pDown, bool pRepeat, double) {
            if (Settings::FloodFill::enabled.get() && pDown && !pRepeat) {
                quickFill();
            }
        });

        return true;
    }
};