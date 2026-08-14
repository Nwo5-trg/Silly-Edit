#include "feature-manager.hpp"

namespace Features {
    FeatureBase* FeatureManager::getFeature(geode::ZStringView pID) {
        const auto it = m_features.find(pID);

        if (it != m_features.end()) {
            return (*it).second;
        }

        return nullptr;
    }
    const geode::utils::StringMap<FeatureBase*>& FeatureManager::getFeatures() const {
        return m_features;
    }

    void FeatureManager::registerFeature(FeatureBase* pFeature) {
        m_features[pFeature->id()] = pFeature;
    }
}