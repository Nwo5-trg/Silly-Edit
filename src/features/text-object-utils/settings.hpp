#pragma once

#include <internal/settings.hpp>

namespace Settings::TextObjectUtils {
    SILLY_API_INLINE_CATEGORY("Text Object Utils", std::nullopt, "text-object-utils-logo.png"_spr, SettingCategory::TextObjectUtils)
    inline SillySetting<bool> enabled{"Enabled", "Text Object Utils", true};

    inline SillySetting<std::string> newlineShortcut{"Newline Shortcut", "Text Object Utils", "\\n"};
    inline SillySetting<bool> swapCopyPaste{"Swap Copy Paste", "Text Object Utils", false};
}