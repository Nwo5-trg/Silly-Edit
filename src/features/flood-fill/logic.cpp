#include "include.hpp"

using namespace geode::prelude;

namespace FloodFill {
    // // girl dont judge me oki this took me like 3 days to come up with and im proud
    // static void recursiveFloodFill(const Rect& pParent, std::vector<Rect>& pShape, std::vector<Rect>& pOutput) {  
    //     for (const auto& rect : pShape) {
    //         if (rect.contains(pParent.center())) {
    //             return;
    //         }
    //     }

    //     const auto bounds = Rect::bounds(pShape);

    //     // values are limits of child in direction
    //     Rect children = bounds;

    //     for (const auto& rect : pShape) {
    //         if (rect.left() <= pParent.right() && rect.right() >= pParent.left()) {
    //             if (rect.bottom() >= pParent.top() && rect.bottom() < children.top()) {
    //                 children.top() = rect.bottom();
    //             }
    //             if (rect.top() <= pParent.bottom() && rect.top() > children.bottom()) {
    //                 children.bottom() = rect.top();
    //             }
    //         }
    //         if (rect.bottom() <= pParent.top() && rect.top() >= pParent.bottom()) {
    //             if (rect.right() <= pParent.left() && rect.right() > children.left()) {
    //                 children.left() = rect.right();
    //             }
    //             if (rect.left() >= pParent.right() && rect.left() < children.right()) {
    //                 children.right() = rect.left();
    //             }
    //         }
    //     }

    //     // values are maximum of child in opposite axis in direction
    //     Rect childrenSqueezeMax{
    //         bounds.left(), bounds.bottom(), bounds.right(), bounds.top()
    //     };
    //     // values are minimum of child in direction
    //     Rect childrenSqueezeMin{
    //         bounds.right(), bounds.top(), bounds.left(), bounds.bottom()
    //     };

    //     for (const auto& rect : pShape) {
    //         if (children.top() != pParent.top() && rect.top() >= pParent.top() && rect.bottom() <= children.top()) {
    //             if (rect.left() > childrenSqueezeMax.top()) {
    //                 childrenSqueezeMax.top() = rect.left();
    //             }
    //             if (rect.right() < childrenSqueezeMin.top()) {
    //                 childrenSqueezeMin.top() = rect.right();
    //             }
    //         }
    //         if (children.left() != pParent.left() && rect.left() <= pParent.left() && rect.right() >= children.left()) {
    //             if (rect.top() > childrenSqueezeMax.left()) {
    //                 childrenSqueezeMax.left() = rect.top();
    //             }
    //             if (rect.bottom() < childrenSqueezeMin.left()) {
    //                 childrenSqueezeMin.left() = rect.bottom();
    //             }
    //         }
    //         if (children.bottom() != pParent.bottom() && rect.bottom() <= pParent.bottom() && rect.top() >= children.bottom()) {
    //             if (rect.left() > childrenSqueezeMax.bottom()) {
    //                 childrenSqueezeMax.bottom() = rect.left();
    //             }
    //             if (rect.right() < childrenSqueezeMin.bottom()) {
    //                 childrenSqueezeMin.bottom() = rect.right();
    //             }
    //         }
    //         if (children.right() != pParent.right() && rect.right() >= pParent.right() && rect.left() <= children.right()) {
    //             if (rect.top() > childrenSqueezeMax.right()) {
    //                 childrenSqueezeMax.right() = rect.top();
    //             }
    //             if (rect.bottom() < childrenSqueezeMin.right()) {
    //                 childrenSqueezeMin.right() = rect.bottom();
    //             }
    //         }
    //     }

    //     pShape.push_back(pParent);
    //     pOutput.push_back(pParent);

    //     recursiveFloodFill({children.top(), childrenSqueezeMin.top(), pParent.top(), childrenSqueezeMax.top()}, pShape, pOutput);
    //     recursiveFloodFill({childrenSqueezeMax.left(), children.left(), childrenSqueezeMin.left(), pParent.left()}, pShape, pOutput);
    //     recursiveFloodFill({pParent.bottom(), childrenSqueezeMin.bottom(), children.bottom(), childrenSqueezeMax.bottom()}, pShape, pOutput);
    //     recursiveFloodFill({childrenSqueezeMax.right(), pParent.right(), childrenSqueezeMin.right(), children.right()}, pShape, pOutput);
    // }

    // std::vector<Rect> floodFill(std::vector<Rect> pShape, cocos2d::CCPoint pCenter, bool pIncludeShape) {
    //     std::vector<Rect> out;

    //     Rect start = Rect::bounds(pShape);

    //     for (const auto& rect : pShape) {
    //         if (rect.bottom() >= pCenter.y && rect.bottom() < start.top()) {
    //             start.top() = rect.bottom();
    //         }
    //         if (rect.right() <= pCenter.x && rect.right() > start.left()) {
    //             start.left() = rect.right();
    //         }
    //         if (rect.top() <= pCenter.y && rect.top() > start.bottom()) {
    //             start.bottom() = rect.top();
    //         }
    //         if (rect.left() >= pCenter.x && rect.left() < start.right()) {
    //             start.right() = rect.left();
    //         }
    //     }

    //     recursiveFloodFill(start, pShape, out);

    //     if (pIncludeShape) {
    //         for (const auto& rect : pShape) {
    //             out.push_back(rect);
    //         }
    //     }

    //     return optimize(out);
    // }
    // std::vector<Rect> optimize(std::vector<Rect> pRects) {
    //     std::vector<Rect> out = std::move(pRects);

    //     // wip

    //     return out;
    // }

    std::vector<Rect> gridFloodFill(std::vector<Rect> pShape, const Rect& pCenter, bool p8Direction, bool pUseCenterEdgeResolving) {
        std::vector<Rect> out;

        std::vector<Rect> stack = {pCenter};
        
        while (!stack.empty()) {
            const auto rect = stack.back();
            stack.pop_back();

            // i love std::ranges algs, gotta be one of my favorite genders
            if (std::ranges::any_of(pShape, [&] (const auto& pBoundry) {
                return pUseCenterEdgeResolving ? pBoundry.contains(rect.center()) : pBoundry.touches(rect);
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
}