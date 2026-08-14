#include "../../../include/include.hpp"
#include <feature/include.hpp>

using namespace geode::prelude;

namespace nwo5::sillyedit {
    void toggleFeature(geode::ZStringView pFeature, bool pEnable, Mod* pMod) {
        if (auto feature = FeatureManager::get()->getFeature(pFeature)) {
            feature->setEnabled(pEnable, pMod);
        }
    }

    bool featureEnabled(geode::ZStringView pFeature) {
        if (auto feature = FeatureManager::get()->getFeature(pFeature)) {
            return feature->enabled();
        }

        return false;
    }
    bool featureForceDisabled(geode::ZStringView pFeature) {
        if (auto feature = FeatureManager::get()->getFeature(pFeature)) {
            return feature->forceDisabled();
        }

        return false;
    }
};