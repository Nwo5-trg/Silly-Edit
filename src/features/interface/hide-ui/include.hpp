#pragma once

#include <Geode/modify/EditorUI.hpp>
#include <feature/include.hpp>
#include <utils/include.hpp>

namespace HideUI {
    class $feature(HideUI) {
        void setup() override {
            m_data.reload = SettingReload::Editor;
            m_data.incompatMods = TINKER_EDIT_ID;
            m_data.incompatMessage = "overlapping functionality with tinker";
        }

        void onEditor() override;
    } feature;

    class $feature_modify(EditorUI) {
        struct Fields {
            CCMenuItemToggler* hideUIToggle = nullptr;
        };

        void onHideUI(CCObject* pSender);

        void showUI(bool show);
        void onPause(CCObject* sender);
    };

    class $setting_category("hide-ui-logo.png"_spr, "Button to show/hide editor ui (practically ripped from betteredit)");
}