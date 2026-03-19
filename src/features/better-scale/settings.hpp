#pragma once

#include <internal/settings.hpp>

namespace Settings::BetterScale {
    SCAPI_INLINE_CATEGORY("Better Scale", std::nullopt, "better-scale-logo.png"_spr, SettingCategory::BetterScale)
    inline SavedSetting<bool> enabled{"Enabled", "Better Scale", true};

    inline SavedSetting<bool> scaleShortcuts{"Scale Shortcuts", "Better Scale", true};
    inline SavedSetting<std::string> shortcutsString{"Shortucts String", "Better Scale", std::string{"0.25,0.5,1,2,4"}, "seperate scales with commas"};
    inline SavedSetting<bool> allowNegative{"Allow Negative", "Better Scale", false, "dont use abs for negative scales"};
    inline SavedSetting<bool> newLockTexture{"New Lock Texture", "Better Scale", true};
    inline SavedSetting<bool> switchModeButton{"Switch Mode Button", "Better Scale", true};
    inline SavedSetting<float> controlSize{"Control Size", "Better Scale", 1.0f, "multiplies scale control size"};
    inline SavedSetting<bool> lockControlSize{"Lock Control Size", "Better Scale", false, "treats control size as a static scale that doesnt change with zoom"};
    inline SavedSetting<float> controlOffset{"Control Offset", "Better Scale", 40.0f, "y offset from default control position"};
}