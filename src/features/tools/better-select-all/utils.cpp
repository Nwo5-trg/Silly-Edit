#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;

namespace BetterSelectAll {
    CCArray* getObjectsWithDirection(SelectDirection direction, bool selectedObjectsCenter) {
        auto objs = CCArray::create();

        const auto center = selectedObjectsCenter ? selection::center() : editor::center();

        const auto shouldInclude = [&] (CCPoint pos) {
            switch (direction) {
                case SelectDirection::NorthWest: return pos.x <= center.x && pos.y >= center.y;
                case SelectDirection::North: return  pos.y >= center.y;
                case SelectDirection::NorthEast: return pos.x >= center.x && pos.y >= center.y;
                case SelectDirection::West: return pos.x <= center.x;
                case SelectDirection::All: return true;
                case SelectDirection::East: return pos.x >= center.x;
                case SelectDirection::SouthWest: return pos.x <= center.x && pos.y <= center.y;
                case SelectDirection::South: return pos.y <= center.y;
                case SelectDirection::SouthEast: return pos.x >= center.x && pos.y <= center.y;
            }
        };
        
        for (auto obj : CCArrayExt<GameObject*>(object::getAll())) {
            if (shouldInclude(obj->getRealPosition())) {
                objs->addObject(obj);
            }
        }

        return objs;
    }
}