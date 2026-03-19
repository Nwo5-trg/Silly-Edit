#pragma once

#include <internal/settings.hpp>

using namespace nwo5::settings;

namespace Settings::ZoomInput {
    SCAPI_INLINE_CATEGORY("Zoom Input", std::nullopt, "zoom-input-logo.png"_spr, SettingCategory::ZoomInput)
    inline SavedSetting<bool> enabled{"Enabled", "Zoom Input", true};

    inline SavedSetting<bool> zoomInput{"Zoom Input", "Zoom Input", true};
    inline SavedSetting<int> rounding{"Rounding", "Zoom Input", 3};
    inline SavedSetting<float> zoomInputScale{"Zoom Input Scale", "Zoom Input", 1.0f};
    inline SavedSetting<float> zoomInputOffset{"Zoom Input Offset", "Zoom Input", -17.5f};
    inline SavedSetting<bool> centered{"Centered", "Zoom Input", false, "will position at center of the screen instead of under position slider"};

    SCAPI_INLINE_CATEGORY("Zoom Bypass", std::nullopt, "zoom-input-logo.png"_spr, SettingCategory::ZoomInput)
    inline SavedSetting<bool> zoomBypass{"Zoom Bypass", "Zoom Bypass", true, "enables following tweaks (zoom input does not have to be enabled)"};
    inline SavedSetting<float> zoomScrollMin{"Zoom Scroll Min", "Zoom Bypass", 0.01f, "technically most zoomed *out* possible"};
    inline SavedSetting<float> zoomScrollMax{"Zoom Scroll Max", "Zoom Bypass", 9999.0f, "technically most zoomed *in* possible"};
    inline SavedSetting<bool> multiplicativeZoom{"Multiplicative Zoom", "Zoom Bypass", true, "multiples and divides zoom by mod instead of adding and subtracting"};
    inline SavedSetting<bool> onlyButtons{"Only Buttons", "Zoom Bypass", false, "if true multiplicative zoom doesnt apply for scrolling"};
    inline SavedSetting<float> multiplicativeZoomMod{"Multiplicative Zoom Mod", "Zoom Bypass", 1.05f};
    inline SavedSetting<float> zoomStep{"Zoom Step", "Zoom Bypass", 0.1f, "will also be treated as a zoom maximum so 0 zoom isnt a thing"};
    inline SavedSetting<bool> noConstrainPosition{"No Constrain Position", "Zoom Bypass", false, "use at ur own risk idk if this is a good hacky workaround uwu"};
}