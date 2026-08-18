#pragma once

#include <feature/include.hpp>

namespace SillyKeybinds {
    class $feature(SillyKeybinds) {
        void onEditor() override;
    } feature;

    class $setting_category("silly-keybinds-logo.png"_spr);

    inline SillySetting<bool> ignoreDamageKeybind{
        "Ignore Damage\nKeybind", "Miscellaneous", true
    };
    inline SillySetting<bool> makeObjectInvisibleKeybind{
        "Make Object\nInvisible Keybind", "Miscellaneous", true
    };
    inline SillySetting<int> invisibleWithGroup{
        "Invisible With\nGroup", "Miscellaneous", 0, {0, 9999}, "by setting to a number above 0, keybind will, instead of toggling always hide, toggle the object haivng an invisible group"
    };
}