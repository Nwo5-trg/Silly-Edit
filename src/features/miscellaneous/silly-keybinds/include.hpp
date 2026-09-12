#pragma once

#include <feature/include.hpp>

namespace SillyKeybinds {
    class $feature(SillyKeybinds) {
        void onEditor() override;
    } feature;

    class $setting_category("silly-keybinds-logo.png"_spr, "Some random editor keybinds i personally find useful :3c");
}