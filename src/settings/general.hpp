#pragma once

#include "silly-setting.hpp"
#include "order.hpp"

namespace Settings::General {
    SILLY_API_INLINE_CATEGORY("General", std::nullopt, "settings-logo.png"_spr, Settings::Order::General)
    
    inline SillySetting<bool> saveSettingPage{"Save Setting Page", "General", true, "remembers what page u were on since last time the settings popup was opened"};
    inline SillySetting<bool> useLogosForDots{"Use Logos For Dots", "General", true, SettingReload::Popup, "if disabled will use small dots like object menu"};
    inline SillySetting<bool> showPageArrows{"Show Page Arrows", "General", true, SettingReload::Popup};
    inline SillySetting<bool> showSettingsButton{"Show Settings Button", "General", true, SettingReload::Pause, "show settings button in editor pause menu"};
    inline SillySetting<bool> showReloadWarnings{"Show Reload Warnings", "General", true};
    inline SillySetting<std::string> settingsButtonTexture{"Settings Button Texture", "General", "Rainbow", {"Bi", "Enby", "Femboy", "Gay", "Genderqueer", "Intersex", "Pan", "Rainbow", "Trans"}, SettingReload::Pause};
    inline SillySetting<float> sayoDeviceSensitivity{"Sayo Device Sensitivity", "General", 1.5f, {0.1f, std::nullopt}, "gay speed"};
    inline SillySetting<float> sayoDeviceScreenBrightness{"Sayo Device Screen Brightness", "General", 0.5f, {0.0f, 1.0f}, "gay saturation"};
    inline SillySetting<bool> disableModWarningPopup{"Disable Mod Warning Popup", "General", false, " stops showing a warning every time you start the game about the mod being in beta or wtv"};

    SILLY_API_INLINE_CATEGORY("Keybinds", std::nullopt, "keybinds-logo.png"_spr, Settings::Order::Keybinds)
}