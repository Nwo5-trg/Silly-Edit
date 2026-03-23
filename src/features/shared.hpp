#pragma once

namespace Shared {
    cocos2d::CCDrawNode* getGridDraw();
    cocos2d::CCLayer* getGridLayer();
    cocos2d::CCDrawNode* getOverlayDraw();
    cocos2d::CCLayer* getOverlayLayer();
    cocos2d::CCLayer* getHiddenLayer();
    
    enum class ChromaNode {
        FreeSnap = 0,
        FreeSnapInvert = 180,
    };

    // idk how cocos does their c style bullshit and quite frankly i dont care to find out rn
    #define SE_UPDATE_FUNC(pFunc) [this]{ pFunc (); }
    void addUpdateFunc(geode::Function<void()> pFunc);
}