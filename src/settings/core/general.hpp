#pragma once

#include <features/registry.hpp>

namespace sillyedit::settings {
    SILLY_API_INLINE_CATEGORY("General", "Global settings for all of sillyedit", "settings-logo.png"_spr, sillyedit::features::FeatureEnum::General)
    
    inline SillySetting<bool> showSettingsButton{
        "Show Settings Button", "Popup", true, SettingReload::Pause, "show settings button in editor pause menu"
    };
    inline SillySetting<std::string> settingsButtonTexture{
        "Settings Button Texture", "General", "Rainbow", {"Rainbow", "Bi", "Enby", "Femboy", "Gay", "Genderqueer", "Intersex", "Pan", "Trans", "Ery", "Soggy"}, SettingReload::Pause
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