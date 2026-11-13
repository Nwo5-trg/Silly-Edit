#pragma once

#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/EditButtonBar.hpp>
#include <feature/include.hpp>
#include <utils/include.hpp>
#include "utils.hpp"

namespace BetterEditMenu {
    class $feature(BetterEditMenu) {
        void onEditor() override;
        void onUpdate() override;
        void onToggled(bool enabled) override;
        void onUIUpdated(float) override;
        void onSettingChanged(std::string name, GenericSetting*) override;
    } feature;

    class $feature_modify(EditorUI) {
        struct Fields {
            EditMenu* editMenu = nullptr;
        };

        static void onModify(auto& self) {
            (void)self.setHookPriorityAfterPost("EditorUI::init", BETTER_EDIT_ID);
        }

        bool init(LevelEditorLayer* editorLayer);
    };
    
    class $feature_modify(EditButtonBar) {
        static void onModify(auto& self) {
            (void)self.setHookPriorityPre("EditButtonBar::loadFromItems", geode::Priority::Early);
        }
        
        void loadFromItems(cocos2d::CCArray* objects, int rows, int columns, bool keepPage);
    };

    class $setting_category("better-edit-menu-logo.png"_spr, "My impl of an overhauled edit tab menu :3");

    inline Setting<float> menuGap{
        "Menu Gap", feature, 5.0f
    };
    inline Setting<bool> hideArrowsIfUnused{
        "Hide Arrows If Unused", feature, false, "if both arrows are disabled, then hide them"
    };
    inline Setting<bool> noDefaultMoveButtons{
        "No Default Move Buttons", feature, true
    };
    inline Setting<bool> noDefaultRotateButtons{
        "No Default Rotate Buttons", feature, true
    };
    inline Setting<bool> noDefaultFlipButtons{
        "No Default Flip Buttons", feature, true
    };
    inline Setting<float> moveInputDefault{
        "Move Input Default", feature, 30, {0, std::nullopt}
    };
    inline Setting<std::string> moveShortcut1{
        "Move Shortcut 1", feature, "1;1/30", "string is formatted as \"VALUE;OPTIONALLABEL\" if no optional label is provided it will just be set to the value"
    };
    inline Setting<std::string> moveShortcut2{
        "Move Shortcut 2", feature, "3;1/10", "string is formatted as \"VALUE;OPTIONALLABEL\" if no optional label is provided it will just be set to the value"
    };
    inline Setting<std::string> moveShortcut3{
        "Move Shortcut 3", feature, "7.5;1/4", "string is formatted as \"VALUE;OPTIONALLABEL\" if no optional label is provided it will just be set to the value"
    };
    inline Setting<std::string> moveShortcut4{
        "Move Shortcut 4", feature, "15;1/2", "string is formatted as \"VALUE;OPTIONALLABEL\" if no optional label is provided it will just be set to the value"
    };
    inline Setting<std::string> moveShortcut5{
        "Move Shortcut 5", feature, "30;1", "string is formatted as \"VALUE;OPTIONALLABEL\" if no optional label is provided it will just be set to the value"
    };
    inline Setting<std::string> moveShortcut6{
        "Move Shortcut 6", feature, "150;2", "string is formatted as \"VALUE;OPTIONALLABEL\" if no optional label is provided it will just be set to the value"
    };
    inline Setting<float> rotationInputDefault{
        "Rotation Input Default", feature, 90, {0, std::nullopt}
    };
    inline Setting<std::string> rotateShortcut1{
        "Rotate Shortcut 1", feature, "26.56505118;26.5", "string is formatted as \"VALUE;OPTIONALLABEL\" if no optional label is provided it will just be set to the value"
    };
    inline Setting<std::string> rotateShortcut2{
        "Rotate Shortcut 2", feature, "45", "string is formatted as \"VALUE;OPTIONALLABEL\" if no optional label is provided it will just be set to the value"
    };
    inline Setting<std::string> rotateShortcut3{
        "Rotate Shortcut 3", feature, "90", "string is formatted as \"VALUE;OPTIONALLABEL\" if no optional label is provided it will just be set to the value"
    };
}