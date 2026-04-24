#pragma once

#include <internal/settings.hpp>

using namespace nwo5::settings;

namespace Settings::ZoomInput {
    SILLY_API_INLINE_CATEGORY("Zoom Input", std::nullopt, "zoom-input-logo.png"_spr, SettingCategory::ZoomInput)
    inline SillySetting<bool> zoomInput{"Enabled", "Zoom Input", true, SettingReload::Editor};

    inline SillySetting<int> rounding{"Rounding", "Zoom Input", 3};
    inline SillySetting<float> zoomInputScale{"Zoom Input Scale", "Zoom Input", 1.0f, SettingReload::Editor};
    inline SillySetting<float> zoomInputOffset{"Zoom Input Offset", "Zoom Input", -17.5f, SettingReload::Editor};
    inline SillySetting<bool> centered{"Centered", "Zoom Input", false, SettingReload::Editor, "will position at center of the screen instead of under position slider"};

    SILLY_API_INLINE_CATEGORY("Zoom Bypass", std::nullopt, "zoom-input-logo.png"_spr, SettingCategory::ZoomInput)
    inline SillySetting<bool> zoomBypass{"Enabled", "Zoom Bypass", false, "settings disabled when tinker is installed, might js remove later tbh"};

    inline SillySetting<float> zoomScrollMin{"Zoom Scroll Min", "Zoom Bypass", 0.01f, "technically most zoomed *out* possible"};
    inline SillySetting<float> zoomScrollMax{"Zoom Scroll Max", "Zoom Bypass", 9999.0f, "technically most zoomed *in* possible"};
    inline SillySetting<bool> multiplicativeZoom{"Multiplicative Zoom", "Zoom Bypass", true, "multiples and divides zoom by mod instead of adding and subtracting"};
    inline SillySetting<bool> onlyButtons{"Only Buttons", "Zoom Bypass", false, "if true multiplicative zoom doesnt apply for scrolling"};
    inline SillySetting<float> multiplicativeZoomMod{"Multiplicative Zoom Mod", "Zoom Bypass", 1.05f};
    inline SillySetting<float> zoomStep{"Zoom Step", "Zoom Bypass", 0.1f, "will also be treated as a zoom maximum so 0 zoom isnt a thing"};
    inline SillySetting<bool> noConstrainPosition{"No Constrain Position", "Zoom Bypass", false, "use at ur own risk idk if this is a good hacky workaround uwu"};
}