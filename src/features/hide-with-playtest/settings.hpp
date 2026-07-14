#pragma once

#include <internal/settings.hpp>

namespace Settings::HideWithPlaytest {
    SILLY_API_INLINE_CATEGORY("Hide With Playtest", std::nullopt, "hide-with-playtest-logo.png"_spr, SettingCategory::HideWithPlaytest)
    inline SillySetting<bool> enabled{"Enabled", "Hide With Playtest", true};

    inline SillySetting<bool> hideTriggers{"Hide Triggers", "Hide With Playtest", true};
    inline SillySetting<int> triggerOpacity{"Trigger Opacity", "Hide With Playtest", 0, {0, 255}, "from 0-255"};
    inline SillySetting<bool> hideSpecialBlocks{"Hide Special Blocks", "Hide With Playtest", true};
    inline SillySetting<int> specialBlockOpacity{"Special Block Opacity", "Hide With Playtest", 0, {0, 255}, "from 0-255"};
}