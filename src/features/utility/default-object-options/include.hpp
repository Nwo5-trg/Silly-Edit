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
            ObjectOptions options;
        };

        static void onModify(auto& pSelf) {
            (void)pSelf.setHookPriorityPost("LevelEditorLayer::createObject", geode::Priority::Replace);
        }

        GameObject* createObject(int key, cocos2d::CCPoint position, bool noUndo);
    };

    class $setting_category("default-object-options-logo.png"_spr, "Configure objects on place");
    
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
        "Use JSON", feature, false, "for more complex configurations u can use a json file, for an example check examples/default-object-options.jsonc in the github repo"
    };
    inline SillySetting<std::string> path{
        "JSON Path", feature, "object-options.json", "relative to config folder (mainly just a convenient way to show the file name u need)"
    };
}