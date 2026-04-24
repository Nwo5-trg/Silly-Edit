#pragma once

namespace FloodFill {
    struct Rect {
        cocos2d::CCPoint start;
        cocos2d::CCPoint end;

        Rect(cocos2d::CCPoint pCenter, cocos2d::CCSize pSize)
            : start(pCenter - pSize / 2), end(pCenter + pSize / 2) {}
        Rect(cocos2d::CCPoint pStart, cocos2d::CCPoint pEnd)
            : start(pStart), end(pEnd) {}
        Rect(float pTop, float pLeft, float pBottom, float pRight)
            : start(pLeft, pBottom), end(pRight, pTop) {}

        static Rect bounds(std::span<const Rect> pRects) {
            Rect out {
                ccp(std::numeric_limits<float>::max(), std::numeric_limits<float>::max()),
                ccp(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest())
            };

            for (const auto& rect : pRects) {
                out.start.x = std::min(rect.start.x, out.start.x);
                out.start.y = std::min(rect.start.y, out.start.y);
                out.end.x = std::max(rect.end.x, out.end.x);
                out.end.y = std::max(rect.end.y, out.end.y);
            }
            
            return out;
        }
        
        Rect operator+(cocos2d::CCPoint pOffset) const {
            return {start + pOffset, end + pOffset};
        }
        bool operator==(const Rect& pOther) const {
            return start == pOther.start && end == pOther.end;
        }

        bool contains(cocos2d::CCPoint pPoint) const {
            return pPoint.x >= start.x 
                && pPoint.x <= end.x 
                && pPoint.y >= start.y 
                && pPoint.y <= end.y;
        }
        bool touches(const Rect& pOther) const {
            return (start.x <= pOther.end.x && end.x >= pOther.start.x) 
                && (start.y <= pOther.end.y && end.y >= pOther.start.y);
        }

        float top() const {
            return end.y;
        }
        float& top() {
            return end.y;
        }
        float left() const {
            return start.x;
        }
        float& left() {
            return start.x;
        }
        float bottom() const {
            return start.y;
        }
        float& bottom() {
            return start.y;
        }
        float right() const {
            return end.x;
        }
        float& right() {
            return end.x;
        }
        cocos2d::CCPoint center() const {
            return (start + end) / 2;
        }
        float height() const {
            return end.y - start.y;
        }
        float width() const {
            return end.x - start.x;
        }
    };

    // std::vector<Rect> floodFill(std::vector<Rect> pShape, cocos2d::CCPoint pCenter, bool pIncludeShape);
    // std::vector<Rect> optimize(std::vector<Rect> pRects);
    
    // doesnt include center
    std::vector<Rect> gridFloodFill(std::vector<Rect> pShape, const Rect& pCenter, bool p8Direction, bool pUseCenterEdgeResolving);
}