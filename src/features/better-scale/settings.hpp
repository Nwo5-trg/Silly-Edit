#pragma once

#include <internal/settings.hpp>

namespace Settings::BetterScale {
    SILLY_API_INLINE_CATEGORY("Better Scale", std::nullopt, "better-scale-logo.png"_spr, SettingCategory::BetterScale)
    inline SillySetting<bool> enabled{"Enabled", "Better Scale", true, SettingReload::Editor};

    inline SillySetting<bool> scaleShortcuts{"Scale Shortcuts", "Better Scale", true, SettingReload::Editor};
    inline SillySetting<std::string> shortcutsString{"Shortucts String", "Better Scale", std::string{"0.25,0.5,1,2,4"}, SettingReload::Editor, "seperate scales with commas"};
    inline SillySetting<bool> allowNegative{"Allow Negative", "Better Scale", false, "dont use abs for negative scales"};
    inline SillySetting<bool> newLockTexture{"New Lock Texture", "Better Scale", true, SettingReload::Editor};
    inline SillySetting<bool> switchModeButton{"Switch Mode Button", "Better Scale", true, SettingReload::Editor};
    inline SillySetting<float> controlSize{"Control Size", "Better Scale", 1.0f, "multiplies scale control size"};
    inline SillySetting<bool> lockControlSize{"Lock Control Size", "Better Scale", false, "treats control size as a static scale that doesnt change with zoom"};
    inline SillySetting<float> controlOffset{"Control Offset", "Better Scale", 40.0f, "y offset from default control position"};
}