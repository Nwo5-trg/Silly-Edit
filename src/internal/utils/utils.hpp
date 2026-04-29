#pragma once

#include <nwo5.silly-api/include/include.hpp>

using namespace nwo5::editor::prelude;
using namespace nwo5::utils::prelude;

namespace nwo5::utils {
    std::pair<float, float> getChromaSettings();

    template<typename T = cocos2d::ccColor4F, typename U>
    auto getChroma(U pOffset) {
        const auto [speed, saturation] = getChromaSettings();

        return nwo5::utils::getChroma<T, U>(speed, pOffset, saturation);
    }

    inline const std::string BETTER_EDIT_ID{"hjfod.betteredit"};
    bool isBetterEditLoaded();
    geode::Mod* getBetterEdit();
    template<typename T>
    std::optional<T> getBetterEditSetting(std::string_view pKey) {
        auto be = getBetterEdit();

        if (!be) {
            return std::nullopt;
        }

        return be->getSettingValue<T>(pKey);
    }
    template<typename T>
    bool setBetterEditSetting(std::string_view pKey, T pVal) {
        auto be = getBetterEdit();

        if (!be) {
            return false;
        }

        be->setSettingValue<T>(pKey, pVal);

        return true;
    }
    
    inline const std::string TINKER_EDIT_ID{"alphalaneous.tinker"};
    bool isTinkerLoaded();
    geode::Mod* getTinker();
    template<typename T>
    std::optional<T> getTinkerSetting(std::string_view pKey) {
        auto tinker = getTinker();

        if (!tinker) {
            return std::nullopt;
        }

        return tinker->getSettingValue<T>(pKey);
    }
    template<typename T>
    bool setTinkerSetting(std::string_view pKey, T pVal) {
        auto tinker = getTinker();

        if (!tinker) {
            return false;
        }

        tinker->setSettingValue<T>(pKey, pVal);

        return true;
    }

    void enableHook(geode::Mod* pMod, std::string_view pKey);
    void disableHook(geode::Mod* pMod, std::string_view pKey);
    void conditionallyEnableHook(bool pEnable, geode::Mod* pMod, std::string_view pKey);
}