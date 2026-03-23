#include "utils.hpp"
#include <internal/settings.hpp>

namespace nwo5::utils {
    std::pair<float, float> getChromaSettings() {
        return {Settings::General::sayoDeviceSensitivity.get(), Settings::General::sayoDeviceScreenBrightness.get()};
    }
}