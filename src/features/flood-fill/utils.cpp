#include <internal/utils/utils.hpp>
#include "include.hpp"

using namespace geode::prelude;

namespace FF {
    std::vector<Rect> gridFloodFill(std::vector<Rect> pShape, const Rect& pCenter, bool p8Direction) {
        const auto bounds = Rect::bounds(pShape);

        std::vector<Rect> out;

        std::vector<Rect> stack = {pCenter};
        
        while (!stack.empty()) {
            const auto rect = stack.back();
            stack.pop_back();

            if (!bounds.contains(rect.center())) {
                continue;
            }

            // i love std::ranges algs, gotta be one of my favorite genders
            if (std::ranges::any_of(pShape, [&] (const auto& pBoundry) {
                return pBoundry.contains(rect.center());
            })) {
                continue;
            }

            pShape.push_back(rect);
            if (rect != pCenter) {
                out.push_back(rect);
            }

            stack.push_back(rect + ccp(0, rect.height()));
            stack.push_back(rect + ccp(0, -rect.height()));
            stack.push_back(rect + ccp(-rect.width(), 0));
            stack.push_back(rect + ccp(rect.width(), 0));
            
            if (p8Direction) {
                stack.push_back(rect + ccp(-rect.width(), rect.height()));
                stack.push_back(rect + ccp(rect.width(), rect.height()));
                stack.push_back(rect + ccp(-rect.width(), -rect.height()));
                stack.push_back(rect + ccp(rect.width(), -rect.height()));
            }
        }

        return out;
    }

    Rect rectFromObject(GameObject* pObj, std::optional<CCPoint> pCenter) {
        return {
            pCenter.has_value() ? pCenter.value() : pObj->getRealPosition(), 
            CCSize{pObj->m_scaleX, pObj->m_scaleY} * editor::object::size(pObj)
        };
    }
    std::vector<Rect> rectsFromObjects(CCArray* pObjs) {
        std::vector<Rect> out;

        for (auto obj : CCArrayExt<GameObject*>(pObjs)) {
            out.emplace_back(obj->getRealPosition(), CCSize{obj->m_scaleX, obj->m_scaleY} * editor::object::size(obj));
        }

        return out;
    }
}