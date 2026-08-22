#pragma once

#include <nwo5.silly-api/include/include.hpp>

namespace Sillyedit {
    constexpr unsigned char modifyOpacity(unsigned char pOpacity, unsigned char pBy) {
        return pBy ? static_cast<unsigned char>(std::clamp(pOpacity / (255.0f / pBy), 0.0f, 255.0f)) : 0;
    }

    std::pair<float, float> getChromaSettings();

    template<typename T = cocos2d::ccColor4F, typename U>
    auto getChroma(U pOffset) {
        const auto [speed, saturation] = getChromaSettings();

        return nwo5::utils::getChroma<T, U>(speed, pOffset, saturation);
    }
}