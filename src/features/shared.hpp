#pragma once

namespace Shared {
    cocos2d::CCDrawNode* getGridDraw();
    cocos2d::CCLayer* getGridLayer();
    cocos2d::CCDrawNode* getOverlayDraw();
    cocos2d::CCLayer* getOverlayLayer();
    cocos2d::CCLayer* getHiddenLayer();
    
    enum class ChromaNode {
        FreeSnap = 0,
        FreeSnapInvert = 3,
    };

    cocos2d::ccColor3B getChroma3B(int pNode);
    template<typename ImplT = cocos2d::ccColor4F, typename Node = size_t, typename T = std::decay_t<ImplT>>
    T getChroma(Node pNode) {
        if constexpr (std::same_as<T, cocos2d::ccColor3B>) {
            return getChroma3B(static_cast<size_t>(pNode));
        }
        else if constexpr (std::same_as<T, cocos2d::ccColor4B>) {
            return geode::cocos::to4B(getChroma3B(static_cast<size_t>(pNode)));
        }
        else if constexpr (std::same_as<T, cocos2d::ccColor4F>) {
            return cocos2d::ccc4FFromccc3B(getChroma3B(static_cast<size_t>(pNode)));
        }
        else {
            static_assert("u cant b that type of gay ;3");
        }
    }

    // idk how cocos does their c style bullshit and quite frankly i dont care to find out rn
    #define SE_UPDATE_FUNC(pFunc) [this]{ pFunc (); }
    void addUpdateFunc(std::move_only_function<void()> pFunc);
}