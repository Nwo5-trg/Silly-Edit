#pragma once

#include <Geode/modify/EditorUI.hpp>
#include <feature/include.hpp>
#include "utils.hpp"

// fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows fuck windows 
namespace FF {
    class $feature(FloodFill) {
        void onEditor() override;
        void onToggled(bool pEnabled) override;
    } feature;

    class $feature_modify(EditorUI) {
        void rectFill(cocos2d::CCArray* pObjs);
        void createFromRects(const std::vector<FF::Rect>& pRects, cocos2d::CCArray* pBoundry, GameObject* pBase, bool pDontSelectCenter);
        void quickFill();
    };

    class $setting_category("flood-fill-logo.png"_spr, "Im not quite sure how i can describe this features, it fills rects or smth rawr");

    inline SillySetting<bool> notifications{
        "Notifications", feature, true
    };
    inline SillySetting<bool> selectFill{
        "Select Fill", feature, true, "select filled objects after filling"
    };
    inline SillySetting<bool> selectBoundry{
        "Select Boundry", feature, false, "select flood fill boundry after filling"
    };
    inline SillySetting<bool> quickFillButton{
        "Quick Fill Button", feature, true, SettingReload::Editor
    };
}