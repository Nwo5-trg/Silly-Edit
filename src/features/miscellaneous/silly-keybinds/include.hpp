#pragma once

#include <feature/include.hpp>

namespace SillyKeybinds {
    class $feature(SillyKeybinds) {
        void onEditor() override;
    } feature;

    class $setting_category("silly-keybinds-logo.png"_spr, "Some random editor keybinds i personally find useful :3c");

    inline SillySetting<int> invisibleWithGroup{
        "Invisible With\nGroup", feature, 0, {0, 9999}, "by setting to a number above 0, keybind will, instead of toggling always hide, toggle the object haivng an invisible group"
    };
}