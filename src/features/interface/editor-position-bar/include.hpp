#pragma once

#include <Geode/modify/EditorUI.hpp>
#include <feature/include.hpp>
#include <utils/include.hpp>
#include "utils.hpp"

namespace EditorPositionBar {
    class $feature(EditorPositionBar) {
        void setup() override {
            m_data.condition = SettingCondition::DesktopOnly;
            m_data.incompatMods = TINKER_EDIT_ID;
            m_data.incompatMessage = "might overlap with tinker grid controls at low zoom levels";
        }

        void onUpdate() override;
        void onToggled(bool enabled) override;
        void onSettingChanged(std::string name, GenericSetting*) override;
    } feature;

    static constexpr float BAR_GAP = 5.0f;

    class $feature_modify(EditorUI) {
        struct Fields {
            std::optional<std::vector<cocos2d::CCPoint>> lastSavedPositions = std::nullopt;
            cocos2d::CCMenu* positionBarMenu = nullptr;
        };

        void showPositonText(int index);
        void moveEditorToSavedPos(cocos2d::CCPoint pos);

        void keyDown(cocos2d::enumKeyCodes key, double timestamp);
    };

    class $setting_category("editor-position-bar-logo.png"_spr, "Show thumbnails of editor positions but also some other cool things");

    inline Setting<bool> showBar{
        "Show Bar", feature, true
    };
    inline Setting<float> scale{
        "Scale", feature, 1.0f, {0.0f, std::nullopt}
    };
    inline Setting<float> offset{
        "Offset", feature, -7.5f
    };
    inline Setting<bool> showGoToText{
        "Show Go To Text", feature, true
    };
    inline Setting<float> snapshotSize{
        "Snapshot Size", feature, 512.0f, {50.0f, 10000.0f}, "30 = one block"
    };
    inline Setting<sillyedit::settings::Modifier> createPositionModifier{
        "Create Position Modifier", feature, "Alt", sillyedit::settings::modifierSettingOptions()
    };
    inline Setting<sillyedit::settings::Modifier> goToPositionModifier{
        "Go To Position Modifier", feature, "Ctrl", sillyedit::settings::modifierSettingOptions()
    };
}