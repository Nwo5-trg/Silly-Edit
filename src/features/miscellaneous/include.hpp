#pragma once

namespace Miscellaneous {
    void removePreviewObject();

    inline auto& shouldHidePreviewObject() {
        static bool val = false;
        return val;
    }
};