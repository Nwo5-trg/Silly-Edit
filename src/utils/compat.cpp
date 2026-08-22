#include <settings/include.hpp>
#include "include.hpp"

using namespace geode::prelude;

namespace Sillyedit {
    bool isBetterEditLoaded() {
        static bool loaded = static_cast<bool>(getBetterEdit());
        return loaded;
    }
    Mod* getBetterEdit() {
        static auto be = Loader::get()->getLoadedMod(BETTER_EDIT_ID);
        return be;
    }

    bool isTinkerLoaded() {
        static bool loaded = static_cast<bool>(getTinker());
        return loaded;
    }
    Mod* getTinker() {
        static auto tinker = Loader::get()->getLoadedMod(TINKER_EDIT_ID);
        return tinker;
    }

    void enableHook(geode::Mod* pMod, std::string_view pKey) {
        if (!pMod) {
            return;
        }

        for (auto hook : pMod->getHooks()) {
            if (hook->getDisplayName() == pKey) {
                (void)hook->enable();
                
                break;
            }
        }
    }
    void disableHook(geode::Mod* pMod, std::string_view pKey) {
        if (!pMod) {
            return;
        }

        for (auto hook : pMod->getHooks()) {
            if (hook->getDisplayName() == pKey) {
                (void)hook->disable();
                
                break;
            }
        }
    }
    void conditionallyEnableHook(bool pEnable, geode::Mod* pMod, std::string_view pKey) {
        if (pEnable) {
            enableHook(pMod, pKey);
        }
        else {
            disableHook(pMod, pKey);
        }
    }
}