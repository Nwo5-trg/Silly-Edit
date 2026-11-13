#pragma once

#include <features/registry.hpp>

using sillyedit::features::FeatureEnum;

namespace sillyedit::settings {
    $execute {
        SettingsManager::get()->registerCategory(FeatureEnum::Popup, "Settings popup themes and other stuff", "popup-logo.png"_spr);
    }

    inline Setting<bool> saveSettingPage{
        "Save Setting Page", FeatureEnum::Popup, true, "remembers what page u were on since last time the settings popup was opened"
    };
    inline Setting<bool> showReloadWarnings{
        "Show Reload Warnings", FeatureEnum::Popup, true
    };
    inline Setting<bool> showTooltips{
        "Show Tooltips", FeatureEnum::Popup, true, SettingReload::Popup, SettingCondition::DesktopOnly
    };
    inline Setting<OpacityType> popupOverlayOpacity{
        "Popup Overlay Opacity", FeatureEnum::Popup, 50, SettingReload::Popup, "opacity of the black backdrop behind the settings popup"
    };
    inline Setting<bool> useCustomBackground{
        "Use Custom Background", FeatureEnum::Popup, false, "use custom background for settings popup, reccomend changing theme to \"Alt\" to make everything a little brighter"
    };
    inline Setting<int> settingsBackground{
        "Custom Background", FeatureEnum::Popup, 2, {1, 59}, "can be any in game background (0-59)"
    };
    inline Setting<std::string> popupTheme{
        "Popup Theme", FeatureEnum::Popup, "Default", {"Default", "Alt", "Geode", "Catpuccin", "Custom"},
    };
    inline Setting<cocos2d::ccColor4B> backgroundColor{
        "Background Color", FeatureEnum::Popup, {152, 86, 51, 255}, "set \"Popup Theme\" to custom to use"
    };
    inline Setting<cocos2d::ccColor4B> sideBarColor{
        "Side Bar Color", FeatureEnum::Popup, {84, 84, 84, 255}, "set \"Popup Theme\" to custom to use"
    };
    inline Setting<cocos2d::ccColor4B> topBarColor{
        "Top Bar Color", FeatureEnum::Popup, {104, 104, 104, 255}, "set \"Popup Theme\" to custom to use"
    };
    inline Setting<cocos2d::ccColor4B> topShadowColor{
        "Top Shadow Color", FeatureEnum::Popup, {0, 0, 0, 100}, "set \"Popup Theme\" to custom to use"
    };
    inline Setting<cocos2d::ccColor4B> floatingMenuColor{
        "Floating Menu Color", FeatureEnum::Popup, {0, 0, 0, 50}, "set \"Popup Theme\" to custom to use"
    };
    inline Setting<cocos2d::ccColor3B> textColor{
        "Text Color", FeatureEnum::Popup, {255, 255, 255}
    };
    inline Setting<cocos2d::ccColor3B> unmatchedSettingColor{
        "Unmatched Setting Color", FeatureEnum::Popup, {100, 100, 100}, "color feature buttons/setting labels are set to when they are unmatched by search, set \"Popup Theme\" to custom to use"
    };
    inline Setting<float> unmatchedSelectedFeatureDarkness{
        "Unmatched Selected Feature Value", FeatureEnum::Popup, 0.5f, {0.0f, 1.0f}, "value (brightness) of chroma effect on feature button when an unmatched feature is currently selected , set \"Popup Theme\" to custom to use"
    };
}