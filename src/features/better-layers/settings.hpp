#pragma once

#include <internal/settings.hpp>

namespace Settings::BetterLayers {
    SILLY_API_INLINE_CATEGORY("Better Layers", std::nullopt, "better-layers-logo.png"_spr, SettingCategory::BetterLayers)
    inline SillySetting<bool> enabled{"Enabled", "Better Layers", true, SettingReload::Editor};

    inline SillySetting<bool> nextFreeButton{"Next Free Button", "Better Layers", true, SettingReload::Editor};
    inline SillySetting<bool> lockButton{"Lock Button", "Better Layers", true, SettingReload::Editor};
    inline SillySetting<bool> unselectableHiddenLayers{"Unselectable Hidden Layers", "Better Layers", true};
    inline SillySetting<int> layerOpacity{"Layer Opacity", "Better Layers", 50};
    inline SillySetting<int> unfocusedLayerOpacity{"Unfocused Layer Opacity", "Better Layers", 25};
    inline SillySetting<bool> unselectableUnfocusedLayers{"Unselectable Unfocused Layers", "Better Layers", false};
}