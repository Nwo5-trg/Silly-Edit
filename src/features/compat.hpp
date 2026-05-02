#pragma once

#include <internal/settings.hpp>

namespace Settings::Compat {
    SILLY_API_INLINE_CATEGORY("Compat", std::nullopt, "compat-logo.png"_spr, SettingCategory::Compat)

    inline SillySetting<bool> disableTinkerButton{"Disable Tinker Button", "Compat", false, SettingReload::Pause};
    inline SillySetting<bool> disableBetterEditButton{"Disable Better Edit Button", "Compat", false, SettingReload::Pause};
    inline SillySetting<bool> removeBEMoveButtons{"Remove BE Move Buttons", "Compat", false, SettingReload::Editor};
    inline SillySetting<bool> tinkerV1Button{"Tinker V1 Button", "Compat", false, SettingReload::Pause, "gl alpha"};
};