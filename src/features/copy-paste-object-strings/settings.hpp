#pragma once

#include <internal/settings.hpp>

namespace Settings::CopyPasteObjectStrings {
    SILLY_API_INLINE_CATEGORY("Copy Paste Object Strings", std::nullopt, "copy-paste-object-strings-logo.png"_spr, SettingCategory::CopyPasteObjectStrings)
    inline SillySetting<bool> enabled{"Enabled", "Copy Paste Object Strings", false};
    
    inline SillySetting<bool> copy{"Copy", "Copy Paste Object Strings", true};
    inline SillySetting<bool> paste{"Paste", "Copy Paste Object Strings", true};
    inline SillySetting<bool> fallbackEditor{"Fallback Editor", "Copy Paste Object Strings", true, "fallback to editor clipboard if your clipboard doesnt contain a valid object id, if disabled then just nothing gets pasted"};
    inline SillySetting<bool> dontOverrideEditor{"Dont Override Editor", "Copy Paste Object Strings", false, "by default when pasting a valid object string, it overrides the default editor clipboard, enabling this setting still pastes the string, but doesnt set the editor clipboard to the string"};
    inline SillySetting<bool> copyNotification{"Copy Notification", "Copy Paste Object Strings", false};
    inline SillySetting<bool> pasteNotification{"Paste Notification", "Copy Paste Object Strings", true};
}