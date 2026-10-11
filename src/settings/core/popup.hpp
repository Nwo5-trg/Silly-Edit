#pragma once

#include <features/registry.hpp>

namespace sillyedit::settings {
    SILLY_API_INLINE_CATEGORY("Popup", "Settings popup themes and other stuff", "popup-logo.png"_spr, sillyedit::features::FeatureEnum::Popup)
    
    inline SillySetting<bool> saveSettingPage{
        "Save Setting Page", "Popup", true, "remembers what page u were on since last time the settings popup was opened"
    };
    inline SillySetting<bool> showReloadWarnings{
        "Show Reload Warnings", "Popup", true
    };
    inline SillySetting<bool> showTooltips{
        "Show Tooltips", "Popup", true, SettingReload::Popup, SettingCondition::DesktopOnly
    };
    inline SillySetting<int> popupOverlayOpacity{
        "Popup Overlay Opacity", "Popup", 50, {0, 255}, SettingReload::Popup, "opacity of the black backdrop behind the settings popup"
    };
    inline SillySetting<bool> useCustomBackground{
        "Use Custom Background", "Popup", false, "use custom background for settings popup, reccomend changing theme to \"Alt\" to make everything a little brighter"
    };
    inline SillySetting<int> settingsBackground{
        "Custom Background", "Popup", 2, {1, 59}, "can be any in game background (0-59)"
    };
    inline SillySetting<std::string> popupTheme{
        "Popup Theme", "Popup", "Default", {"Default", "Alt", "Geode", "Catpuccin", "Custom"},
    };
    inline SillySetting<cocos2d::ccColor4B> backgroundColor{
        "Background Color", "Popup", {152, 86, 51, 255}, "set \"Popup Theme\" to custom to use"
    };
    inline SillySetting<cocos2d::ccColor4B> sideBarColor{
        "Side Bar Color", "Popup", {84, 84, 84, 255}, "set \"Popup Theme\" to custom to use"
    };
    inline SillySetting<cocos2d::ccColor4B> topBarColor{
        "Top Bar Color", "Popup", {104, 104, 104, 255}, "set \"Popup Theme\" to custom to use"
    };
    inline SillySetting<cocos2d::ccColor4B> topShadowColor{
        "Top Shadow Color", "Popup", {0, 0, 0, 100}, "set \"Popup Theme\" to custom to use"
    };
    inline SillySetting<cocos2d::ccColor4B> floatingMenuColor{
        "Floating Menu Color", "Popup", {0, 0, 0, 50}, "set \"Popup Theme\" to custom to use"
    };
    inline SillySetting<cocos2d::ccColor3B> textColor{
        "Text Color", "Popup", {255, 255, 255}
    };
    inline SillySetting<cocos2d::ccColor3B> unmatchedSettingColor{
        "Unmatched Setting Color", "Popup", {100, 100, 100}, "color feature buttons/setting labels are set to when they are unmatched by search, set \"Popup Theme\" to custom to use"
    };
    inline SillySetting<float> unmatchedSelectedFeatureDarkness{
        "Unmatched Selected Feature Value", "Popup", 0.5f, {0.0f, 1.0f}, "value (brightness) of chroma effect on feature button when an unmatched feature is currently selected , set \"Popup Theme\" to custom to use"
    };
}