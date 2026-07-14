#pragma once

#include <internal/settings.hpp>

namespace Settings::Miscellaneous {
    SILLY_API_INLINE_CATEGORY("Miscellaneous", std::nullopt, "miscellaneous-logo.png"_spr, SettingCategory::Miscellaneous)

    #ifdef GEODE_IS_DESKTOP // i need a better way of doing platform specific settings but wtv
    inline SillySetting<bool> placeObjectPreview{"Place Object Preview", "Miscellaneous", true};
    inline SillySetting<int> placeObjectPreviewOpacity{"Preview Opacity", "Miscellaneous", 75, {0, 255}, "from 0-255"};
    #endif
    inline SillySetting<bool> minorBugfixes{"Minor Bugfixes", "Miscellaneous", true, "fixes that are like 10 lines so its not even worth seperating them"};
}