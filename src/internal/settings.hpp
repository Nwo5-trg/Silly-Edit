#pragma once

#include <nwo5.silly-api/include/settings/include.hpp>

using namespace nwo5::settings::prelude;

enum class SettingCategory {
    General,
    Keybinds,
    DefaultObjectOptions,
    Ruler,
    BetterScale,
    ReplaceObjects,
    FreeSnap,
    TextObjectUtils,
    ZoomInput,
    CopyPasteObjectStrings,
    BetterSelectAll,
    ObjectTabIcons
};

namespace Settings::General {
    SCAPI_INLINE_CATEGORY("General", std::nullopt, "settings-logo.png"_spr, SettingCategory::General)
    
    inline SavedSetting<bool> saveSettingPage{"Save Setting Page", "General", true, "remembers what page u were on since last time the settings popup was opened"};
    inline SavedSetting<bool> useLogosForDots{"Use Logos For Dots", "General", true, "if disabled will use small dots like object menu, reload settings popup to apply"};
    inline SavedSetting<bool> showPageArrows{"Show Page Arrows", "General", true, "reload settings popup to apply"};
    // prolly shouldve come up with a better way than ifdefs for savedsettings but wtv
    #ifdef GEODE_IS_DESKTOP
        inline SavedSetting<bool> showSettingsButton{"Show Settings Button", "General", true, "show settings button in editor pause menu"};
    #endif
    inline SavedSetting<float> sayoDeviceSensitivity{"Sayo Device Sensitivity", "General", 1.5f, "gay speed"};
    inline SavedSetting<float> sayoDeviceScreenBrightness{"Sayo Device Screen Brightness", "General", 1.0f, "gay saturation"};

    SCAPI_INLINE_CATEGORY("Keybinds", std::nullopt, "keybinds-logo.png"_spr, SettingCategory::Keybinds)
}