#include "base.hpp"

namespace Features {
    FeatureBase::FeatureBase(FeatureEnum pEnum, bool pDefaultEnabled) {
        m_id = getID(pEnum);

        static Settings::SillySetting<bool> enabled{"Enabled", m_id, pDefaultEnabled};
        m_enabled = &enabled;

        nwo5::settings::listenForAllSavedSettingChanges([this] (std::string_view pKey, GenericSetting* pSetting) {
            if (const auto name = pSetting->name(); name != "Enabled") {
                this->onSettingChanged(name, pSetting);
            }
            else {
                this->onToggled(static_cast<Settings::SillySetting<bool>*>(pSetting)->get());
            }
        }, m_id).leak();
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
    }
    bool FeatureBase::enabled() const {
        return m_enabled->get() && m_disabledBy.empty();
    }
    bool FeatureBase::forceDisabled() const {
        return !m_disabledBy.empty();
    }
}