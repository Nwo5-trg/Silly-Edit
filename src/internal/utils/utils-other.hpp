#pragma once

namespace Utils {
    // fuck you geode for not having a substr overload (as of writing this idk abt v5)
    size_t stringCount(std::string_view pString, std::string_view pSubstr);
    size_t stringCount(std::string_view pString, char pSubstr);
    // ccmenuitemtoggler triggers callback before its actually toggled so
    // technically breaks when calling with togglewithcallback but idk fuck robtop
    bool isToggled(cocos2d::CCObject* pToggler);

    template<typename T>
    T random(T pMin, T pMax) {
        return geode::utils::random::generate(pMin, pMax);
    }
    template<typename T>
    requires std::is_integral_v<T>
    T random(T pMin, T pMax) {
        return geode::utils::random::generate(pMin, pMax + static_cast<T>(1));
    }

    template<typename T>
    auto numToString(T pNum, size_t pPrecision = 4) {
        return geode::utils::numToString(pNum, pPrecision);
    }
    template<typename T>
    requires std::is_floating_point_v<T>
    auto numToString(T pNum, size_t pPrecision = 4) {
        auto str = geode::utils::numToString(pNum, pPrecision);
        
        if (str.find('.') != std::string::npos) {
            if (const auto end = str.find_last_not_of('0'); end != std::string::npos) {
                str.erase(end + 1);
            }

            if (str.ends_with('.')) {
                str.pop_back();
            }
        }

        return str;
    }

    template<typename Callback>
    void setupKeybind(cocos2d::CCNode* pNode, std::string pKeybind, Callback&& pCallback) {
        // no string_view 3:
        pNode->addEventListener(geode::KeybindSettingPressedEventV3(geode::Mod::get(), std::move(pKeybind)), std::forward<Callback>(pCallback));
    }
}