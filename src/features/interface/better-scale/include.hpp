#pragma once

#include <Geode/modify/GJScaleControl.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <feature/include.hpp>
#include <utils/include.hpp>

namespace BetterScale {
    class $feature(BetterScale, SettingCondition::None, SettingReload::Editor) {
        void onEditor() override;
    } feature;

    class $feature_modify(GJScaleControl) {
        struct Fields {
            std::vector<cocos2d::CCNode*> scaleNodes;
            std::vector<cocos2d::CCNode*> scaleXYNodes;

            geode::Label* newScaleLabel = nullptr;
            geode::TextInput* scaleInput = nullptr;
            geode::Label* newScaleXLabel = nullptr;
            geode::TextInput* scaleXInput = nullptr;
            geode::Label* newScaleYLabel = nullptr;
            geode::TextInput* scaleYInput = nullptr;

            std::vector<float> shortcuts;
            cocos2d::CCMenu* shortcutsMenu = nullptr;
            cocos2d::CCMenu* shortcutsXMenu = nullptr;
            cocos2d::CCMenu* shortcutsYMenu = nullptr;

            cocos2d::CCMenu* extrasMenu = nullptr;

            bool betterScaleLoaded = false;
        };

        static constexpr cocos2d::CCSize INPUT_SIZE = {45.0f, 20.0f};
        static constexpr float LABEL_SCALE = 0.75f;

        static constexpr float SHORTCUT_SIZE = 20.0f;
        static constexpr float SHORTCUT_SPACE = 30.0f;
        static constexpr float SHORTCUT_GAP = 5.0f;

        static constexpr float EXTRAS_BUTTON_SIZE = 30.0f;
        static constexpr float EXTRAS_GAP = 5.0f;

        static constexpr float DEFAULT_SLIDER_Y_HEIGHT = 60.0f;
        static constexpr float DEFAULT_LABEL_Y_HEIGHT = 90.0f;

        static constexpr float DEFAULT_LOCK_HEIGHT = 60.0f;
        static constexpr float DEFAULT_LOCK_XY_HEIGHT = 120.0f;

        static void onModify(auto& pSelf) {
            (void)pSelf.setHookPriorityAfterPost("GJScaleControl::init", TINKER_EDIT_ID);
        }

        void customScale(float pScale, ObjectScaleType pType);
        void updateShortcuts();
        void updateInputValues();
        void updateCustomNodes();
        void onScaleShortcut(cocos2d::CCObject* pSender);
        void onSwitchMode(cocos2d::CCObject* pSender);

        bool init();
        void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    };

    class $feature_modify(EditorUI) {
        void activateScaleControl(cocos2d::CCObject* sender);
        void updateScaleControl();
    };

    class $setting_category("better-scale-logo.png"_spr, "Very opinionated scale input with *colors*");

    inline SillySetting<bool> scaleShortcuts{
        "Scale Shortcuts", feature, true, SettingReload::Editor
    };
    inline SillySetting<std::string> shortcutsString{
        "Shortucts String", feature, std::string{"0.25,0.5,1,2,4"}, SettingReload::Editor, "seperate scales with commas"
    };
    inline SillySetting<bool> allowNegative{
        "Allow Negative", feature, false, "dont use abs for negative scales"
    };
    inline SillySetting<bool> newLockTexture{
        "New Lock Texture", feature, true, SettingReload::Editor
    };
    inline SillySetting<bool> switchModeButton{
        "Switch Mode Button", feature, true, SettingReload::Editor
    };
    inline SillySetting<float> controlSize{
        "Control Size", feature, 0.75f, {0.1f, std::nullopt}, "multiplies scale control size"
    };
    inline SillySetting<bool> lockControlSize{
        "Lock Control Size", feature, false, "treats control size as a static scale that doesnt change with zoom"
    };
    inline SillySetting<float> controlOffset{
        "Control Offset", feature, 40.0f, "y offset from default control position"
    };
}