#pragma once

#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <feature/include.hpp>

namespace NAME {
    class $feature(NAME) {
        void onEditor() override;
    } feature;

    class $feature_modify(EditorUI) {

    };

    class $feature_modify(LevelEditorLayer) {

    };

    class $setting_category("NAME-logo.png"_spr, "");

    inline SillySetting<bool> rawr{
        "", feature, true
    };
}