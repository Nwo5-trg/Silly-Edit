#pragma once

#include <internal/settings.hpp>

namespace Settings::ReplaceObjects {
    SE_CATEGORY("Replace Objects", "replace-object-logo.png"_spr, SettingCategory::ReplaceObjects)
    SE_SETTING("Replace Objects", "Enabled", bool, enabled, true)

    SE_SETTING("Replace Objects", "Save Settings", bool, saveSettings, true, "save strings and select filter toggle across sessions")
    SE_SETTING("Replace Objects", "Replace Warning Threshold", int, replaceWarningThreshold, 256, "show a warning when trying to replace this many or more objects")
}