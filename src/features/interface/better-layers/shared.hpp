#pragma once

#include "include.hpp"

namespace Shared {
    inline auto& getLayerSettingsPtr() {
        static BetterLayers::LayerSettings* val = nullptr;
        return val;
    }
}