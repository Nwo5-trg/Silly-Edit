#pragma once

#include <internal/settings.hpp>

using namespace nwo5::settings;

namespace Settings::ZoomInput {
    SILLY_API_INLINE_CATEGORY("Zoom Input", std::nullopt, "zoom-input-logo.png"_spr, SettingCategory::ZoomInput)
    inline SillySetting<bool> zoomInput{"Enabled", "Zoom Input", true, SettingReload::Editor};

    inline SillySetting<int> rounding{"Rounding", "Zoom Input", 3, {0, 7}};
    inline SillySetting<float> zoomInputScale{"Zoom Input\nScale", "Zoom Input", 1.0f, {0.0f, std::nullopt}, SettingReload::Editor};
    inline SillySetting<float> zoomInputOffset{"Zoom Input\nOffset", "Zoom Input", -17.5f, SettingReload::Editor};
    inline SillySetting<bool> centered{"Centered", "Zoom Input", false, SettingReload::Editor, "will position at center of the screen instead of under position slider"};
    inline SillySetting<bool> noConstrainPosition{"No Constrain\nPosition", "Zoom Input", false, "use at ur own risk idk if this is a good hacky workaround uwu, works regardless of zook input being enabled"};
}