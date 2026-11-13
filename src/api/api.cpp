#include "../../../include.hpp"
#include <feature/include.hpp>

using namespace geode::prelude;

namespace nwo5::sillyedit {
    void toggleFeature(geode::ZStringView feature, bool enable, Mod* mod) {
        if (auto feature = FeatureManager::get()->getFeature(feature)) {
            feature->setEnabled(enable, mod);
        }
    }

    bool featureEnabled(geode::ZStringView feature) {
        if (auto feature = FeatureManager::get()->getFeature(feature)) {
            return feature->enabled();
        }

        return false;
    }
    bool featureForceDisabled(geode::ZStringView feature) {
        if (auto feature = FeatureManager::get()->getFeature(feature)) {
            return feature->forceDisabled();
        }

        return false;
    }
};