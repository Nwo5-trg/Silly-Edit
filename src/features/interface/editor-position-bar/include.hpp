#pragma once

#include <Geode/modify/EditorUI.hpp>
#include <feature/include.hpp>
#include "utils.hpp"

namespace EditorPositionBar {
    class $feature(EditorPositionBar) {
        void setup() override {
            m_data.condition = SettingCondition::DesktopOnly;
        }

        void onUpdate() override;
        void onToggled(bool pEnabled) override;
        void onSettingChanged(std::string pName, GenericSetting*) override;
    } feature;

    static constexpr float BAR_GAP = 5.0f;

    class $feature_modify(EditorUI) {
        struct Fields {
            std::optional<std::vector<cocos2d::CCPoint>> lastSavedPositions = std::nullopt;
            cocos2d::CCMenu* positionBarMenu = nullptr;
        };

        void showPositonText(int pIndex);
        void moveEditorToSavedPos(cocos2d::CCPoint pPos);

        void keyDown(cocos2d::enumKeyCodes key, double timestamp);
    };

    class $setting_category("editor-position-bar-logo.png"_spr, "Show thumbnails of editor positions but also some other cool things");

    inline SillySetting<bool> showBar{
        "Show Bar", feature, true
    };
    inline SillySetting<float> scale{
        "Scale", feature, 1.0f, {0.0f, std::nullopt}
    };
    inline SillySetting<float> offset{
        "Offset", feature, -7.5f
    };
    inline SillySetting<bool> showGoToText{
        "Show Go To Text", feature, true
    };
    inline SillySetting<float> snapshotSize{
        "Snapshot Size", feature, 512.0f, {50.0f, 10000.0f}, "30 = one block"
    };
    inline SillySetting<sillyedit::settings::Modifier> createPositionModifier{
        "Create Position Modifier", feature, "Alt", sillyedit::settings::modifierSettingOptions()
    };
    inline SillySetting<sillyedit::settings::Modifier> goToPositionModifier{
        "Go To Position Modifier", feature, "Ctrl", sillyedit::settings::modifierSettingOptions()
    };
}