#pragma once

#include <Geode/modify/LevelEditorLayer.hpp>
#include <feature/include.hpp>

namespace SetupStartpos {
    class $feature(SetupStartpos) {} feature;

    class $feature_modify(LevelEditorLayer) {
        GameObject* createObject(int objectID, cocos2d::CCPoint position, bool noUndo);
        void setupStartpos(StartPosObject* pStartpos);
    };

    class $setting_category("setup-startpos-logo.png"_spr);

    inline SillySetting<bool> gamemode{
        "Gamemode", feature, true
    };
    inline SillySetting<bool> freeMode{
        "Free Mode", feature, true, "if clean startpos is installed"
    };
    inline SillySetting<bool> speed{
        "Speed", feature, true
    };
    inline SillySetting<bool> mini{
        "Mini", feature, true
    };
    inline SillySetting<bool> mirror{
        "Mirror", feature, true
    };
    inline SillySetting<bool> dual{
        "Dual", feature, false
    };
    inline SillySetting<bool> gravity{
        "Gravity", feature, true, "tries to calculate gravity based on orbs and portals and stuff (will b completely broken for memory levels mind u)"
    };
}