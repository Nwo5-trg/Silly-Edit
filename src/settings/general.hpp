#pragma once

#include <features/registry.hpp>

namespace sillyedit::settings {
    SILLY_API_INLINE_CATEGORY("General", "Global settings for all of sillyedit", "settings-logo.png"_spr, sillyedit::features::FeatureEnum::General)
    
    inline SillySetting<bool> saveSettingPage{
        "Save Setting Page", "General", true, "remembers what page u were on since last time the settings popup was opened"
    };
    inline SillySetting<bool> showSettingsButton{
        "Show Settings Button", "General", true, SettingReload::Pause, "show settings button in editor pause menu"
    };
    inline SillySetting<bool> showReloadWarnings{
        "Show Reload Warnings", "General", true
    };
    inline SillySetting<bool> useCustomBackground{
        "Use Custom Background", "General", false, SettingReload::Popup, "use custom background for settings popup, reccomend changing theme to \"Alt\" to make everything a little brighter"
    };
    inline SillySetting<int> settingsBackground{
        "Custom Background", "General", 2, {1, 59}, SettingReload::Popup, "can be any in game background (0-59)"
    };
    inline SillySetting<std::string> popupTheme{
        "Popup Theme", "General", "Default", {"Default", "Alt", "Geode"}, SettingReload::Popup
    };
    inline SillySetting<bool> showTooltips{
        "Show Tooltips", "General", true, SettingReload::Popup
    };
    inline SillySetting<std::string> settingsButtonTexture{
        "Settings Button Texture", "General", "Rainbow", {"Bi", "Enby", "Femboy", "Gay", "Genderqueer", "Intersex", "Pan", "Rainbow", "Trans"}, SettingReload::Pause
    };
    inline SillySetting<bool> disableModWarningPopup{
        "Disable Mod Warning Popup", "General", false, " stops showing a warning every time you start the game about the mod being in beta or wtv"
    };
    inline SillySetting<int> invisibleWithGroup{
        "Invisible With Group", "General", 0, {0, 9999}, "affects context menu and silly keybinds, if number not set to zero, any features that toggle an objects visibility will instead set the objects group to this"
    };
    inline SillySetting<float> sayoDeviceSensitivity{
        "Sayo Device Sensitivity", "General", 1.5f, {0.1f, std::nullopt}, "gay speed"
    };
    inline SillySetting<float> sayoDeviceScreenBrightness{
        "Sayo Device Screen Brightness", "General", 0.5f, {0.0f, 1.0f}, "gay saturation"
    };
}