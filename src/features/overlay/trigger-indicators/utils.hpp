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
            std::vector<std::vector<GameObject*>> clusterResult;

            std::vector<GameObject*> objectTargets;
            std::vector<GameObject*> triggerTargets;
        } m_state;

        std::vector<EffectGameObject*> m_triggers;
        std::array<bool, nwo5::editor::constants::MAX_GROUPS + 1> m_groupBlacklist;
        std::array<bool, nwo5::editor::constants::OBJECT_IDS + 1> m_triggerBlacklist;

        void drawTargets(cocos2d::CCPoint start, bool targetingTrigger);

        void drawToRect(cocos2d::CCPoint start, cocos2d::CCRect rect);   
        void drawLine(cocos2d::CCPoint start, cocos2d::CCPoint end);

        void drawInputExtra(cocos2d::CCPoint pos, cocos2d::CCSize scale, float opacity);
        void drawOutputExtra(cocos2d::CCPoint pos, cocos2d::CCSize scale, float opacity);

        void updateTargets(int group);

        static cocos2d::CCPoint inputExtraPosFor(GameObject* obj);
        static std::pair<cocos2d::CCPoint, cocos2d::CCPoint> outputExtraPosFor(GameObject* obj, bool hasCenter);

        static void clusterObjects(std::vector<std::vector<GameObject*>>& out, std::span<GameObject* const> objs, float clusterSize);
        static cocos2d::CCRect getObjectBounds(std::span<GameObject* const> objs, bool addSize);
        
    public:
        void draw();
        void updateBlacklist();
        void updateTriggerList();
    };
}