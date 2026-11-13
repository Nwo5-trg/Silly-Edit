#pragma once

#include <feature/include.hpp>

namespace HideWithPlaytest {
    class $feature(HideWithPlaytest) {} feature;
    
    class $setting_category("hide-with-playtest-logo.png"_spr, "Configure what is visible during playtesting");

    inline Setting<bool> hideTriggers{
        "Hide Triggers", feature, true
    };
    inline Setting<int> triggerOpacity{
        "Trigger Opacity", feature, 0, {0, 255}
    };
    inline Setting<bool> hideSpecialBlocks{
        "Hide Special Blocks", feature, true
    };
    inline Setting<int> specialBlockOpacity{
        "Special Block Opacity", feature, 0, {0, 255}
    };
};