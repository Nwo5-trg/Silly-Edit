#pragma once

#include <utils/include.hpp>

namespace TriggerIndicators {
    class Drawer {
    protected:
        struct {
            cocos2d::ccColor4F color{0.0f, 0.0f, 0.0f, 1.0f};
            float thickness = 0.0f;
            bool isCenter = false;
            EffectGameObject* trigger = nullptr;
            std::vector<GameObject*> objectTargets;
            std::vector<GameObject*> triggerTargets;
            std::vector<std::vector<GameObject*>> clusterResult;
        } m_state;

        std::array<bool, nwo5::editor::constants::MAX_GROUPS + 1> m_groupBlacklist;
        std::array<bool, nwo5::editor::constants::OBJECT_IDS + 1> m_triggerBlacklist;

        void drawTargets(cocos2d::CCPoint pStart, bool pTargetingTrigger);

        void drawToRect(cocos2d::CCPoint pStart, cocos2d::CCRect pRect);   
        void drawLine(cocos2d::CCPoint pStart, cocos2d::CCPoint pEnd);

        void drawInputExtra(cocos2d::CCPoint pPos, cocos2d::CCSize pScale, float pOpacity);
        void drawOutputExtra(cocos2d::CCPoint pPos, cocos2d::CCSize pScale, float pOpacity);

        void updateTargets(int pGroup);

        cocos2d::CCPoint inputExtraPosFor(GameObject* pObj) const;
        std::pair<cocos2d::CCPoint, cocos2d::CCPoint> outputExtraPosFor(GameObject* pObj, bool pHasCenter) const;
        
    public:
        void draw();
        void updateBlacklist();
    };
}