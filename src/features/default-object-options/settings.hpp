#pragma once

#include <internal/settings.hpp>

namespace Settings::DefaultObjectOptions {
    SCAPI_INLINE_CATEGORY("Default Object Options", std::nullopt, "default-object-options-logo.png"_spr, SettingCategory::DefaultObjectOptions)
    inline SavedSetting<bool> enabled{"Enabled", "Default Object Options", true};

    inline SavedSetting<bool> dontFade{"Dont Fade", "Default Object Options", false};
    inline SavedSetting<bool> dontEnter{"Dont Enter", "Default Object Options", false, "dont recommend turning this one on - dont fade is prolly wat u want but u do u"};
    inline SavedSetting<bool> noGlow{"No Glow", "Default Object Options", false};
    inline SavedSetting<bool> useJSON{"Use JSON", "Default Object Options", false, "most ppl will prolly find this useful enough for auto enabling noglow/dontfade but if you want more advanced settings like obj str f***ery u can use json :333"};
    inline SavedSetting<std::string> path{"JSON Path", "Default Object Options", std::string{"entries.json"}, "relative to config folder (mainly just a convenient way to show the file name u need)"};
}