#pragma once

#include <internal/settings.hpp>

namespace Settings::ReplaceObjects {
    SILLY_API_INLINE_CATEGORY("Replace Objects", std::nullopt, "replace-object-logo.png"_spr, SettingCategory::ReplaceObjects)
    inline SillySetting<bool> enabled{"Enabled", "Replace Objects", true, SettingReload::Editor};

    inline SillySetting<bool> saveSettings{"Save Settings", "Replace Objects", true, "save strings and select filter toggle across sessions"};
    inline SillySetting<int> replaceWarningThreshold{"Replace Warning Threshold", "Replace Objects", 256, "show a warning when trying to replace this many or more objects"};
    inline SillySetting<float> width{"Width", "Replace Objects", 150.0f, SettingReload::Editor};
}