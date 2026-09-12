#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;

namespace BetterSelectAll {
    void EditorPauseLayer::onSelectAll(CCObject* sender) {
        if (BetterSelectAll::enabled() && BetterSelectAll::openPopup) {
            BetterSelectAllPopup::create()->show();
        }
        else {
            GD::EditorPauseLayer::onSelectAll(sender);
        }
    }





    void Feature::onEditor() {
        auto self = editor::ui();

        feature.registerKeybind<"select-all-key">([] (bool pDown, bool pRepeat) {
            if (pDown && !pRepeat) {
                selection::add(
                    BetterSelectAll::getObjectsWithDirection(SelectDirection::All, false),
                    true, true
                );

                editor::update(false, true);
            }
        });
        feature.registerKeybind<"select-all-left-key">([] (bool pDown, bool pRepeat) {
            if (pDown && !pRepeat) {
                selection::add(
                    getObjectsWithDirection(SelectDirection::West, false),
                    true, true
                );

                editor::update(false, true);
            }
        });
        feature.registerKeybind<"select-all-down-key">([] (bool pDown, bool pRepeat) {
            if (pDown && !pRepeat) {
                selection::add(
                    getObjectsWithDirection(SelectDirection::South, false),
                    true, true
                );

                editor::update(false, true);
            }
        });
        feature.registerKeybind<"select-all-up-key">([] (bool pDown, bool pRepeat) {
            if (pDown && !pRepeat) {
                selection::add(
                    getObjectsWithDirection(SelectDirection::North, false),
                    true, true
                );

                editor::update(false, true);
            }
        });
        feature.registerKeybind<"select-all-right-key">([] (bool pDown, bool pRepeat) {
            if (pDown && !pRepeat) {
                selection::add(
                    getObjectsWithDirection(SelectDirection::East, false),
                    true, true
                );

                editor::update(false, true);
            }
        });
    }
}