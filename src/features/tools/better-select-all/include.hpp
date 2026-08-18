#pragma once

#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>
#include <feature/include.hpp>
#include "utils.hpp"

namespace BetterSelectAll {
    class $feature(BetterSelectAll) {
        void onEditor() override;
    } feature;

    class $feature_modify(EditorPauseLayer) {
        void onSelectAll(cocos2d::CCObject* sender);
    };

    class $setting_category("better-select-all-logo.png"_spr, "Select all with/without filters, select all in a certain direction, or select none if thats what you prefer");

    inline SillySetting<bool> saveState{
        "Save State", feature, true, "save toggles for the next time u open the popup"
    };
    inline SillySetting<bool> openPopup{
        "Open Popup", feature, true, SettingReload::Pause, "if false doesnt override select all button as u can only use the keybinds for this feature"
    };
}