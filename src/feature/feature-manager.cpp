#include <utils/include.hpp>
#include "feature-manager.hpp"

using namespace geode::prelude;

namespace sillyedit::features {
    FeatureBase* FeatureManager::getFeature(geode::ZStringView iD) {
        const auto it = m_features.find(iD);

        if (it != m_features.end()) {
            return (*it).second;
        }

        return nullptr;
    }
    const geode::utils::StringMap<FeatureBase*>& FeatureManager::getFeatures() const {
        return m_features;
    }

    void FeatureManager::registerFeature(FeatureBase* feature) {
        if (!std::ranges::contains(m_queuedFeatures, feature)) {
            m_queuedFeatures.push_back(feature);
        }
    }

    void FeatureManager::onEditorInit() {
        for (auto [_, feature] : sillyedit::features::FeatureManager::get()->getFeatures()) {
            feature->onEditor();
        }

        for (auto [_, feature] : sillyedit::features::FeatureManager::get()->getFeatures()) {
            feature->onToggled(feature->enabled());
        }

        for (auto [_, feature] : sillyedit::features::FeatureManager::get()->getFeatures()) {
            feature->onUIUpdated(1.0f);
        }
    }
    void FeatureManager::onEditorUpdate() {
        for (auto [_, feature] : m_features) {
            feature->onUpdate();
        }
    }

    void FeatureManager::load() {
        for (auto feature : m_queuedFeatures) {
            m_features[feature->id()] = feature;
            
            feature->init();
        }

        m_queuedFeatures.clear();
    }
}