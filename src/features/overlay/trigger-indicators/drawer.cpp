#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

namespace TriggerIndicators {
    void Drawer::drawFor(GameObject* pObj) {

    }

    void Drawer::drawInputExtra(GameObject* pObj) {

    }
    void Drawer::drawOutputExtra(GameObject* pObj) {

    }

    cocos2d::CCPoint Drawer::inputExtraPosFor(GameObject* pObj) const {
        return pObj->getRealPosition() + (sillyedit::utils::triggerHasBodyOffset(pObj->m_objectID) ? sillyedit::utils::TRIGGER_BODY_OFFSET : CCPointZero) - CCPoint{editor::constants::GRID_SIZE / 2, 0.0f};
    }
    std::pair<cocos2d::CCPoint, cocos2d::CCPoint> Drawer::outputExtraPosFor(GameObject* pObj) const {
        const auto pos = pObj->getRealPosition() + (sillyedit::utils::triggerHasBodyOffset(pObj->m_objectID) ? sillyedit::utils::TRIGGER_BODY_OFFSET : CCPointZero);
 
        if (trigger::get(pObj).center().exists()) {
            return {{pos.x + editor::constants::GRID_SIZE / 2, pos.y + 5.0f}, {pos.x + editor::constants::GRID_SIZE / 2, pos.y - 5.0f}};
        }
        else {
            return {{pos.x + editor::constants::GRID_SIZE / 2, pos.y}, CCPointZero};
        }
    }
    
    void Drawer::draw() {
        const auto shouldSelectionCull = !selection::empty() && std::ranges::any_of(selection::getExt(), [] (GameObject* pObj) {
            return object::hasGroups(pObj);
        });
        const auto cullDistance = ccMax(editor::size(false)) * (shouldSelectionCull ? TriggerIndicators::cullMultiplierSelectionMod.get() : TriggerIndicators::cullMultiplier);
        const auto cullDistanceSQ = cullDistance * cullDistance;
        const auto center = editor::center(false);

        for (auto obj : CCArrayExt<GameObject*>(editor::objectArray())) {
            if (!trigger::is(obj) || !obj->isVisible()) {
                continue;
            };

            if (obj->getRealPosition().getDistanceSq(center) < cullDistanceSQ) {
                continue;
            }
        }
    }
}