#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>
#include <internal/utils/utils.hpp>
#include "include.hpp"
#include "settings.hpp"

using namespace geode::prelude;

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

        nwo5::utils::setupKeybind(this, "better-select-all-select-all-key", [this] (const Keybind&, bool pDown, bool pRepeat, double) {
            if (Settings::BetterSelectAll::enabled.get() && pDown && !pRepeat) {
                editor::selection::add(
                    BetterSelectAll::getObjectsWithDirection(BetterSelectAll::SelectDirection::All, false),
                    true, true
                );

                editor::update(false, true);
            }
        });
        nwo5::utils::setupKeybind(this, "better-select-all-select-all-left-key", [this] (const Keybind&, bool pDown, bool pRepeat, double) {
            if (Settings::BetterSelectAll::enabled.get() && pDown && !pRepeat) {
                editor::selection::add(
                    BetterSelectAll::getObjectsWithDirection(BetterSelectAll::SelectDirection::West, false),
                    true, true
                );

                editor::update(false, true);
            }
        });
        nwo5::utils::setupKeybind(this, "better-select-all-select-all-down-key", [this] (const Keybind&, bool pDown, bool pRepeat, double) {
            if (Settings::BetterSelectAll::enabled.get() && pDown && !pRepeat) {
                editor::selection::add(
                    BetterSelectAll::getObjectsWithDirection(BetterSelectAll::SelectDirection::South, false),
                    true, true
                );

                editor::update(false, true);
            }
        });
        nwo5::utils::setupKeybind(this, "better-select-all-select-all-up-key", [this] (const Keybind&, bool pDown, bool pRepeat, double) {
            if (Settings::BetterSelectAll::enabled.get() && pDown && !pRepeat) {
                editor::selection::add(
                    BetterSelectAll::getObjectsWithDirection(BetterSelectAll::SelectDirection::North, false),
                    true, true
                );

                editor::update(false, true);
            }
        });
        nwo5::utils::setupKeybind(this, "better-select-all-select-all-right-key", [this] (const Keybind&, bool pDown, bool pRepeat, double) {
            if (Settings::BetterSelectAll::enabled.get() && pDown && !pRepeat) {
                editor::selection::add(
                    BetterSelectAll::getObjectsWithDirection(BetterSelectAll::SelectDirection::East, false),
                    true, true
                );

                editor::update(false, true);
            }
        });
        
        return true;
    }
};