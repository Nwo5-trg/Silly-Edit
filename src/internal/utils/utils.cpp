#include "utils.hpp"
#include <features/settings/settings.hpp>

namespace nwo5::utils {
    float getChromaSpeed() {
        return Settings::General::sayoDeviceSensitivity;
    }
}