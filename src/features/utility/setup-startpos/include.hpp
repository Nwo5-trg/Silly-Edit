#pragma once

#include <Geode/modify/LevelEditorLayer.hpp>
#include <feature/include.hpp>

namespace SetupStartpos {
    class $feature(SetupStartpos) {} feature;

    class $feature_modify(LevelEditorLayer) {
        GameObject* createObject(int objectID, cocos2d::CCPoint position, bool noUndo);
        void setupStartpos(StartPosObject* startpos);
    };

    class $setting_category("setup-startpos-logo.png"_spr, "Configure startpos settings on place");

    inline Setting<bool> gamemode{
        "Gamemode", feature, true
    };
    inline Setting<bool> freeMode{
        "Free Mode", feature, true, "if clean startpos is installed"
    };
    inline Setting<bool> speed{
        "Speed", feature, true
    };
    inline Setting<bool> mini{
        "Mini", feature, true
    };
    inline Setting<bool> mirror{
        "Mirror", feature, true
    };
    inline Setting<bool> dual{
        "Dual", feature, false
    };
    inline Setting<bool> gravity{
        "Gravity", feature, true, "tries to calculate gravity based on orbs and portals and stuff (will b completely broken for memory levels mind u)"
    };
}