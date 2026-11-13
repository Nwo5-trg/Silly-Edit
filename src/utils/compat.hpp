#pragma once

#include <nwo5.silly-api/include.hpp>

namespace sillyedit::utils {
    inline const std::string BETTER_EDIT_ID{"hjfod.betteredit"};
    bool isBetterEditLoaded();
    geode::Mod* getBetterEdit();
    template<typename T>
    std::optional<T> getBetterEditSetting(std::string_view key) {
        auto be = getBetterEdit();

        if (!be) {
            return std::nullopt;
        }

        return be->getSettingValue<T>(key);
    }
    template<typename T>
    bool setBetterEditSetting(std::string_view key, T val) {
        auto be = getBetterEdit();

        if (!be) {
            return false;
        }

        be->setSettingValue<T>(key, val);

        return true;
    }
    
    inline const std::string TINKER_EDIT_ID{"alphalaneous.tinker"};
    bool isTinkerLoaded();
    geode::Mod* getTinker();
    template<typename T>
    std::optional<T> getTinkerSetting(std::string_view key) {
        auto tinker = getTinker();

        if (!tinker) {
            return std::nullopt;
        }

        return tinker->getSettingValue<T>(key);
    }
    template<typename T>
    bool setTinkerSetting(std::string_view key, T val) {
        auto tinker = getTinker();

        if (!tinker) {
            return false;
        }

        tinker->setSettingValue<T>(key, val);

        return true;
    }

    void enableHook(geode::Mod* mod, std::string_view key);
    void disableHook(geode::Mod* mod, std::string_view key);
    void conditionallyEnableHook(bool enable, geode::Mod* mod, std::string_view key);
}