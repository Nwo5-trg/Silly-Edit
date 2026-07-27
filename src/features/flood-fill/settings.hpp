#pragma once

#include <internal/settings.hpp>

namespace Settings::FloodFill {
    SILLY_API_INLINE_CATEGORY("Flood Fill", std::nullopt, "flood-fill-logo.png"_spr, SettingCategory::FloodFill)
    inline SillySetting<bool> enabled{"Enabled", "Flood Fill", true, SettingReload::Editor};

    inline SillySetting<bool> notifications{"Notifications", "Flood Fill", true};
    inline SillySetting<bool> selectFill{"Select Fill", "Flood Fill", true, "select filled objects after filling"};
    inline SillySetting<bool> selectBoundry{"Select Boundry", "Flood Fill", false, "select flood fill boundry after filling"};
    inline SillySetting<bool> quickFillButton{"Quick Fill\nButton", "Flood Fill", true, SettingReload::Editor};
    inline SillySetting<bool> selectSpecialFill{"Select Special\nFill", "Flood Fill", true, "select filled objects after rect filling with special key"};
    inline SillySetting<bool> specialAsButton{"Special As\nButton", "Flood Fill", 
    #ifdef GEODE_IS_DESKTOP
        true
    #else
        false
    #endif
    , "use special key as a shortcut to quick fill button instead of its own functionality"};
    inline SillySetting<float> specialPreviewThickness{"Special Preview\nThickness", "Flood Fill", 1.0f, {0.0f, std::nullopt}};
    inline SillySetting<bool> scaleWithZoom{"Scale With\nZoom", "Flood Fill", false};
    inline SillySetting<cocos2d::ccColor3B> specialPreviewColor{"Special Preview\nColor", "Flood Fill", { 255, 50, 200 }};
    inline SillySetting<float> specialPreviewFill{"Special Preview\nFill", "Flood Fill", 0.0f, {0.0f, 1.0f}};
    inline SillySetting<bool> chroma{"Chroma", "Flood Fill", false};
}