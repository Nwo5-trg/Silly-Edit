#pragma once

#include <internal/settings.hpp>

namespace Settings::Compat {
    SILLY_API_INLINE_CATEGORY("Compat", std::nullopt, "compat-logo.png"_spr, SettingCategory::Compat)

    inline SillySetting<bool> disableTinkerButton{"Disable Tinker Button", "Compat", false, SettingReload::Pause};
    inline SillySetting<bool> tinkerBEButton{"Tinker BE Button", "Compat", false, SettingReload::Pause, "tinkers basically better better edit so"};
    inline SillySetting<bool> tinkerBEUIButton{"Tinker BE UI Button", "Compat", false, SettingReload::Editor, "change the tinker hide ui button texture to the be one"};
};