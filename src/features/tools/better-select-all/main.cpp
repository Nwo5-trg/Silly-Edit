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

        nwo5::utils::setupKeybind(self, "better-select-all-select-all-key", [] (const Keybind&, bool pDown, bool pRepeat, double) {
            if (BetterSelectAll::enabled() && pDown && !pRepeat) {
                editor::selection::add(
                    BetterSelectAll::getObjectsWithDirection(BetterSelectAll::SelectDirection::All, false),
                    true, true
                );

                editor::update(false, true);
            }
        });
        nwo5::utils::setupKeybind(self, "better-select-all-select-all-left-key", [] (const Keybind&, bool pDown, bool pRepeat, double) {
            if (BetterSelectAll::enabled() && pDown && !pRepeat) {
                editor::selection::add(
                    BetterSelectAll::getObjectsWithDirection(BetterSelectAll::SelectDirection::West, false),
                    true, true
                );

                editor::update(false, true);
            }
        });
        nwo5::utils::setupKeybind(self, "better-select-all-select-all-down-key", [] (const Keybind&, bool pDown, bool pRepeat, double) {
            if (BetterSelectAll::enabled() && pDown && !pRepeat) {
                editor::selection::add(
                    BetterSelectAll::getObjectsWithDirection(BetterSelectAll::SelectDirection::South, false),
                    true, true
                );

                editor::update(false, true);
            }
        });
        nwo5::utils::setupKeybind(self, "better-select-all-select-all-up-key", [] (const Keybind&, bool pDown, bool pRepeat, double) {
            if (BetterSelectAll::enabled() && pDown && !pRepeat) {
                editor::selection::add(
                    BetterSelectAll::getObjectsWithDirection(BetterSelectAll::SelectDirection::North, false),
                    true, true
                );

                editor::update(false, true);
            }
        });
        nwo5::utils::setupKeybind(self, "better-select-all-select-all-right-key", [] (const Keybind&, bool pDown, bool pRepeat, double) {
            if (BetterSelectAll::enabled() && pDown && !pRepeat) {
                editor::selection::add(
                    BetterSelectAll::getObjectsWithDirection(BetterSelectAll::SelectDirection::East, false),
                    true, true
                );

                editor::update(false, true);
            }
        });
    }
}