#include <settings/include.hpp>
#include "include.hpp"

using namespace geode::prelude;

namespace Sillyedit {
    std::pair<float, float> getChromaSettings() {
        return {Settings::sayoDeviceSensitivity, Settings::sayoDeviceScreenBrightness};
    }
}