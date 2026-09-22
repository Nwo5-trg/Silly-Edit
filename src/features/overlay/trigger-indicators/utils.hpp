#pragma once

#include <utils/include.hpp>

namespace TriggerIndicators {
    class Drawer {
    protected:
        cocos2d::ccColor4F color{0.0f, 0.0f, 0.0f, 1.0f};
        float thickness = 0.0f;
        bool targetingTrigger = false;
        bool drawDotted = false;
        std::array<bool, nwo5::editor::constants::MAX_GROUPS + 1> groupBlacklist;
        std::array<bool, nwo5::editor::constants::OBJECT_IDS + 1> triggerBlacklist;
        std::vector<GameObject*> targets;

        void drawFor(GameObject* pObj);

        void drawInputExtra(GameObject* pObj);
        void drawOutputExtra(GameObject* pObj);

        cocos2d::CCPoint inputExtraPosFor(GameObject* pObj) const;
        std::pair<cocos2d::CCPoint, cocos2d::CCPoint> outputExtraPosFor(GameObject* pObj) const;
        
    public:
        void draw();
    };
}