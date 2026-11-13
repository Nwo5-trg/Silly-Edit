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

        feature.registerKeybind<"select-all-key">([] (bool down, bool repeat) {
            if (down && !repeat) {
                selection::add(
                    BetterSelectAll::getObjectsWithDirection(SelectDirection::All, false),
                    true, true
                );

                editor::update(false, true);
            }
        });
        feature.registerKeybind<"select-all-left-key">([] (bool down, bool repeat) {
            if (down && !repeat) {
                selection::add(
                    getObjectsWithDirection(SelectDirection::West, false),
                    true, true
                );

                editor::update(false, true);
            }
        });
        feature.registerKeybind<"select-all-down-key">([] (bool down, bool repeat) {
            if (down && !repeat) {
                selection::add(
                    getObjectsWithDirection(SelectDirection::South, false),
                    true, true
                );

                editor::update(false, true);
            }
        });
        feature.registerKeybind<"select-all-up-key">([] (bool down, bool repeat) {
            if (down && !repeat) {
                selection::add(
                    getObjectsWithDirection(SelectDirection::North, false),
                    true, true
                );

                editor::update(false, true);
            }
        });
        feature.registerKeybind<"select-all-right-key">([] (bool down, bool repeat) {
            if (down && !repeat) {
                selection::add(
                    getObjectsWithDirection(SelectDirection::East, false),
                    true, true
                );

                editor::update(false, true);
            }
        });
    }
}