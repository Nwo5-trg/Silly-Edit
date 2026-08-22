#pragma once

#include <Geode/modify/EditorUI.hpp>
#include <feature/include.hpp>

namespace SelectionUtils {
    class $feature(SelectionUtils) {
        void onEditor() override;
        void onUpdate() override;
    } feature;

    class $feature_modify(EditorUI) {
        struct Fields {
            bool prollySnapping = false;
            cocos2d::CCPoint correctLastTouchPos;
        };

        GameObject* getSnapObject();
        cocos2d::CCPoint getSnappedPos(GameObject* pObj);
        void snapSelection(GameObject* pSnapObj);

        static void onModify(auto& pSelf);
        
        void draw();
        bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
        void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
        void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    };

    class $setting_category("selection-utils-logo.png"_spr, "A bunch of utils related to selecting objects (wow its almost like thats the name)");

    inline SillySetting<float> gridSize{
        "Grid Size", feature, 15.0f, {1.0f, std::nullopt}
    };
    inline SillySetting<cocos2d::ccColor3B> snapObjectColor{
        "Snap Object\nColor", feature, cocos2d::ccc3(255, 127, 0), "color snap object will be changed to" 
    };
    inline SillySetting<bool> snapIndicator{
        "Snap Indicator", feature, true, "previews where the snap object will snap to"
    };
    inline SillySetting<float> snapIndicatorFill{
        "Snap Indicator\nFill", feature, 0.25f, {0.0f, 1.0f}, "opacity of snap indicator fill"
    };
    inline SillySetting<float> snapIndicatorThickness{
        "Snap Indicator\nThickness", feature, 2.5f, {0.0f, std::nullopt}
    };
    inline SillySetting<cocos2d::ccColor3B> selectedObjectColor{
        "Selected Object\nColor", feature, cocos2d::ccc3(0, 255, 0), "color selected objects (and selection rect) will be changed to"
    };
    inline SillySetting<float> selectionRectThickness{
        "Selection Rect\nThickness", feature, 1.0f, {0.1, std::nullopt} 
    };
    inline SillySetting<int> selectionRectFill{
        "Selection Rect\nFill", feature, 0, {0, 255}, "selection rect fill opacity, 0-255"
    };
    inline SillySetting<bool> alwaysSingleSelect{
        "Always Single\nSelect", feature, false, "HACKY ! clicking on a selected object with multiple objects selected, will deselect all but that object"
    };
    inline SillySetting<bool> clickEmptyToDeselect{
        "Click Empty\nTo Deselect", feature, false, "HACKY ! clicking on blank space in editor deselects current selection"
    };
    inline SillySetting<bool> chroma{
        "Chroma", feature, false
    };
}