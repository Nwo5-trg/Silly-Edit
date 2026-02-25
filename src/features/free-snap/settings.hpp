#pragma once

#include <internal/settings.hpp>

namespace Settings::FreeSnap {
    SE_CATEGORY("Free Snap", "free-snap-logo.png"_spr, SettingCategory::FreeSnap)
    SE_SETTING("Free Snap", "Enabled", bool, enabled, true)

    SE_SETTING("Free Snap", "Grid Size", float, gridSize, 15.0f)
    SE_SETTING("Free Snap", "Snap Object Color", cocos2d::ccColor3B, snapObjectColor, cocos2d::ccc3(255, 127, 0), "color snap object will be changed to")
    SE_SETTING("Free Snap", "Snap Indicator", bool, snapIndicator, true, "previews where the snap object will snap to")
    SE_SETTING("Free Snap", "Snap Indicator Fill", float, snapIndicatorFill, 0.25f, "opacity of snap indicator fill")
    SE_SETTING("Free Snap", "Snap Indicator Thickness", float, snapIndicatorThickness, 1.0f)
    SE_SETTING("Free Snap", "Selected Object Color", cocos2d::ccColor3B, selectedObjectColor, cocos2d::ccc3(0, 255, 0), "color selected objects will be changed to")
    SE_SETTING("Free Snap", "Chroma", bool, chroma, false)
}