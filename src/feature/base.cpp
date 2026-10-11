#include "feature-manager.hpp"

using namespace geode::prelude;

namespace sillyedit::features {
    FeatureBase::FeatureBase(FeatureEnum pEnum) {
        m_id = getID(pEnum);
        
        FeatureManager::get()->registerFeature(this);
    }
    FeatureBase::~FeatureBase() {
        // see im a good girl i dont leak the settings even tho features only get destroyed on game close
        if (m_enabled) {
            delete m_enabled;
        }
    }

    void FeatureBase::setup() {}

    void FeatureBase::onEditor() {}
    void FeatureBase::onUpdate() {}
    void FeatureBase::onToggled(bool pEnabled) {}
    void FeatureBase::onUIUpdated(float pScale) {}
    void FeatureBase::onSettingChanged(std::string pName, nwo5::settings::GenericSetting* pSetting) {}

    geode::ZStringView FeatureBase::id() const {
        return m_id;
    }
    geode::ZStringView FeatureBase::keybindsEnabledKey() const {
        static const auto str = fmt::format("{}-Enabled", geode::utils::string::replace(m_id, " ", "-"));

        return str;
    }
    const FeatureBase::Data& FeatureBase::data() const {
        return m_data;
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

    void FeatureBase::init() {
        if (m_enabled) {
            return;
        }

        this->setup();

        m_enabled = new sillyedit::settings::SillySetting<bool>{"Enabled", m_id, m_data.defaultEnabled, m_data.reload, m_data.condition};

        nwo5::settings::listenForAllSavedSettingChanges([this] (std::string_view pKey, GenericSetting* pSetting) {
            if (!GameManager::get()->m_levelEditorLayer) {
                return;
            }

            const auto name = pSetting->name();
            
            this->onSettingChanged(name, pSetting);

            if (name == "Enabled") {
                this->onToggled(this->enabled());
            }
        }, m_id).leak();
    }
}