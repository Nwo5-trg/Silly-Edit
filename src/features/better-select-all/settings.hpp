#pragma once

#include <internal/settings.hpp>

namespace Settings::BetterSelectAll {
    SILLY_API_INLINE_CATEGORY("Better Select All", std::nullopt, "better-select-all-logo.png"_spr, SettingCategory::BetterSelectAll)
    inline SillySetting<bool> enabled{"Enabled", "Better Select All", true, SettingReload::Pause};
    
    inline SillySetting<bool> saveState{"Save State", "Better Select All", true, "save toggles for the next time u open the popup"};
    inline SillySetting<bool> openPopup{"Open Popup", "Better Select All", true, SettingReload::Pause, "if false doesnt override select all button as u can only use the keybinds for this feature"};
}