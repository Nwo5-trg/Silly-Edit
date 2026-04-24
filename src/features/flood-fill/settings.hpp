#pragma once

#include <internal/settings.hpp>

namespace Settings::FloodFill {
    SILLY_API_INLINE_CATEGORY("Flood Fill", std::nullopt, "flood-fill-logo.png"_spr, SettingCategory::FloodFill)
    inline SillySetting<bool> enabled{"Enabled", "Flood Fill", true, SettingReload::Editor};

    inline SillySetting<bool> notifications{"Notifications", "Flood Fill", true};
    inline SillySetting<bool> selectFill{"Select Fill", "Flood Fill", true};
    inline SillySetting<bool> selectBoundry{"Select Boundry", "Flood Fill", false};
    inline SillySetting<bool> quickFillButton{"Quick Fill Button", "Flood Fill", true, SettingReload::Editor};
}