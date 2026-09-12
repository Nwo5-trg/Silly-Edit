#pragma once

#include <nwo5.silly-api/include/include.hpp>

using namespace nwo5::editor::prelude;

namespace sillyedit::utils {
    enum class DrawNode {
        Default
    };
    enum class ChromaNode {
        Default = 0,
        SelectionUtilsDefault = 60,
        SelectionUtilsInvert = 240,
    };

    nwo5::utils::SillyDrawNode* getGridDraw(DrawNode pDrawNode = DrawNode::Default);
    cocos2d::CCLayer* getGridLayer();
    nwo5::utils::SillyDrawNode* getOverlayDraw(DrawNode pDrawNode = DrawNode::Default);
    cocos2d::CCLayer* getOverlayLayer();
    cocos2d::CCLayer* getHiddenLayer();
    
    inline auto& shouldApplyCustomPlacedObjectOptions() {
        static bool val = false;
        return val;
    }
    inline auto& shouldBlockScrolling() {
        static bool val = false;
        return val;
    }
}