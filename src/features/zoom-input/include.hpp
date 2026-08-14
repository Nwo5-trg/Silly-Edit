#pragma once

#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <feature/include.hpp>

namespace ZoomInput {
    class $feature(ZoomInput) {
        void onEditor() override;
        void onUIUpdated(float pScale) override;
    } feature;

    class $feature_modify(EditorUI) {
        struct Fields {
            cocos2d::CCMenu* zoomContainer = nullptr;
            geode::TextInput* zoomInput = nullptr;
        };

        void updateZoomInput(float pZoom);
        void updateZoomContainer(float pScale);
        void onZoomInputButton(cocos2d::CCObject*);

        void updateZoom(float zoom);
        void constrainGameLayerPosition(float x, float y);
    };

    class $feature_modify(LevelEditorLayer) {
        bool init(GJGameLevel* level, bool noUI);
    };

    class $setting_category("zoom-input-logo.png"_spr);

    inline SillySetting<int> rounding{
        "Rounding", feature, 3, {0, 7}
    };
    inline SillySetting<float> zoomInputScale{
        "Zoom Input\nScale", feature, 1.0f, {0.0f, std::nullopt}, SettingReload::Editor
    };
    inline SillySetting<float> zoomInputOffset{
        "Zoom Input\nOffset", feature, -17.5f, SettingReload::Editor
    };
    inline SillySetting<bool> centered{
        "Centered", feature, false, SettingReload::Editor, "will position at center of the screen instead of under position slider"
    };
    inline SillySetting<bool> noConstrainPosition{
        "No Constrain\nPosition", feature, false, "use at ur own risk idk if this is a good hacky workaround uwu, works regardless of zook input being enabled"
    };
}