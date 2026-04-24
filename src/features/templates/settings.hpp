#pragma once

#include <internal/settings.hpp>

namespace Settings::Templates {
    SILLY_API_INLINE_CATEGORY("Templates", std::nullopt, "templates-logo.png"_spr, SettingCategory::Templates)
    inline SillySetting<bool> enabled{"Enabled", "Templates", true, SettingReload::Editor};
}