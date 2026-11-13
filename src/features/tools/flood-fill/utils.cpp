#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;

namespace FF {
    std::vector<Rect> gridFloodFill(std::vector<Rect> shape, const Rect& center, bool p8Direction) {
        const auto bounds = Rect::bounds(shape);

        std::vector<Rect> out;

        std::vector<Rect> stack = {center};
        
        while (!stack.empty()) {
            const auto rect = stack.back();
            stack.pop_back();

            if (!bounds.contains(rect.center())) {
                continue;
            }

            // i love std::ranges algs, gotta be one of my favorite genders
            if (std::ranges::any_of(shape, [&] (const auto& boundry) {
                return boundry.contains(rect.center());
            })) {
                continue;
            }

            shape.push_back(rect);
            if (rect != center) {
                out.push_back(rect);
            }

            stack.push_back(rect + CCPoint{0, rect.height()});
            stack.push_back(rect + CCPoint{0, -rect.height()});
            stack.push_back(rect + CCPoint{-rect.width(), 0});
            stack.push_back(rect + CCPoint{rect.width(), 0});
            
            if (p8Direction) {
                stack.push_back(rect + CCPoint{-rect.width(), rect.height()});
                stack.push_back(rect + CCPoint{rect.width(), rect.height()});
                stack.push_back(rect + CCPoint{-rect.width(), -rect.height()});
                stack.push_back(rect + CCPoint{rect.width(), -rect.height()});
            }
        }

        return out;
    }

    Rect rectFromObject(GameObject* obj, std::optional<CCPoint> center) {
        return {
            center.has_value() ? center.value() : obj->getRealPosition(), 
            CCSize{obj->m_scaleX, obj->m_scaleY} * object::size(obj)
        };
    }
    std::vector<Rect> rectsFromObjects(CCArray* objs) {
        std::vector<Rect> out;

        for (auto obj : CCArrayExt<GameObject*>(objs)) {
            out.emplace_back(obj->getRealPosition(), CCSize{obj->m_scaleX, obj->m_scaleY} * object::size(obj));
        }

        return out;
    }
}