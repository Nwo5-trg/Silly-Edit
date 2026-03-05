#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>
#include <internal/utils/utils.hpp>
#include "include.hpp"
#include "settings.hpp"

using namespace geode::prelude;

namespace BetterSelectAll {
    CCArray* getObjectsWithDirection(SelectDirection pDirection, bool pSelectedObjectsCenter) {
        auto objs = CCArray::create();

        const auto center = pSelectedObjectsCenter ? Editor::Selection::center() : Editor::center();

        const auto shouldInclude = [&] (CCPoint pPos) {
            switch (pDirection) {
                case SelectDirection::NorthWest: return pPos.x <= center.x && pPos.y >= center.y;
                case SelectDirection::North: return  pPos.y >= center.y;
                case SelectDirection::NorthEast: return pPos.x >= center.x && pPos.y >= center.y;
                case SelectDirection::West: return pPos.x <= center.x;
                case SelectDirection::All: return true;
                case SelectDirection::East: return pPos.x >= center.x;
                case SelectDirection::SouthWest: return pPos.x <= center.x && pPos.y <= center.y;
                case SelectDirection::South: return pPos.y <= center.y;
                case SelectDirection::SouthEast: return pPos.x >= center.x && pPos.y <= center.y;
            }
        };
        
        for (auto obj : CCArrayExt<GameObject*>(Editor::Object::getAll())) {
            if (shouldInclude(obj->getRealPosition())) {
                objs->addObject(obj);
            }
        }

        return objs;
    }
}

class $modify(EditorPauseLayer) {
    void onSelectAll(CCObject* sender) {
        if (Settings::BetterSelectAll::enabled.get() && Settings::BetterSelectAll::openPopup.get()) {
            BetterSelectAll::BetterSelectAllPopup::create()->show();
        }
        else {
            EditorPauseLayer::onSelectAll(sender);
        }
    }
};

class $modify(EditorUI) {
    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) {
            return false;
        }

        Utils::setupKeybind(this, "better-select-all-select-all-key", [this] (const Keybind&, bool pDown, bool pRepeat, double) {
            if (Settings::BetterSelectAll::enabled.get() && pDown && !pRepeat) {
                Editor::Selection::add(
                    BetterSelectAll::getObjectsWithDirection(BetterSelectAll::SelectDirection::All, false),
                    true, true
                );

                Editor::update(false, true);
            }
        });
        Utils::setupKeybind(this, "better-select-all-select-all-left-key", [this] (const Keybind&, bool pDown, bool pRepeat, double) {
            if (Settings::BetterSelectAll::enabled.get() && pDown && !pRepeat) {
                Editor::Selection::add(
                    BetterSelectAll::getObjectsWithDirection(BetterSelectAll::SelectDirection::West, false),
                    true, true
                );

                Editor::update(false, true);
            }
        });
        Utils::setupKeybind(this, "better-select-all-select-all-down-key", [this] (const Keybind&, bool pDown, bool pRepeat, double) {
            if (Settings::BetterSelectAll::enabled.get() && pDown && !pRepeat) {
                Editor::Selection::add(
                    BetterSelectAll::getObjectsWithDirection(BetterSelectAll::SelectDirection::South, false),
                    true, true
                );

                Editor::update(false, true);
            }
        });
        Utils::setupKeybind(this, "better-select-all-select-all-up-key", [this] (const Keybind&, bool pDown, bool pRepeat, double) {
            if (Settings::BetterSelectAll::enabled.get() && pDown && !pRepeat) {
                Editor::Selection::add(
                    BetterSelectAll::getObjectsWithDirection(BetterSelectAll::SelectDirection::North, false),
                    true, true
                );

                Editor::update(false, true);
            }
        });
        Utils::setupKeybind(this, "better-select-all-select-all-right-key", [this] (const Keybind&, bool pDown, bool pRepeat, double) {
            if (Settings::BetterSelectAll::enabled.get() && pDown && !pRepeat) {
                Editor::Selection::add(
                    BetterSelectAll::getObjectsWithDirection(BetterSelectAll::SelectDirection::East, false),
                    true, true
                );

                Editor::update(false, true);
            }
        });
        
        return true;
    }
};