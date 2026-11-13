#pragma once

#include <Geode/modify/EditorUI.hpp>
#include <feature/include.hpp>
#include "utils.hpp"

namespace ContextMenu {
    class $feature(ContextMenu) {
        void setup() override {
            m_data.condition = SettingCondition::DesktopOnly;
        }

        void onEditor() override;
    } feature;

    class $feature_modify(EditorUI) {
        struct Fields {
            ContextMenuNode* menu = nullptr;
        };

        void keyDown(cocos2d::enumKeyCodes key, double timestamp);
        void moveObjectCall(EditCommand command);
        bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    };

    class $setting_category("context-menu-logo.png"_spr, "(WIP) Right click on object(s) for a configurable context menu :3");

    inline Setting<bool> selectOnClick{
        "Select On Click", feature, true, "when right clicking on an unselected object, select it instead of the static menu showing"
    };
    inline Setting<float> scaleMultiplier{
        "Scale Multiplier", feature, 1.0f
    };
    inline Setting<bool> scrollAway{
        "Scroll Away", feature, false, "instead of blocking scrolling when context menu is showing, close context menu when scroll is attempted"
    };
    inline Setting<sillyedit::settings::Modifier> specialModifier{
        "Special Modifier", feature, "Shift", sillyedit::settings::modifierSettingOptions(), "e.g. changes copy to copy special, scale to scale xy"
    };
    inline Setting<bool> editGroup{
        "Edit Group", feature, true
    };
    inline Setting<bool> editObject{
        "Edit Object", feature, true
    };
    inline Setting<bool> editExtras{
        "Edit Extras", feature, true
    };
    inline Setting<bool> editSpecial{
        "Edit Special", feature, true
    };
    inline Setting<bool> createMode{
        "Create Mode", feature, true
    };
    inline Setting<bool> editMode{
        "Edit Mode", feature, true
    };
    inline Setting<bool> deleteMode{
        "Delete Mode", feature, true
    };
    inline Setting<bool> flipX{
        "Flip X", feature, true
    };
    inline Setting<bool> flipY{
        "Flip Y", feature, true
    };
    inline Setting<bool> scale{
        "Scale", feature, true
    };
    inline Setting<bool> transform{
        "Transform", feature, true
    };
    inline Setting<bool> selectAll{
        "Select All", feature, true
    };
    inline Setting<bool> deselectAll{
        "Deselect All", feature, true
    };
    inline Setting<bool> deleteSetting{
        "Delete", feature, true
    };
    inline Setting<bool> copy{
        "Copy", feature, true
    };
    inline Setting<bool> paste{
        "Paste", feature, true
    };
    inline Setting<bool> pasteState{
        "Paste State", feature, true
    };
    inline Setting<bool> duplicate{
        "Duplicate", feature, true
    };
    inline Setting<bool> toggleInvisible{
        "Toggle Invisible", feature, true
    };
    inline Setting<bool> goToLayer{
        "Go To Layer", feature, true
    };
}