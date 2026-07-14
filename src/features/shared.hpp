#pragma once

#include <internal/utils/utils.hpp>

namespace Shared {
    nwo5::utils::SillyDrawNode* getGridDraw();
    cocos2d::CCLayer* getGridLayer();
    nwo5::utils::SillyDrawNode* getOverlayDraw();
    cocos2d::CCLayer* getOverlayLayer();
    cocos2d::CCLayer* getHiddenLayer();
    
    enum class ChromaNode {
        FreeSnap = 0,
        FreeSnapInvert = 180,
    };

    // idk how cocos does their c style bullshit and quite frankly i dont care to find out rn
    // note from the future wtf was i doing here but also im too lazy to change it so :3c
    #define SE_UPDATE_FUNC(pFunc) [this]{ pFunc (); }
    void addUpdateFunc(geode::Function<void()> pFunc);
}