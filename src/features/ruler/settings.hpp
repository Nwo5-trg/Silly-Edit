#pragma once

#include <internal/settings.hpp>

namespace Settings::Ruler {
    SILLY_API_INLINE_CATEGORY("Ruler", std::nullopt, "ruler-logo.png"_spr, SettingCategory::Ruler)
    inline SillySetting<bool> enabled{"Enabled", "Ruler", true, SettingReload::Editor};

    inline SillySetting<bool> useGDUnits{"Use GD Units", "Ruler", true, "one block = 10 gd units = 30 units, if enabled will show 45 units as '1, 5'"};
    inline SillySetting<bool> editorTabButton{"Editor Tab Button", "Ruler", true, SettingReload::Editor};

    // SILLY_API_INLINE_CATEGORY("Ruler - Visual", std::nullopt, "ruler-logo.png"_spr, SettingCategory::Ruler)
    inline SillySetting<float> thickness{"Thickness", "Ruler", 1.0f, {0.0f, std::nullopt}};
    inline SillySetting<bool> scaleWithZoom{"Scale With\nZoom", "Ruler", true};
    inline SillySetting<float> padding{"Padding", "Ruler", 0.0f};
    inline SillySetting<float> fillOpacity{"Fill Opacity", "Ruler", 0.0f, {0.0f, 1.0f}};
    inline SillySetting<float> labelSize{"Label Scale", "Ruler", 1.25f, {0.0f, std::nullopt}};
    inline SillySetting<float> labelDistance{"Label Distance", "Ruler", 2.5f};
    inline SillySetting<bool> dontRotateLabel{"Dont Rotate\nLabel", "Ruler", false};
    inline SillySetting<bool> labelOnRight{"Label On\nRight", "Ruler", false};
    inline SillySetting<bool> labelOnBottom{"Label On\nBottom", "Ruler", false};
    inline SillySetting<bool> chroma{"Chroma", "Ruler", false};
}