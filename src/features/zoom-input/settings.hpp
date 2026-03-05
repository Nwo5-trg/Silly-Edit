#pragma once

#include <internal/settings.hpp>

namespace Settings::ZoomInput {
    SE_CATEGORY("Zoom Input", "zoom-input-logo.png"_spr, SettingCategory::ZoomInput)
    SE_SETTING("Zoom Input", "Enabled", bool, enabled, true)

    SE_SETTING("Zoom Input", "Zoom Input", bool, zoomInput, true)
    SE_SETTING("Zoom Input", "Rounding", int, rounding, 3)
    SE_SETTING("Zoom Input", "Zoom Input Scale", float, zoomInputScale, 1.0f)
    SE_SETTING("Zoom Input", "Zoom Input Offset", float, zoomInputOffset, -17.5f)
    SE_SETTING("Zoom Input", "Centered", bool, centered, false, "will position at center of the screen instead of under position slider")

    SE_CATEGORY("Zoom Bypass", "zoom-input-logo.png"_spr, SettingCategory::ZoomInput)
    SE_SETTING("Zoom Bypass", "Zoom Bypass", bool, zoomBypass, true, "enables following tweaks (zoom input does not have to be enabled)")
    SE_SETTING("Zoom Bypass", "Zoom Scroll Min", float, zoomScrollMin, 0.01f, "technically most zoomed *out* possible");
    SE_SETTING("Zoom Bypass", "Zoom Scroll Max", float, zoomScrollMax, 9999.0f, "technically most zoomed *in* possible");
    SE_SETTING("Zoom Bypass", "Multiplicative Zoom", bool, multiplicativeZoom, true, "multiples and divides zoom by mod instead of adding and subtracting")
    SE_SETTING("Zoom Bypass", "Only Buttons", bool, onlyButtons, false, "if true multiplicative zoom doesnt apply for scrolling")
    SE_SETTING("Zoom Bypass", "Multiplicative Zoom Mod", float, multiplicativeZoomMod, 1.05f)
    SE_SETTING("Zoom Bypass", "Zoom Step", float, zoomStep, 0.1f, "will also be treated as a zoom maximum so 0 zoom isnt a thing")
    SE_SETTING("Zoom Bypass", "No Constrain Position", bool, noConstrainPosition, false, "use at ur own risk idk if this is a good hacky workaround uwu")
}