#pragma once

#include <internal/settings.hpp>

namespace Settings::DefaultObjectOptions {
    SILLY_API_INLINE_CATEGORY("Default Object Options", std::nullopt, "default-object-options-logo.png"_spr, SettingCategory::DefaultObjectOptions)
    inline SillySetting<bool> enabled{"Enabled", "Default Object Options", true};

    inline SillySetting<bool> dontFade{"Dont Fade", "Default Object Options", false, SettingReload::Editor};
    inline SillySetting<bool> dontEnter{"Dont Enter", "Default Object Options", false, SettingReload::Editor, "dont recommend turning this one on - dont fade is prolly wat u want but u do u"};
    inline SillySetting<bool> noGlow{"No Glow", "Default Object Options", false, SettingReload::Editor};
    inline SillySetting<bool> useJSON{"Use JSON", "Default Object Options", false, SettingReload::Editor, "json gets parsed on editorlayer init ! most ppl will prolly find this useful enough for auto enabling noglow/dontfade but if you want more advanced settings like obj str f***ery u can use json :333"};
    inline SillySetting<std::string> path{"JSON Path", "Default Object Options", "entries.json", "relative to config folder (mainly just a convenient way to show the file name u need)"};
}