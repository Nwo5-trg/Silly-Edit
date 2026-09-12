#pragma once

#include <nwo5.silly-api/include/include.hpp>

namespace sillyedit::utils {
    constexpr unsigned char modifyOpacity(unsigned char pOpacity, unsigned char pBy) {
        return pBy ? static_cast<unsigned char>(std::clamp(pOpacity / (255.0f / pBy), 0.0f, 255.0f)) : 0;
    }

    std::pair<float, float> getChromaSettings();

    template<typename T = cocos2d::ccColor4F, typename U = float>
    auto getChroma(U pOffset = 0.0f) {
        const auto [speed, saturation] = getChromaSettings();

        return nwo5::utils::getChroma<T, U>(speed, pOffset, saturation);
    }

    bool isProbablierObjectString(std::string_view pStr);

    // should mayb move these to sillyapi
    constexpr cocos2d::CCPoint TRIGGER_BODY_OFFSET{0.0f, -4.0f};
    bool triggerHasBodyOffset(int pID);
}