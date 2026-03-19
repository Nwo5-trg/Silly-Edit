#pragma once

#include <internal/settings.hpp>

namespace Settings::ReplaceObjects {
    SCAPI_INLINE_CATEGORY("Replace Objects", std::nullopt, "replace-object-logo.png"_spr, SettingCategory::ReplaceObjects)
    inline SavedSetting<bool> enabled{"Enabled", "Replace Objects", true};

    inline SavedSetting<bool> saveSettings{"Save Settings", "Replace Objects", true, "save strings and select filter toggle across sessions"};
    inline SavedSetting<int> replaceWarningThreshold{"Replace Warning Threshold", "Replace Objects", 256, "show a warning when trying to replace this many or more objects"};
}