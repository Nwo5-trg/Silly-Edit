#pragma once

#include <internal/settings.hpp>

namespace Settings::SelectionUtils {
    SILLY_API_INLINE_CATEGORY("Selection Utils", std::nullopt, "selection-utils-logo.png"_spr, SettingCategory::SelectionUtils)
    inline SillySetting<bool> enabled{"Enabled", "Selection Utils", true};

    inline SillySetting<float> gridSize{"Grid Size", "Selection Utils", 15.0f, {1.0f, std::nullopt}};
    inline SillySetting<cocos2d::ccColor3B> snapObjectColor{"Snap Object\nColor", "Selection Utils", cocos2d::ccc3(255, 127, 0), "color snap object will be changed to"};
    inline SillySetting<bool> snapIndicator{"Snap Indicator", "Selection Utils", true, "previews where the snap object will snap to"};
    inline SillySetting<float> snapIndicatorFill{"Snap Indicator\nFill", "Selection Utils", 0.25f, {0.0f, 1.0f}, "opacity of snap indicator fill"};
    inline SillySetting<float> snapIndicatorThickness{"Snap Indicator\nThickness", "Selection Utils", 2.5f, {0.0f, std::nullopt}};
    inline SillySetting<cocos2d::ccColor3B> selectedObjectColor{"Selected Object\nColor", "Selection Utils", cocos2d::ccc3(0, 255, 0), "color selected objects will be changed to"};
    inline SillySetting<bool> alwaysSingleSelect{"Always Single\nSelect", "Selection Utils", true, "HACKY ! clicking on a selected object with multiple objects selected, will deselect all but that object"};
    inline SillySetting<bool> chroma{"Chroma", "Selection Utils", false};
}