#include "setting.hpp"

using namespace geode::prelude;

namespace sillyedit::settings {
    std::string generateKey(std::string key) {
        return string::toLower(string::replace(std::move(key), " ", "-"));
    }
}