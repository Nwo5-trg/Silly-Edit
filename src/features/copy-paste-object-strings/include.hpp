#pragma once

#include <Geode/modify/EditorUI.hpp>
#include <feature/include.hpp>

namespace CopyPasteObjectStrings {
    class $feature(CopyPasteObjectStrings) {
        void onEditor() override;
        void onToggled(bool) override;
    } feature;

    class $feature_modify(EditorUI) {
        static void disableHooksCuzFuckYou();

        void doCopyObjects(bool withColor);
        void doPasteObjects(bool withColor);
    };

    class $setting_category("copy-paste-object-strings-logo.png"_spr);

    inline SillySetting<bool> copy{
        "Copy", feature, true
    };
    inline SillySetting<bool> paste{
        "Paste", feature, true
    };
    inline SillySetting<bool> fallbackEditor{
        "Fallback Editor", feature, true, "fallback to editor clipboard if your clipboard doesnt contain a valid object id, if disabled then just nothing gets pasted"
    };
    inline SillySetting<bool> dontOverrideEditor{
        "Dont Override\nEditor", feature, false, "by default when pasting a valid object string, it overrides the default editor clipboard, enabling this setting still pastes the string, but doesnt set the editor clipboard to the string"
    };
    inline SillySetting<bool> copyNotification{
        "Copy Notification", feature, false
    };
    inline SillySetting<bool> pasteNotification{
        "Paste Notification", feature, true
    };
}