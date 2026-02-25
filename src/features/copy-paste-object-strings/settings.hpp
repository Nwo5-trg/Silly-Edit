#pragma once

#include <internal/settings.hpp>

namespace Settings::CopyPasteObjectStrings {
    SE_CATEGORY("Copy Paste Object Strings", "copy-paste-object-strings-logo.png"_spr, SettingCategory::CopyPasteObjectStrings)
    SE_SETTING("Copy Paste Object Strings", "Enabled", bool, enabled, false)
    
    SE_SETTING("Copy Paste Object Strings", "Copy", bool, copy, true)
    SE_SETTING("Copy Paste Object Strings", "Paste", bool, paste, true)
    SE_SETTING("Copy Paste Object Strings", "Fallback Editor", bool, fallbackEditor, true, "fallback to editor clipboard if your clipboard doesnt contain a valid object id, if disabled then just nothing gets pasted")
    SE_SETTING("Copy Paste Object Strings", "Dont Override Editor", bool, dontOverrideEditor, false, "by default when pasting a valid object string, it overrides the default editor clipboard, enabling this setting still pastes the string, but doesnt set the editor clipboard to the string")
    SE_SETTING("Copy Paste Object Strings", "Copy Notification", bool, copyNotification, false)
    SE_SETTING("Copy Paste Object Strings", "Paste Notification", bool, pasteNotification, true)
}