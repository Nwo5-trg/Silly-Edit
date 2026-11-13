#pragma once

#include <Geode/modify/EditorUI.hpp>
#include <feature/include.hpp>
#include "utils.hpp"

// fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows 
namespace FF {
    class $feature(FloodFill) {
        void onEditor() override;
        void onToggled(bool enabled) override;
    } feature;

    class $feature_modify(EditorUI) {
        void rectFill(cocos2d::CCArray* objs);
        void createFromRects(const std::vector<FF::Rect>& rects, cocos2d::CCArray* boundry, GameObject* base, bool dontSelectCenter);
        void quickFill();
    };

    class $setting_category("flood-fill-logo.png"_spr, "Im not quite sure how i can describe this features, it fills rects or smth rawr");

    inline Setting<bool> notifications{
        "Notifications", feature, true
    };
    inline Setting<bool> selectFill{
        "Select Fill", feature, true, "select filled objects after filling"
    };
    inline Setting<bool> selectBoundry{
        "Select Boundry", feature, false, "select flood fill boundry after filling"
    };
    inline Setting<bool> quickFillButton{
        "Quick Fill Button", feature, true, SettingReload::Editor
    };
}