#pragma once

#include <features/registry.hpp>

using sillyedit::features::FeatureEnum;

namespace sillyedit::settings {
    $execute {
        SettingsManager::get()->registerCategory(FeatureEnum::General, "Global settings for all of sillyedit", "settings-logo.png"_spr);
    }

    inline Setting<bool> showSettingsButton{
        "Show Settings Button", FeatureEnum::General, true, SettingReload::Pause, "show settings button in editor pause menu"
    };
    inline Setting<std::string> settingsButtonTexture{
        "Settings Button Texture", FeatureEnum::General, "Rainbow", {"Rainbow", "Bi", "Enby", "Femboy", "Gay", "Genderqueer", "Intersex", "Pan", "Trans", "Ery", "Soggy"}, SettingReload::Pause
    };
    inline Setting<bool> disableModWarningPopup{
        "Disable Mod Warning Popup", FeatureEnum::General, false, " stops showing a warning every time you start the game about the mod being in beta or wtv"
    };
    inline Setting<int> invisibleWithGroup{
        "Invisible With Group", FeatureEnum::General, 0, {0, 9999}, "affects context menu and silly keybinds, if number not set to zero, any features that toggle an objects visibility will instead set the objects group to this"
    };
    inline Setting<float> sayoDeviceSensitivity{
        "Sayo Device Sensitivity", FeatureEnum::General, 1.5f, {0.1f, std::nullopt}, "gay speed"
    };
    inline Setting<float> sayoDeviceScreenBrightness{
        "Sayo Device Screen Brightness", FeatureEnum::General, 0.5f, {0.0f, 1.0f}, "gay saturation"
    };
}