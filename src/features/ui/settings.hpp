#pragma once

#include <internal/settings.hpp>

namespace Settings::UI {
    SILLY_API_INLINE_CATEGORY("UI (WIP)", std::nullopt, "ui-logo.png"_spr, SettingCategory::UI)
    inline SillySetting<bool> enabled{"Enabled", "UI (WIP)", true};

    inline SillySetting<bool> eventLink{"Event Link\nPopup", "UI (WIP)", true};
    inline SillySetting<std::string> pinnedEvents{"Pinned Events", "UI (WIP)", "69,70,71,72,73,74,12,8", "comma seperated list"};
    inline SillySetting<bool> cameraMode{"Camera Mode\nPopup", "UI (WIP)", true};
}