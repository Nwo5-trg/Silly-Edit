#pragma once

#include "include.hpp"

namespace sillyedit::shared {
    void removePreviewObject();

    inline auto& shouldHidePreviewObject() {
        static bool val = false;
        return val;
    }
}