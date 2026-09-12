#include <settings/include.hpp>
#include "include.hpp"

using namespace geode::prelude;

namespace sillyedit::utils {
    std::pair<float, float> getChromaSettings() {
        return {Settings::sayoDeviceSensitivity, Settings::sayoDeviceScreenBrightness};
    }

    bool isProbablierObjectString(std::string_view pStr) {
        if (pStr.find_first_of("1234567890") != 0) {
            return false;
        }

        if (pStr.ends_with(',') || pStr.ends_with('.') || misc::stringCount(pStr, ',') < 5) {
            return false;
        }

        return true;
    }

    bool triggerHasBodyOffset(int pID) {
        switch (pID) {
            case 22: [[fallthrough]];
            case 24: [[fallthrough]];
            case 23: [[fallthrough]];
            case 25: [[fallthrough]];
            case 26: [[fallthrough]];
            case 27: [[fallthrough]];
            case 28: [[fallthrough]];
            case 55: [[fallthrough]];
            case 56: [[fallthrough]];
            case 57: [[fallthrough]];
            case 58: [[fallthrough]];
            case 59: [[fallthrough]];
            case 1816: [[fallthrough]];
            case 1915: [[fallthrough]];
            case 3640: [[fallthrough]];
            case 3643: {
                return false;
            break; }
            default: {};
        }

        return trigger::is(pID);
    }
}