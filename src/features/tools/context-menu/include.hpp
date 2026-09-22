#pragma once

#include <Geode/modify/EditorUI.hpp>
#include <feature/include.hpp>
#include "utils.hpp"

namespace ContextMenu {
    class $feature(ContextMenu, SettingCondition::DesktopOnly) {
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

    inline SillySetting<bool> selectOnClick{
        "Select On Click", feature, true, "when right clicking on an unselected object, select it instead of the static menu showing"
    };
    inline SillySetting<float> scaleMultiplier{
        "Scale Multiplier", feature, 1.0f
    };
    inline SillySetting<bool> scrollAway{
        "Scroll Away", feature, false, "instead of blocking scrolling when context menu is showing, close context menu when scroll is attempted"
    };
    inline SillySetting<sillyedit::settings::Modifier> specialModifier{
        "Special Modifier", feature, "Shift", sillyedit::settings::modifierSettingOptions(), "e.g. changes copy to copy special, scale to scale xy"
    };
    inline SillySetting<bool> editGroup{
        "Edit Group", feature, true
    };
    inline SillySetting<bool> editObject{
        "Edit Object", feature, true
    };
    inline SillySetting<bool> editExtras{
        "Edit Extras", feature, true
    };
    inline SillySetting<bool> editSpecial{
        "Edit Special", feature, true
    };
    inline SillySetting<bool> createMode{
        "Create Mode", feature, true
    };
    inline SillySetting<bool> editMode{
        "Edit Mode", feature, true
    };
    inline SillySetting<bool> deleteMode{
        "Delete Mode", feature, true
    };
    inline SillySetting<bool> flipX{
        "Flip X", feature, true
    };
    inline SillySetting<bool> flipY{
        "Flip Y", feature, true
    };
    inline SillySetting<bool> scale{
        "Scale", feature, true
    };
    inline SillySetting<bool> transform{
        "Transform", feature, true
    };
    inline SillySetting<bool> selectAll{
        "Select All", feature, true
    };
    inline SillySetting<bool> deselectAll{
        "Deselect All", feature, true
    };
    inline SillySetting<bool> deleteSetting{
        "Delete", feature, true
    };
    inline SillySetting<bool> copy{
        "Copy", feature, true
    };
    inline SillySetting<bool> paste{
        "Paste", feature, true
    };
    inline SillySetting<bool> pasteState{
        "Paste State", feature, true
    };
    inline SillySetting<bool> duplicate{
        "Duplicate", feature, true
    };
    inline SillySetting<bool> toggleInvisible{
        "Toggle Invisible", feature, true
    };
    inline SillySetting<bool> goToLayer{
        "Go To Layer", feature, true
    };
}