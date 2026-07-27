#pragma once

#include <internal/settings.hpp>

namespace Settings::Miscellaneous {
    SILLY_API_INLINE_CATEGORY("Miscellaneous", std::nullopt, "miscellaneous-logo.png"_spr, SettingCategory::Miscellaneous)

    inline SillySetting<bool> placeObjectPreview{"Place Object\nPreview", "Miscellaneous", 
    #ifdef GEODE_IS_DESKTOP
        true
    #else
        false
    #endif
    , "only works on android with mouse and doesnt work on ios ! show a ghost of the object ur about to place"};
    inline SillySetting<int> placeObjectPreviewOpacity{"Preview\nOpacity", "Miscellaneous", 75, {0, 255}};
    inline SillySetting<bool> fixObjectLabel{"Fix Object\nLabel", "Miscellaneous", true, "sometimes it doesnt show, this should fix it"};
    inline SillySetting<bool> templates{"Templates", "Miscellaneous", true, "save a level as a \"template\" which all new levels now copy (save template button in leveloptions)"};
}