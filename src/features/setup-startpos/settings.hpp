#pragma once

#include <internal/settings.hpp>

using namespace nwo5::settings;

namespace Settings::SetupStartpos {
    SILLY_API_INLINE_CATEGORY("Setup Startpos", std::nullopt, "setup-startpos-logo.png"_spr, SettingCategory::SetupStartpos)
    inline SillySetting<bool> enabled{"Enabled", "Setup Startpos", true};

    inline SillySetting<bool> gamemode{"Gamemode", "Setup Startpos", true};
    inline SillySetting<bool> freeMode{"Free Mode", "Setup Startpos", true, "if clean startpos is installed"};
    inline SillySetting<bool> speed{"Speed", "Setup Startpos", true};
    inline SillySetting<bool> mini{"Mini", "Setup Startpos", true};
    inline SillySetting<bool> mirror{"Mirror", "Setup Startpos", true};
    inline SillySetting<bool> dual{"Dual", "Setup Startpos", false};
    inline SillySetting<bool> gravity{"Gravity", "Setup Startpos", true, "tries to calculate gravity based on orbs and portals and stuff (will b completely broken for memory levels mind u)"};

}