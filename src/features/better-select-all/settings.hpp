#pragma once

#include <internal/settings.hpp>

namespace Settings::BetterSelectAll {
    SE_CATEGORY("Better Select All", "better-select-all-logo.png"_spr, SettingCategory::BetterSelectAll)
    SE_SETTING("Better Select All", "Enabled", bool, enabled, true)
    
    SE_SETTING("Better Select All", "Save State", bool, saveState, true, "save toggles for the next time u open the popup")
    SE_SETTING("Better Select All", "Open Popup", bool, openPopup, true, "if false doesnt override select all button as u can only use the keybinds for this feature")
}