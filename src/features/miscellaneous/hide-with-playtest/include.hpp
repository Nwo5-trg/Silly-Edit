#pragma once

#include <feature/include.hpp>

namespace HideWithPlaytest {
    class $feature(HideWithPlaytest) {} feature;
    
    class $setting_category("hide-with-playtest-logo.png"_spr, "Configure what is visible during playtesting");

    inline SillySetting<bool> hideTriggers{
        "Hide Triggers", feature, true
    };
    inline SillySetting<int> triggerOpacity{
        "Trigger Opacity", feature, 0, {0, 255}
    };
    inline SillySetting<bool> hideSpecialBlocks{
        "Hide Special\nBlocks", feature, true
    };
    inline SillySetting<int> specialBlockOpacity{
        "Special Block\nOpacity", feature, 0, {0, 255}
    };
};