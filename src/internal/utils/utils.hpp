#pragma once

#include <nwo5.silly-api/include/include.hpp>

using namespace nwo5::editor::prelude;

namespace nwo5::utils {
    float getChromaSpeed();

    template<typename T = cocos2d::ccColor4F, typename U>
    auto getChroma(U pOffset) {
        return nwo5::utils::getChroma<T, U>(getChromaSpeed(), pOffset, 0.5f);
    }
}