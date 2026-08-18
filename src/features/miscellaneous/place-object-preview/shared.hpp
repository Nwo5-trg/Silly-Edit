#pragma once

#include "include.hpp"

namespace Shared {
    void removePreviewObject();

    inline auto& shouldHidePreviewObject() {
        static bool val = false;
        return val;
    }
}