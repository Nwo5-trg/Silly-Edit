#pragma once

#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/EditButtonBar.hpp>
#include <feature/include.hpp>
#include "utils.hpp"

namespace BetterEditMenu {
    class $feature(BetterEditMenu) {
        void onEditor() override;
        void onUpdate() override;
        void onToggled(bool pEnabled) override;
        void onUIUpdated(float) override;
        void onSettingChanged(std::string pName, GenericSetting*) override;
    } feature;

    class $feature_modify(EditorUI) {
        struct Fields {
            EditMenu* editMenu = nullptr;
        };

        void updateEditButtonColor(int tag, cocos2d::ccColor3B color);
        void toggleSpecialEditButtons();
    };
    
    class $feature_modify(EditButtonBar) {
        static void onModify(auto& pSelf) {
            (void)pSelf.setHookPriorityPre("EditButtonBar::loadFromItems", geode::Priority::Early);
        }
        
        void loadFromItems(cocos2d::CCArray* objects, int rows, int columns, bool keepPage);
    };

    class $setting_category("better-edit-menu-logo.png"_spr, "My impl of an overhauled edit tab menu :3");

    inline SillySetting<float> menuGap{
        "Menu Gap", feature, 5.0f
    };
    inline SillySetting<bool> hideArrowsIfUnused{
        "Hide Arrows If Unused", feature, false, "if both arrows are disabled, then hide them"
    };
    inline SillySetting<bool> noDefaultMoveButtons{
        "No Default Move Buttons", feature, true
    };
    inline SillySetting<bool> noDefaultRotateButtons{
        "No Default Rotate Buttons", feature, true
    };
    inline SillySetting<bool> noDefaultFlipButtons{
        "No Default Flip Buttons", feature, true
    };
    inline SillySetting<float> moveInputDefault{
        "Move Input Default", feature, 30, {0, std::nullopt}
    };
    inline SillySetting<std::string> moveShortcut1{
        "Move Shortcut 1", feature, "1;1/30", "string is formatted as \"VALUE;OPTIONALLABEL\" if no optional label is provided it will just be set to the value"
    };
    inline SillySetting<std::string> moveShortcut2{
        "Move Shortcut 2", feature, "3;1/10", "string is formatted as \"VALUE;OPTIONALLABEL\" if no optional label is provided it will just be set to the value"
    };
    inline SillySetting<std::string> moveShortcut3{
        "Move Shortcut 3", feature, "7.5;1/4", "string is formatted as \"VALUE;OPTIONALLABEL\" if no optional label is provided it will just be set to the value"
    };
    inline SillySetting<std::string> moveShortcut4{
        "Move Shortcut 4", feature, "15;1/2", "string is formatted as \"VALUE;OPTIONALLABEL\" if no optional label is provided it will just be set to the value"
    };
    inline SillySetting<std::string> moveShortcut5{
        "Move Shortcut 5", feature, "30;1", "string is formatted as \"VALUE;OPTIONALLABEL\" if no optional label is provided it will just be set to the value"
    };
    inline SillySetting<std::string> moveShortcut6{
        "Move Shortcut 6", feature, "150;2", "string is formatted as \"VALUE;OPTIONALLABEL\" if no optional label is provided it will just be set to the value"
    };
    inline SillySetting<float> rotationInputDefault{
        "Rotation Input Default", feature, 90, {0, std::nullopt}
    };
    inline SillySetting<std::string> rotateShortcut1{
        "Rotate Shortcut 1", feature, "26.56505118;26.5", "string is formatted as \"VALUE;OPTIONALLABEL\" if no optional label is provided it will just be set to the value"
    };
    inline SillySetting<std::string> rotateShortcut2{
        "Rotate Shortcut 2", feature, "45", "string is formatted as \"VALUE;OPTIONALLABEL\" if no optional label is provided it will just be set to the value"
    };
    inline SillySetting<std::string> rotateShortcut3{
        "Rotate Shortcut 3", feature, "90", "string is formatted as \"VALUE;OPTIONALLABEL\" if no optional label is provided it will just be set to the value"
    };
}