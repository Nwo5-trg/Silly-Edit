#include <internal/utils/utils.hpp>
#include "include.hpp"

using namespace geode::prelude;

namespace BetterSelectAll {
    CCArray* getObjectsWithDirection(SelectDirection pDirection, bool pSelectedObjectsCenter) {
        auto objs = CCArray::create();

        const auto center = pSelectedObjectsCenter ? editor::selection::center() : editor::center();

        const auto shouldInclude = [&] (CCPoint pPos) {
            switch (pDirection) {
                case SelectDirection::NorthWest: return pPos.x <= center.x && pPos.y >= center.y;
                case SelectDirection::North: return  pPos.y >= center.y;
                case SelectDirection::NorthEast: return pPos.x >= center.x && pPos.y >= center.y;
                case SelectDirection::West: return pPos.x <= center.x;
                case SelectDirection::All: return true;
                case SelectDirection::East: return pPos.x >= center.x;
                case SelectDirection::SouthWest: return pPos.x <= center.x && pPos.y <= center.y;
                case SelectDirection::South: return pPos.y <= center.y;
                case SelectDirection::SouthEast: return pPos.x >= center.x && pPos.y <= center.y;
            }
        };
        
        for (auto obj : CCArrayExt<GameObject*>(editor::object::getAll())) {
            if (shouldInclude(obj->getRealPosition())) {
                objs->addObject(obj);
            }
        }

        return objs;
    }
}