#pragma once

#include <nwo5.silly-api/include.hpp>

namespace sillyedit::settings {
    template<typename>
    class Setting;
}

namespace sillyedit::utils {
    constexpr unsigned char modifyOpacity(unsigned char opacity, unsigned char by) {
        return by ? static_cast<unsigned char>(std::clamp(opacity / (255.0f / by), 0.0f, 255.0f)) : 0;
    }

    constexpr cocos2d::CCPoint getLineCut(cocos2d::CCPoint start, cocos2d::CCRect rect) {
        const auto direction = ((rect.origin + rect.size / 2) - start);
        auto intersection = start + direction;
        float cutAt = 1.0f;

        for (float edge : {rect.getMinX(), rect.getMaxX()}) {
            if (!direction.x) {
                continue;
            }

            const auto t = (edge - start.x) / direction.x;
            const auto y = start.y + t * direction.y;

            if (t >= 0.0f && t <= 1.0f && y >= rect.getMinY() && y <= rect.getMaxY() && t < cutAt) {
                intersection = cocos2d::CCPoint{edge, y};
                cutAt = t;
            }
        }

        for (float edge : {rect.getMinY(), rect.getMaxY()}) {
            if (!direction.y) {
                continue;
            }

            const auto t = (edge - start.y) / direction.y;
            const auto x = start.x + t * direction.x;

            if (t >= 0.0f && t <= 1.0f && x >= rect.getMinX() && x <= rect.getMaxX() && t < cutAt) {
                intersection = cocos2d::CCPoint{x, edge};
                cutAt = t;
            }
        }

        return intersection;
    }

    constexpr uint32_t hashInt(uint32_t val, uint32_t offset = 0) {
        val ^= offset + 0x9e3779b9;
        val = (val ^ 61) ^ (val >> 16);
        val += (val << 3);
        val ^= (val >> 4);
        val *= 0x27d4eb2d;
        val ^= (val >> 15);
        
        return val;
    }

    std::pair<float, float> getChromaSettings();

    template<typename T = cocos2d::ccColor4F, typename U = float>
    auto getChroma(U offset = 0.0f) {
        const auto [speed, saturation] = getChromaSettings();

        return nwo5::utils::getChroma<T, U>(speed, offset, saturation);
    }

    bool isProbablierObjectString(std::string_view str);

    bool modifierDown(const settings::Setting<std::string>& str);

    // should mayb move these to sillyapi
    inline constexpr cocos2d::CCPoint TRIGGER_BODY_OFFSET{0.0f, -4.0f};
    bool triggerHasBodyOffset(int iD);

    bool weightedFuzzyMatch(geode::ZStringView str, geode::ZStringView query, double wight, double& out);

    // these help *a lot* btw
    inline constexpr auto localTriggerInfoArray = nwo5::editor::trigger::impl::createTriggerInfoArray();
    GEODE_INLINE inline bool isTriggerFast(GameObject* obj) {
        return obj->m_classType == GameObjectClassType::Effect && (obj->m_isTrigger || localTriggerInfoArray[std::min(obj->m_objectID, nwo5::editor::constants::OBJECT_IDS)].isTrigger());
    }
    GEODE_INLINE inline float pointDistanceSQFast(float x1, float x2, float y1, float y2) {
        return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
    }
    GEODE_INLINE inline bool canSelectLayerFast(GameObject* obj, int currentLayer) {
        if (currentLayer == nwo5::editor::constants::ALL_LAYERS) {
            return true;
        }

        if (obj->m_editorLayer == currentLayer) {
            return true;
        }
        else if (const auto layer2 = obj->m_editorLayer2; layer2 && layer2 == currentLayer) {
            return true;
        }

        return false;
    }

    // remind me to js move this to selection utils or smth cuz its only used there anyway and its dumb and stupod
    CCTextInputNode* findUITextInputs(cocos2d::CCPoint pos);
}