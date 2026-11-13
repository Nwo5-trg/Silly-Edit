#pragma once

namespace FF {
    struct Rect {
        cocos2d::CCPoint start;
        cocos2d::CCPoint end;

        Rect(cocos2d::CCPoint center, cocos2d::CCSize size)
            : start(center - size / 2), end(center + size / 2) {}
        Rect(cocos2d::CCPoint start, cocos2d::CCPoint end)
            : start(start), end(end) {}
        Rect(float top, float left, float bottom, float right)
            : start(left, bottom), end(right, top) {}

        static Rect bounds(std::span<const Rect> rects) {
            Rect out {
                cocos2d::CCPoint{std::numeric_limits<float>::max(), std::numeric_limits<float>::max()},
                cocos2d::CCPoint{std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest()}
            };

            for (const auto& rect : rects) {
                out.start.x = std::min(rect.start.x, out.start.x);
                out.start.y = std::min(rect.start.y, out.start.y);
                out.end.x = std::max(rect.end.x, out.end.x);
                out.end.y = std::max(rect.end.y, out.end.y);
            }
            
            return out;
        }
        
        Rect operator+(cocos2d::CCPoint offset) const {
            return {start + offset, end + offset};
        }
        bool operator==(const Rect& other) const {
            return start == other.start && end == other.end;
        }

        bool contains(cocos2d::CCPoint point) const {
            return point.x >= start.x 
                && point.x <= end.x 
                && point.y >= start.y 
                && point.y <= end.y;
        }
        bool touches(const Rect& other) const {
            return (start.x <= other.end.x && end.x >= other.start.x) 
                && (start.y <= other.end.y && end.y >= other.start.y);
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
    
    // doesnt include center
    std::vector<Rect> gridFloodFill(std::vector<Rect> shape, const Rect& center, bool p8Direction);
    Rect rectFromObject(GameObject* obj, std::optional<cocos2d::CCPoint> center = std::nullopt);
    std::vector<Rect> rectsFromObjects(cocos2d::CCArray* objs);
}