#pragma once

#include <internal/settings.hpp>

namespace Settings::General {
    SE_CATEGORY("General", "settings-button.png"_spr, SettingCategory::General)
    
    SE_SETTING("General", "Save Setting Page", bool, saveSettingPage, true, "remembers what page u were on since last time the settings popup was opened")
    SE_SETTING("General", "Use Logos For Dots", bool, useLogosForDots, true, "if disabled will use small dots like object menu, reload settings popup to apply")
    SE_SETTING("General", "Show Page Arrows", bool, showPageArrows, true, "reload settings popup to apply")
    SE_SETTING("General", "Sayo Device Sensitivity", float, sayoDeviceSensitivity, 0.25f, "gay speed")

    SE_CATEGORY("Keybinds", "settings-button.png"_spr, SettingCategory::Keybinds)
}