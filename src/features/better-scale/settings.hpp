#pragma once

#include <internal/settings.hpp>

namespace Settings::BetterScale {
    SE_CATEGORY("Better Scale", "better-scale-logo.png"_spr, SettingCategory::BetterScale)
    SE_SETTING("Better Scale", "Enabled", bool, enabled, true)

    SE_SETTING("Better Scale", "Scale Shortcuts", bool, scaleShortcuts, true)
    SE_SETTING("Better Scale", "Shortucts String", std::string, shortcutsString, "0.25,0.5,1,2,4", "seperate scales with commas")
    SE_SETTING("Better Scale", "Allow Negative", bool, allowNegative, false, "dont use abs for negative scales")
    SE_SETTING("Better Scale", "New Lock Texture", bool, newLockTexture, true)
    SE_SETTING("Better Scale", "Switch Mode Button", bool, switchModeButton, true)
    SE_SETTING("Better Scale", "Control Size", float, controlSize, 1.0f, "multiplies scale control size")
    SE_SETTING("Better Scale", "Lock Control Size", bool, lockControlSize, false, "treats control size as a static scale that doesnt change with zoom")
    SE_SETTING("Better Scale", "Control Offset", float, controlOffset, 40.0f, "y offset from default control position")
}