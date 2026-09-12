#pragma once

#include "include.hpp"

namespace sillyedit::shared {
    inline auto& getLayerSettingsPtr() {
        static BetterLayers::LayerSettings* val = nullptr;
        return val;
    }
}