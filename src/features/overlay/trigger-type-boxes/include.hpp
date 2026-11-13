#pragma once

#include <Geode/modify/LevelEditorLayer.hpp>
#include <feature/include.hpp>

namespace TriggerTypeBoxes {
    class $feature(TriggerTypeBoxes) {} feature;

    class $feature_modify(LevelEditorLayer) {
        void drawTriggerTypeBoxes(GameObject* obj);
        
        void updateDebugDraw();
    };

    class $setting_category("trigger-type-boxes-logo.png"_spr, "Customisable touch and spawn indicators");

    inline Setting<bool> touchTriggerIndicators{
        "Touch Trigger Indicators", feature, true
    };
    inline Setting<cocos2d::ccColor3B> touchTriggerColor{
        "Touch Trigger Color", feature, {0, 255, 255}
    };
    inline Setting<int> touchTriggerFill{
        "Touch Trigger Fill", feature, 0, {0, 255}
    };
    inline Setting<float> touchTriggerSize{
        "Touch Trigger Size", feature, 30.0f, {0.1f, std::nullopt}
    };
    inline Setting<bool> spawnTriggerIndicators{
        "Spawn Trigger Indicators", feature, true
    };
    inline Setting<cocos2d::ccColor3B> spawnTriggerColor{
        "Spawn Trigger Color", feature, {0, 255, 255}
    };
    inline Setting<int> spawnTriggerFill{
        "Spawn Trigger Fill", feature, 0, {0, 255}
    };
    inline Setting<float> spawnTriggerSize{
        "Spawn Trigger Size", feature, 25.0f, {0.1f, std::nullopt}
    };
    inline Setting<bool> multiTriggerIndicators{
        "Mutli Trigger Indicators", feature, true
    };
    inline Setting<float> multiTriggerDistance{
        "Mutli Trigger Distance", feature, 10.0f
    };
    inline Setting<int> multiTriggerOpacity{
        "Mutli Trigger Opacity", feature, 200, {0, 255}
    };
    inline Setting<float> thickness{
        "Thickness", feature, 0.5, {0.1f, std::nullopt}
    };
    inline Setting<bool> scaleWithZoom{
        "Scale With Zoom", feature, true
    };
    inline Setting<bool> drawInfront{
        "Draw Infront", feature, true
    };
    inline Setting<bool> offsetTouchTrigger{
        "Offset Touch Trigger", feature, false, "offset touch trigger box to be centered around the body of the trigger"
    };
    inline Setting<bool> chroma{
        "Chroma", feature, false
    };
}