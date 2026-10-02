#pragma once

#include <nwo5.silly-api/include/include.hpp>

namespace sillyedit::settings {
    template<typename>
    class SillySetting;
}

namespace sillyedit::utils {
    constexpr unsigned char modifyOpacity(unsigned char pOpacity, unsigned char pBy) {
        return pBy ? static_cast<unsigned char>(std::clamp(pOpacity / (255.0f / pBy), 0.0f, 255.0f)) : 0;
    }

    constexpr cocos2d::CCPoint getLineCut(cocos2d::CCPoint pStart, cocos2d::CCRect pRect) {
        const auto direction = ((pRect.origin + pRect.size / 2) - pStart);
        auto intersection = pStart + direction;
        float cutAt = 1.0f;

        for (float edge : {pRect.getMinX(), pRect.getMaxX()}) {
            if (!direction.x) {
                continue;
            }

            const auto t = (edge - pStart.x) / direction.x;
            const auto y = pStart.y + t * direction.y;

            if (t >= 0.0f && t <= 1.0f && y >= pRect.getMinY() && y <= pRect.getMaxY() && t < cutAt) {
                intersection = cocos2d::CCPoint{edge, y};
                cutAt = t;
            }
        }

        for (float edge : {pRect.getMinY(), pRect.getMaxY()}) {
            if (!direction.y) {
                continue;
            }

            const auto t = (edge - pStart.y) / direction.y;
            const auto x = pStart.x + t * direction.x;

            if (t >= 0.0f && t <= 1.0f && x >= pRect.getMinX() && x <= pRect.getMaxX() && t < cutAt) {
                intersection = cocos2d::CCPoint{x, edge};
                cutAt = t;
            }
        }

        return intersection;
    }

    constexpr uint32_t hashInt(uint32_t pVal, uint32_t pOffset = 0) {
        pVal ^= pOffset + 0x9e3779b9;
        pVal = (pVal ^ 61) ^ (pVal >> 16);
        pVal += (pVal << 3);
        pVal ^= (pVal >> 4);
        pVal *= 0x27d4eb2d;
        pVal ^= (pVal >> 15);
        
        return pVal;
    }

    std::pair<float, float> getChromaSettings();

    template<typename T = cocos2d::ccColor4F, typename U = float>
    auto getChroma(U pOffset = 0.0f) {
        const auto [speed, saturation] = getChromaSettings();

        return nwo5::utils::getChroma<T, U>(speed, pOffset, saturation);
    }

    bool isProbablierObjectString(std::string_view pStr);

    bool modifierDown(const settings::SillySetting<std::string>& pStr);

    // should mayb move these to sillyapi
    inline constexpr cocos2d::CCPoint TRIGGER_BODY_OFFSET{0.0f, -4.0f};
    bool triggerHasBodyOffset(int pID);

    // these help *a lot* btw
    inline constexpr auto localTriggerInfoArray = nwo5::editor::trigger::impl::createTriggerInfoArray();
    GEODE_INLINE inline bool isTriggerFast(GameObject* pObj) {
        return pObj->m_classType == GameObjectClassType::Effect && (pObj->m_isTrigger || localTriggerInfoArray[std::min(pObj->m_objectID, nwo5::editor::constants::OBJECT_IDS)].isTrigger());
    }
    GEODE_INLINE inline float pointDistanceSQFast(float pX1, float pX2, float pY1, float pY2) {
        return (pX2 - pX1) * (pX2 - pX1) + (pY2 - pY1) * (pY2 - pY1);
    }

    // remind me to js move this to selection utils or smth cuz its only used there anyway and its dumb and stupod
    CCTextInputNode* findUITextInputs(cocos2d::CCPoint pPos);
}