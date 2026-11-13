#include <features/overlay/place-object-preview/shared.hpp>
#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;

static void showNotification(ZStringView str, NotificationIcon icon) {
    if (FF::notifications) {
        geode::Notification::create(str, icon)->show();
    }
}

namespace FF {
    void EditorUI::rectFill(CCArray* objs) {
        const auto aPos = static_cast<GameObject*>(objs->firstObject())->getRealPosition();
        const auto bPos = static_cast<GameObject*>(objs->lastObject())->getRealPosition();

        if (aPos == bPos) {
            return showNotification("objects overlapped", NotificationIcon::Warning);
        }

        auto base = static_cast<GameObject*>(objs->firstObject());

        const std::string str{base->getSaveString(m_editorLayer)};

        const auto size = CCSize{base->m_scaleX, base->m_scaleY} * object::size(base);
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

                    object::move(obj, {x, y});

                    placedObjs->addObject(obj);
                }
            }
        }

        object::remove(objs, true);

        if (FF::selectFill) {
            selection::set(placedObjs, false, true);
        }

        m_editorLayer->m_undoObjects->addObject(
            UndoObject::createWithArray(placedObjs, UndoCommand::Paste)
        );

        editor::update();

        showNotification("successfully rect filled !", NotificationIcon::Info);
    }

    void EditorUI::createFromRects(const std::vector<FF::Rect>& rects, CCArray* boundry, GameObject* base, bool dontSelectCenter) {
        auto placedObjs = CCArray::create();

        const std::string str{base->getSaveString(m_editorLayer)};
        const auto size = object::size(base);

        for (const auto& rect : rects) {
            if (auto res = m_editorLayer->createObjectsFromString(str, true, true); res && res->count()) {
                auto obj = static_cast<GameObject*>(res->firstObject());
                
                object::move(obj, rect.center());
                object::scale(obj, rect.width() / size, rect.height() / size);

                placedObjs->addObject(obj);
            }
        }

        if (placedObjs->count()) {
            selection::clear();

            if (FF::selectFill) {
                if (!dontSelectCenter) {
                    selection::add(base, false, true);
                }
                selection::add(placedObjs, false, true);
            }
            if (FF::selectBoundry) {
                selection::add(boundry, false, true);
            }

            m_editorLayer->m_undoObjects->addObject(
                UndoObject::createWithArray(placedObjs, UndoCommand::Paste)
            );
        }
        
        editor::update();

        showNotification("successfully flood filled !", NotificationIcon::Info);
    }

    void EditorUI::quickFill() {
        if (const auto count = selection::count(); count < 2) {
            return showNotification("no (or too little) objs selected !", NotificationIcon::Warning);
        }
        else if (count == 2) {
            return this->rectFill(selection::get());
        }

        auto objs = selection::get();
        
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
                FF::gridFloodFill(std::move(FF::rectsFromObjects(objs)), FF::rectFromObject(center), false), objs, center, false
            );
        }
        else {
            auto first = static_cast<GameObject*>(objs->firstObject());
            const auto centerPos = this->getGridSnappedPos(object::center(objs));
            const auto delta = first->getRealPosition() - this->getGridSnappedPos(first->getRealPosition());
            const auto center = FF::rectFromObject(first, centerPos + delta);

            auto rects = FF::gridFloodFill(std::move(FF::rectsFromObjects(objs)), center, false);
            rects.push_back(center);

            this->createFromRects(rects, objs, first, true);
        }
    }





    void Feature::onEditor() {
        auto self = editor::ui<FF::EditorUI>();

        feature.registerKeybind<"quick-fill">([self] (bool down, bool repeat) {
            if (down && !repeat) {
                self->quickFill();
            }
        });
    }

    void Feature::onToggled(bool enabled) {
        auto self = editor::ui<FF::EditorUI>();

        editor::conditionallyRegisterEditTabButtonFrame(
            enabled && FF::quickFillButton,
            "quickfill.png"_spr, "quick-fill-button"_spr, 2, [self] (auto) {
                self->quickFill();
            }
        );
    }
}