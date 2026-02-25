#pragma once

#include <internal/settings.hpp>

namespace Settings::DefaultObjectOptions {
    SE_CATEGORY("Default Object Options", "default-object-options-logo.png"_spr, SettingCategory::DefaultObjectOptions)
    SE_SETTING("Default Object Options", "Enabled", bool, enabled, true)

    SE_SETTING("Default Object Options", "Dont Fade", bool, dontFade, false)
    SE_SETTING("Default Object Options", "Dont Enter", bool, dontEnter, false, "dont recommend turning this one on - dont fade is prolly wat u want but u do u")
    SE_SETTING("Default Object Options", "No Glow", bool, noGlow, false)
    SE_SETTING("Default Object Options", "Use JSON", bool, useJSON, false, "most ppl will prolly find this useful enough for auto enabling noglow/dontfade but if you want more advanced settings like obj str f***ery u can use json :333")
    SE_SETTING("Default Object Options", "JSON Path", std::string, path, "entries.json", "relative to config folder (mainly just a convenient way to show the file name u need)")
}