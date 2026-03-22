#pragma once

#include <internal/settings.hpp>

namespace Settings::General {
    SCAPI_INLINE_CATEGORY("General", std::nullopt, "settings-logo.png"_spr, SettingCategory::General)
    
    inline SavedSetting<bool> saveSettingPage{"Save Setting Page", "General", true, "remembers what page u were on since last time the settings popup was opened"};
    inline SavedSetting<bool> useLogosForDots{"Use Logos For Dots", "General", true, "if disabled will use small dots like object menu, reload settings popup to apply"};
    inline SavedSetting<bool> showPageArrows{"Show Page Arrows", "General", true, "reload settings popup to apply"};
    inline SavedSetting<float> sayoDeviceSensitivity{"Sayo Device Sensitivity", "General", 1.5f, "gay speed"};

    SCAPI_INLINE_CATEGORY("Keybinds", std::nullopt, "keybinds-logo.png"_spr, SettingCategory::Keybinds)
}