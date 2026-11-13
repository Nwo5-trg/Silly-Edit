#include <Geode/modify/SetupTriggerPopup.hpp>
#include <feature/include.hpp>
#include "utils.hpp"

namespace EasingPreview {
    class $feature(EasingPreview) {} feature;

    class $feature_modify(SetupTriggerPopup) {
        void onClose(cocos2d::CCObject* sender);
        void createEasingControls(cocos2d::CCPoint position, float scale, int page, int group);
        void createCustomEasingControls(gd::string text, cocos2d::CCPoint position, float scale, int typeProperty, int rateProperty, int page, int group);
    };

    class $setting_category("easing-preview-logo.png"_spr, "Click easing label");

    inline Setting<bool> graphByDefault{
        "Graph By Default", feature, false, "show graph view by default"
    };
    inline Setting<float> animationDuration{
        "Animation Duration", feature, 1.5f, {0.1f, 10.0f}
    };
    inline Setting<float> animationResetTime{
        "Animation Reset Time", feature, 0.5f, {0.0f, 10.0f}
    };
    inline Setting<int> graphDetail{
        "Graph Detail", feature, 50, {5, 200}
    };
    inline Setting<float> graphThickness{
        "Graph Thickess", feature, 0.75f, {0.1f, 10.0f}
    };
    inline Setting<bool> chroma{
        "Chroma", feature, false
    };
}