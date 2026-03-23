#pragma once

#include <nwo5.silly-api/include/include.hpp>

using namespace nwo5::editor::prelude;

namespace nwo5::utils {
    std::pair<float, float> getChromaSettings();

    template<typename T = cocos2d::ccColor4F, typename U>
    auto getChroma(U pOffset) {
        const auto [speed, saturation] = getChromaSettings();

        return nwo5::utils::getChroma<T, U>(speed, pOffset, saturation);
    }
}