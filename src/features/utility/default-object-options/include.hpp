#pragma once

#include <Geode/modify/LevelEditorLayer.hpp>
#include <feature/include.hpp>
#include "utils.hpp"

namespace DefaultObjectOptions {
    class $feature(DefaultObjectOptions) {
        void onEditor() override;
        void onSettingChanged(std::string pName, GenericSetting*) override;
    } feature;

    class $feature_modify(LevelEditorLayer) {
        struct Fields {
            DefaultObjectOptions::ObjectOptions options;
        };

        static void onModify(auto& pSelf);

        GameObject* createObject(int key, cocos2d::CCPoint position, bool noUndo);
    };

    class $setting_category("default-object-options-logo.png"_spr);
    
    inline SillySetting<bool> dontFade{
        "Dont Fade", feature, false
    };
    inline SillySetting<bool> dontEnter{
        "Dont Enter", feature, false, "dont recommend turning this one on - dont fade is prolly wat u want but u do u"
    };
    inline SillySetting<bool> noGlow{
        "No Glow", feature, false
    };
    inline SillySetting<bool> useJSON{
        "Use JSON", feature, false, "most ppl will prolly find this useful enough for auto enabling noglow/dontfade but if you want more advanced settings like obj str fuckery u can use json :333"
    };
    inline SillySetting<std::string> path{
        "JSON Path", feature, "entries.json", "relative to config folder (mainly just a convenient way to show the file name u need)"
    };
}