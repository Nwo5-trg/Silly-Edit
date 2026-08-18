#pragma once

#include <Geode/modify/EditorUI.hpp>
#include <feature/include.hpp>

namespace HideUIToggle {
    class $feature(HideUIToggle, SettingCondition::None, SettingReload::Editor) {
        void onEditor() override;
    } feature;

    class $feature_modify(EditorUI) {
        struct Fields {
            CCMenuItemToggler* hideUIToggle = nullptr;
        };

        void onHideUI(CCObject* pSender);

        void showUI(bool show);
    };

    class $setting_category("hide-ui-toggle-logo.png"_spr, "Button to show/hide editor ui (practically ripped from betteredit)");
}