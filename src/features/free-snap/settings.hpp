#pragma once

#include <internal/settings.hpp>

namespace Settings::FreeSnap {
    SCAPI_INLINE_CATEGORY("Free Snap", std::nullopt, "free-snap-logo.png"_spr, SettingCategory::FreeSnap)
    inline SavedSetting<bool> enabled{"Enabled", "Free Snap", true};

    inline SavedSetting<float> gridSize{"Grid Size", "Free Snap", 15.0f};
    inline SavedSetting<cocos2d::ccColor3B> snapObjectColor{"Snap Object Color", "Free Snap", cocos2d::ccc3(255, 127, 0), "color snap object will be changed to"};
    inline SavedSetting<bool> snapIndicator{"Snap Indicator", "Free Snap", true, "previews where the snap object will snap to"};
    inline SavedSetting<float> snapIndicatorFill{"Snap Indicator Fill", "Free Snap", 0.25f, "opacity of snap indicator fill"};
    inline SavedSetting<float> snapIndicatorThickness{"Snap Indicator Thickness", "Free Snap", 1.0f};
    inline SavedSetting<cocos2d::ccColor3B> selectedObjectColor{"Selected Object Color", "Free Snap", cocos2d::ccc3(0, 255, 0), "color selected objects will be changed to"};
    inline SavedSetting<bool> chroma{"Chroma", "Free Snap", false};
}