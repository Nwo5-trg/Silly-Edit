#pragma once

#include <nwo5.silly-api/include/include.hpp>

using namespace nwo5::editor::prelude;

namespace Sillyedit {
    enum class DrawNode {
        Default
    };
    enum class ChromaNode {
        Default = 0,
        SelectionUtilsInvert = 180,
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