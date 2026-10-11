#pragma once

#include "base.hpp"

namespace sillyedit::features {
    class FeatureManager {
    protected:
        geode::utils::StringMap<FeatureBase*> m_features;
        std::vector<FeatureBase*> m_queuedFeatures;
    
    public:
        static FeatureManager* get() {
            static FeatureManager inst;
            return &inst;
        }

        FeatureBase* getFeature(geode::ZStringView pID);
        const geode::utils::StringMap<FeatureBase*>& getFeatures() const;

        void registerFeature(FeatureBase* pFeature);
 
        void onEditorInit();
        void onEditorUpdate();

        void load();
    };
}