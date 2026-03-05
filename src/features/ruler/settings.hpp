#pragma once

#include <internal/settings.hpp>

namespace Settings::Ruler {
    SE_CATEGORY("Ruler", "ruler-logo.png"_spr, SettingCategory::Ruler)
    SE_SETTING("Ruler", "Enabled", bool, enabled, true)

    SE_SETTING("Ruler", "Use GD Units", bool, useGDUnits, true, "one block = 10 gd units = 30 units, if enabled will show 45 units as '1, 5'")
    SE_SETTING("Ruler", "Editor Tab Button", bool, editorTabButton, true)

    SE_CATEGORY("Ruler - Visual", "ruler-logo.png"_spr, SettingCategory::Ruler)
    SE_SETTING("Ruler - Visual", "Thickness", float, thickness, 1.0f)
    SE_SETTING("Ruler - Visual", "Padding", float, padding, 0.0f)
    SE_SETTING("Ruler - Visual", "Fill Opacity", float, fillOpacity, 0.0f)
    SE_SETTING("Ruler - Visual", "Label Size", float, labelSize, 1.25f)
    SE_SETTING("Ruler - Visual", "Label Distance", float, labelDistance, 2.5f)
    SE_SETTING("Ruler - Visual", "Dont Rotate Label", bool, dontRotateLabel, false)
    SE_SETTING("Ruler - Visual", "Label On Right", bool, labelOnRight, false)
    SE_SETTING("Ruler - Visual", "Label On Bottom", bool, labelOnBottom, false)
    SE_SETTING("Ruler - Visual", "Chroma", bool, chroma, false)
}