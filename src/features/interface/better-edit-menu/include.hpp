#pragma once

#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/EditButtonBar.hpp>
#include <feature/include.hpp>
#include "utils.hpp"

namespace BetterEditMenu {
    class $feature(BetterEditMenu) {
        void onToggled(bool pEnabled) override;
        void onUIUpdated(float pScale) override;
    } feature;

    class $feature_modify(EditorUI) {
        struct Fields {
            EditMenu* editMenu = nullptr;
        };
    };
    
    class $feature_modify(EditButtonBar) {
        static void onModify(auto& pSelf) {
            (void)pSelf.setHookPriorityPre("EditButtonBar::loadFromItems", geode::Priority::Early);
        }
        void loadFromItems(cocos2d::CCArray* objects, int rows, int columns, bool keepPage);
    };

    class $setting_category("better-edit-menu-logo.png"_spr, "My impl of an overhauled edit tab menu :3");
}