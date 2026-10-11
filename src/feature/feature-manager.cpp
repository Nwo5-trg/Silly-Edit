#include <nwo5.ui-scaling/include/compat.hpp>
#include <utils/include.hpp>
#include "feature-manager.hpp"

using namespace geode::prelude;
using namespace nwo5::uiscaling::prelude;

namespace sillyedit::features {
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
        if (!std::ranges::contains(m_queuedFeatures, pFeature)) {
            m_queuedFeatures.push_back(pFeature);
        }
    }

    void FeatureManager::onEditorInit() {
        for (auto [_, feature] : sillyedit::features::FeatureManager::get()->getFeatures()) {
            feature->onEditor();
        }

        for (auto [_, feature] : sillyedit::features::FeatureManager::get()->getFeatures()) {
            feature->onToggled(feature->enabled());
        }

        editor::ui()->addEventListener(uiscaling::compat::EditorUI::Changed(), [] (float pScale, auto) {
            for (auto [_, feature] : sillyedit::features::FeatureManager::get()->getFeatures()) {
                feature->onUIUpdated(pScale);
            }
        });
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