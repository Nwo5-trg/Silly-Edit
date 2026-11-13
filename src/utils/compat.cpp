#include <settings/include.hpp>
#include "include.hpp"

using namespace geode::prelude;

namespace sillyedit::utils {
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

    void enableHook(geode::Mod* mod, std::string_view key) {
        if (!mod) {
            return;
        }

        for (auto hook : mod->getHooks()) {
            if (hook->getDisplayName() == key) {
                (void)hook->enable();
                
                break;
            }
        }
    }
    void disableHook(geode::Mod* mod, std::string_view key) {
        if (!mod) {
            return;
        }

        for (auto hook : mod->getHooks()) {
            if (hook->getDisplayName() == key) {
                (void)hook->disable();
                
                break;
            }
        }
    }
    void conditionallyEnableHook(bool enable, geode::Mod* mod, std::string_view key) {
        if (enable) {
            enableHook(mod, key);
        }
        else {
            disableHook(mod, key);
        }
    }
}