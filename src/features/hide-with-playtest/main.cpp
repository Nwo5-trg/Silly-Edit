#include "settings.hpp"
#include "include.hpp"
#include <internal/utils/utils.hpp>

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

namespace HideWithPlaytest {
    unsigned char opacityForObject(unsigned char pOpacity, GameObject* pObj) {
        static auto arr = createTypeArray();

        const auto id = pObj->m_objectID;

        if (Settings::HideWithPlaytest::hideTriggers.get() && editor::trigger::is(id)) {
            return nwo5::utils::modifyOpacity(pOpacity, Settings::HideWithPlaytest::triggerOpacity.get());
        }
        
        switch (arr[id]) {
            case HideWithPlaytestType::Default: {
                return pOpacity;
            }
            case HideWithPlaytestType::DBlock: {
                return nwo5::utils::modifyOpacity(pOpacity, Settings::HideWithPlaytest::specialBlockOpacity.get());
            }
        }
    }
};