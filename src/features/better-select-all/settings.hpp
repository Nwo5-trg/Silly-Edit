#pragma once

#include <internal/settings.hpp>

namespace Settings::BetterSelectAll {
    SCAPI_INLINE_CATEGORY("Better Select All", std::nullopt, "better-select-all-logo.png"_spr, SettingCategory::BetterSelectAll)
    inline SavedSetting<bool> enabled{"Enabled", "Better Select All", true};
    
    inline SavedSetting<bool> saveState{"Save State", "Better Select All", true, "save toggles for the next time u open the popup"};
    inline SavedSetting<bool> openPopup{"Open Popup", "Better Select All", true, "if false doesnt override select all button as u can only use the keybinds for this feature"};
}