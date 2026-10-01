#pragma once

#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <feature/include.hpp>

namespace ZoomInput {
    class $feature(ZoomInput) {
        void onEditor() override;
        void onToggled(bool pEnabled) override;
        void onUIUpdated(float pScale) override;
    } feature;

    inline constexpr cocos2d::CCSize BASE_ZOOM_INPUT_SIZE = {20.0f, 10.0f};

    class $feature_modify(EditorUI) {
        struct Fields {
            cocos2d::CCMenu* zoomContainer = nullptr;
            geode::TextInput* zoomInput = nullptr;
        };

        void updateZoomInput(float pZoom);
        void updateZoomContainer();
        void onZoomInputButton(cocos2d::CCObject*);

        void updateZoom(float zoom);
        void constrainGameLayerPosition(float x, float y);
    };

    class $feature_modify(LevelEditorLayer) {
        bool init(GJGameLevel* level, bool noUI);
    };

    class $setting_category("zoom-input-logo.png"_spr, "Little input under editor slider that shows you current zoom and lets you type it in");

    inline SillySetting<int> rounding{
        "Rounding", feature, 3, {0, 7}
    };
    inline SillySetting<float> zoomInputScale{
        "Zoom Input Scale", feature, 1.0f, {0.0f, std::nullopt}, SettingReload::Editor
    };
    inline SillySetting<float> zoomInputOffset{
        "Zoom Input Offset", feature, -17.5f, SettingReload::Editor
    };
    inline SillySetting<bool> centered{
        "Centered", feature, false, SettingReload::Editor, "will position at center of the screen instead of under position slider"
    };
    inline SillySetting<bool> noConstrainPosition{
        "No Constrain Position", feature, false, "use at ur own risk idk if this is a good hacky workaround uwu, works regardless of zook input being enabled"
    };
}