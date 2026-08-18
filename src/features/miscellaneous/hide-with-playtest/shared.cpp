#include <utils/include.hpp>
#include "include.hpp"

enum class HideWithPlaytestType {
    Default,
    DBlock
};

static constexpr auto createTypeArray() {
    std::array<HideWithPlaytestType, editor::constants::OBJECT_IDS + 1> arr;
    arr.fill(HideWithPlaytestType::Default);

    arr[1755] = HideWithPlaytestType::DBlock;
    arr[1813] = HideWithPlaytestType::DBlock;
    arr[1829] = HideWithPlaytestType::DBlock;
    arr[1859] = HideWithPlaytestType::DBlock;
    arr[2866] = HideWithPlaytestType::DBlock;

    return arr;
}

namespace Shared {
    unsigned char hideWithPlaytestOpacityForObject(unsigned char pOpacity, GameObject* pObj) {
        static auto arr = createTypeArray();

        const auto id = pObj->m_objectID;

        if (HideWithPlaytest::hideTriggers.get() && editor::trigger::is(id)) {
            return nwo5::utils::modifyOpacity(pOpacity, HideWithPlaytest::triggerOpacity.get());
        }
        
        switch (arr[id]) {
            case HideWithPlaytestType::Default: {
                return pOpacity;
            }
            case HideWithPlaytestType::DBlock: {
                return nwo5::utils::modifyOpacity(pOpacity, HideWithPlaytest::specialBlockOpacity.get());
            }
        }
    }
};