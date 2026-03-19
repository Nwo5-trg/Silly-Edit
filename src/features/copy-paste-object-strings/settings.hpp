#pragma once

#include <internal/settings.hpp>

namespace Settings::CopyPasteObjectStrings {
    SCAPI_INLINE_CATEGORY("Copy Paste Object Strings", std::nullopt, "copy-paste-object-strings-logo.png"_spr, SettingCategory::CopyPasteObjectStrings)
    inline SavedSetting<bool> enabled{"Enabled", "Copy Paste Object Strings", false};
    
    inline SavedSetting<bool> copy{"Copy", "Copy Paste Object Strings", true};
    inline SavedSetting<bool> paste{"Paste", "Copy Paste Object Strings", true};
    inline SavedSetting<bool> fallbackEditor{"Fallback Editor", "Copy Paste Object Strings", true, "fallback to editor clipboard if your clipboard doesnt contain a valid object id, if disabled then just nothing gets pasted"};
    inline SavedSetting<bool> dontOverrideEditor{"Dont Override Editor", "Copy Paste Object Strings", false, "by default when pasting a valid object string, it overrides the default editor clipboard, enabling this setting still pastes the string, but doesnt set the editor clipboard to the string"};
    inline SavedSetting<bool> copyNotification{"Copy Notification", "Copy Paste Object Strings", false};
    inline SavedSetting<bool> pasteNotification{"Paste Notification", "Copy Paste Object Strings", true};
}