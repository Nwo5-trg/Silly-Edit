#include "feature-manager.hpp"

namespace Features {
    FeatureBase::FeatureBase(FeatureEnum pEnum) {
        m_id = getID(pEnum);
        
        FeatureManager::get()->registerFeature(this);
    }

    void FeatureBase::onEditor() {}

    void FeatureBase::onToggled(bool pEnabled) {};

    void FeatureBase::onUpdate() {}
    void FeatureBase::onSettingChanged(std::string pName, nwo5::settings::GenericSetting* pSetting) {}

    void FeatureBase::onUIUpdated(float pScale) {}

    geode::ZStringView FeatureBase::id() const {
        return m_id;
    }

    void FeatureBase::setEnabled(bool pEnabled, geode::Mod* pMod) {
        if (pEnabled) {
            m_disabledBy.erase(pMod->getID());
        }
        else {
            m_disabledBy.insert(pMod->getID());
        }

        this->onToggled(this->enabled());
    }
    bool FeatureBase::enabled() const {
        return m_enabled->get() && m_disabledBy.empty();
    }
    bool FeatureBase::forceDisabled() const {
        return !m_disabledBy.empty();
    }
}