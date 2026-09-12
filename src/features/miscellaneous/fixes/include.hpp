#pragma once

#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>
#include <feature/include.hpp>

namespace Fixes {
    class $feature(Fixes) {} feature;

    class $feature_modify(EditorUI) {
        void selectObject(GameObject* object, bool ignoreFilter);
        void keyDown(cocos2d::enumKeyCodes key, double timestamp);
    };

    class $feature_modify(GJBaseGameLayer) {
        struct Fields {
            bool loading = false;
        };
        
        void loadUpToPosition(float position, int order, int channel);
        void processAreaEffects(gd::vector<EnterEffectInstance>* effects, GJAreaActionType type, float dt, bool visibleFrame);
    };

    class $feature_modify(PlayerObject) {
        void collidedWithSlopeInternal(float dt, GameObject* object, bool forced);
    };
    
    class $setting_category("fixes-logo.png"_spr, "Misc fixes to some aspects of the editor");

    inline SillySetting<bool> fixAreaCorruption{
        "Fix Area Corruption", feature, true, "area triggers can no longer fuck up ur saves"
    };
    inline SillySetting<bool> fixObjectInfoLabel{
        "Fix Object Info Label", feature, true, "sometimes the label doesnt always show/update, this fixes that"
    };
    inline SillySetting<bool> fixIgnoreDamageWave{
        "Fix Ignore Damage Wave", feature, true
    };
    inline SillySetting<bool> allowShiftChangeModes{
        "Allow Shift Change Modes", feature, true, "allow you to hold shift while clicking 1, 2, and 3 to change modes (i might b the only one who gets any use out of this but wtv)"
    };
}