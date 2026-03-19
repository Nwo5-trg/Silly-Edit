#pragma once

#include <internal/settings.hpp>

namespace Settings::Ruler {
    SCAPI_INLINE_CATEGORY("Ruler", std::nullopt, "ruler-logo.png"_spr, SettingCategory::Ruler)
    inline SavedSetting<bool> enabled{"Enabled", "Ruler", true};

    inline SavedSetting<bool> useGDUnits{"Use GD Units", "Ruler", true, "one block = 10 gd units = 30 units, if enabled will show 45 units as '1, 5'"};
    inline SavedSetting<bool> editorTabButton{"Editor Tab Button", "Ruler", true};

    SCAPI_INLINE_CATEGORY("Ruler - Visual", std::nullopt, "ruler-logo.png"_spr, SettingCategory::Ruler)
    inline SavedSetting<float> thickness{"Thickness", "Ruler - Visual", 1.0f};
    inline SavedSetting<float> padding{"Padding", "Ruler - Visual", 0.0f};
    inline SavedSetting<float> fillOpacity{"Fill Opacity", "Ruler - Visual", 0.0f};
    inline SavedSetting<float> labelSize{"Label Size", "Ruler - Visual", 1.25f};
    inline SavedSetting<float> labelDistance{"Label Distance", "Ruler - Visual", 2.5f};
    inline SavedSetting<bool> dontRotateLabel{"Dont Rotate Label", "Ruler - Visual", false};
    inline SavedSetting<bool> labelOnRight{"Label On Right", "Ruler - Visual", false};
    inline SavedSetting<bool> labelOnBottom{"Label On Bottom", "Ruler - Visual", false};
    inline SavedSetting<bool> chroma{"Chroma", "Ruler - Visual", false};
}