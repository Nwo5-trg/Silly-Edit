#pragma once

#include <Geode/modify/EditorUI.hpp>
#include <feature/include.hpp>
#include "utils.hpp"

// fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows 
namespace FF {
    class $feature(FloodFill) {
        void onEditor() override;
        void onUpdate() override;
    } feature;

    constexpr float MINIMUM_SPECIAL_MOUSE_DISTANCE = 5.0f;

    class $feature_modify(EditorUI) {
        struct Fields {
            bool specialHold = false;
            cocos2d::CCPoint specialStart = cocos2d::CCPointZero;
        };

        void rectFill(cocos2d::CCArray* pObjs);
        void createFromRects(const std::vector<FF::Rect>& pRects, cocos2d::CCArray* pBoundry, GameObject* pBase);
        void quickFill();

        void keyDown(cocos2d::enumKeyCodes key, double timestamp);
    };

    class $setting_category("flood-fill-logo.png"_spr, "Im not quite sure how i can describe this features, it fills rects or smth rawr");

    inline SillySetting<bool> notifications{
        "Notifications", feature, true
    };
    inline SillySetting<bool> selectFill{
        "Select Fill", feature, true, "select filled objects after filling"
    };
    inline SillySetting<bool> selectBoundry{
        "Select Boundry", feature, false, "select flood fill boundry after filling"
    };
    inline SillySetting<bool> quickFillButton{
        "Quick Fill\nButton", feature, true, SettingReload::Editor
    };
    inline SillySetting<bool> selectSpecialFill{
        "Select Special\nFill", feature, true, SettingCondition::DesktopOnly, "select filled objects after rect filling with special key"
    };
    inline SillySetting<bool> specialAsButton{
        "Special As\nButton", feature, false, SettingCondition::DesktopOnly, "use special key as a shortcut to quick fill button instead of its own functionality"
    };
    inline SillySetting<float> specialPreviewThickness{
        "Special Preview\nThickness", feature, 1.0f, {0.0f, std::nullopt}
    };
    inline SillySetting<bool> scaleWithZoom{
        "Scale With\nZoom", feature, false
    };
    inline SillySetting<cocos2d::ccColor3B> specialPreviewColor{
        "Special Preview\nColor", feature, { 255, 50, 200 }, SettingCondition::DesktopOnly
    };
    inline SillySetting<float> specialPreviewFill{
        "Special Preview\nFill", feature, 0.0f, {0.0f, 1.0f}, SettingCondition::DesktopOnly
    };
    inline SillySetting<bool> chroma{
        "Chroma", feature, false
    };
}