#pragma once

#include <internal/utils/utils.hpp>

namespace Shared {
    enum class DrawNode {
        Default
    };

    nwo5::utils::SillyDrawNode* getGridDraw(DrawNode pDrawNode = DrawNode::Default);
    cocos2d::CCLayer* getGridLayer();
    nwo5::utils::SillyDrawNode* getOverlayDraw(DrawNode pDrawNode = DrawNode::Default);
    cocos2d::CCLayer* getOverlayLayer();
    cocos2d::CCLayer* getHiddenLayer();

    enum class ChromaNode {
        Default = 0,
        SelectionUtilsInvert = 180,
    };

    void addUpdateFunc(geode::Function<void()> pFunc);

    inline auto& shouldApplyCustomPlacedObjectOptions() {
        static bool val = false;
        return val;
    }
}