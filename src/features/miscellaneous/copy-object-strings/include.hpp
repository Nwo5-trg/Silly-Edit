#pragma once

#include <Geode/modify/EditorUI.hpp>
#include <feature/include.hpp>
#include <utils/include.hpp>

namespace CopyObjectStrings {
    class $feature(CopyObjectStrings) {
        void setup() override {
            m_data.defaultEnabled = false;
            m_data.incompatMods = TINKER_EDIT_ID;
            m_data.incompatMessage = "disables tinkers hooks >w<";
        }

        void onToggled(bool enabled) override;
    } feature;

    class $feature_modify(EditorUI) {
        static void disableHooksCuzFuckYou();

        void doCopyObjects(bool withColor);
        void doPasteObjects(bool withColor);
    };

    class $setting_category("copy-object-strings-logo.png"_spr, "Copy obj strings and paste from your clipboard");

    inline Setting<bool> copy{
        "Copy", feature, true
    };
    inline Setting<bool> paste{
        "Paste", feature, true
    };
    inline Setting<bool> fallbackEditor{
        "Fallback Editor", feature, true, "fallback to editor clipboard if your clipboard doesnt contain a valid object id, if disabled then just nothing gets pasted"
    };
    inline Setting<bool> dontOverrideEditor{
        "Dont Override Editor", feature, false, "by default when pasting a valid object string, it overrides the default editor clipboard, enabling this setting still pastes the string, but doesnt set the editor clipboard to the string"
    };
    inline Setting<bool> copyNotification{
        "Copy Notification", feature, false
    };
    inline Setting<bool> pasteNotification{
        "Paste Notification", feature, true
    };
}