#pragma once

#include <internal/settings.hpp>

namespace Settings::TextObjectUtils {
    SCAPI_INLINE_CATEGORY("Text Object Utils", std::nullopt, "text-object-utils-logo.png"_spr, SettingCategory::TextObjectUtils)
    inline SavedSetting<bool> enabled{"Enabled", "Text Object Utils", true};

    inline SavedSetting<std::string> newlineShortcut{"Newline Shortcut", "Text Object Utils", std::string{"\\n"}};
    inline SavedSetting<bool> swapCopyPaste{"Swap Copy Paste", "Text Object Utils", false};
}