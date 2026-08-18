#pragma once

#include <Geode/modify/LevelOptionsLayer.hpp>
#include <Geode/modify/GameLevelManager.hpp>
#include <feature/include.hpp>
#include <utils/include.hpp>

namespace Template {
    class $feature(Template) {} feature;

    class $feature_modify(LevelOptionsLayer) {
        void setupOptions();
    };

    class $feature_modify(GameLevelManager) {
        GJGameLevel* createNewLevel();
    };

    class $setting_category("template-logo.png"_spr, "Button in level options to save the current level as a \"template\", all newly created levels will then copy the saved template");
}