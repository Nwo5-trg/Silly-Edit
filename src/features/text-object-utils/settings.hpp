#pragma once

#include <internal/settings.hpp>

namespace Settings::TextObjectUtils {
    SE_CATEGORY("Text Object Utils", "text-object-utils-logo.png"_spr, SettingCategory::TextObjectUtils)
    SE_SETTING("Text Object Utils", "Enabled", bool, enabled, true)

    SE_SETTING("Text Object Utils", "Newline Shortcut", std::string, newlineShortcut, "\\n")
    SE_SETTING("Text Object Utils", "Swap Copy Paste", bool, swapCopyPaste, false)
}