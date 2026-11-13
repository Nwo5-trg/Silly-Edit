#include <utils/include.hpp>
#include "feature-manager.hpp"

using namespace geode::prelude;

namespace sillyedit::features {
    FeatureBase::FeatureBase(FeatureEnum enum) {
        m_id = enum;
        
        FeatureManager::get()->registerFeature(this);
    }

    void FeatureBase::setup() {}

    void FeatureBase::onEditor() {}
    void FeatureBase::onUpdate() {}
    void FeatureBase::onToggled(bool enabled) {}
    void FeatureBase::onUIUpdated(float scale) {}
    void FeatureBase::onSettingChanged(std::string name, settings::GenericSetting* setting) {}

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

    void FeatureBase::setEnabled(bool enabled, geode::Mod* mod) {
        if (enabled) {
            m_disabledBy.erase(mod->getID());
        }
        else {
            m_disabledBy.insert(mod->getID());
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

        m_settingListener = nwo5::cache::ValueCacheChanged().listen([this] (Mod* mod, std::string key, void* ptr) {
            if (mod != Mod::get() || editor::notLoaded()) {
                return;
            }

            auto setting = static_cast<settings::GenericSetting*>(ptr);

            this->onSettingChanged(setting->name(), setting);
        });

        m_enabled = std::make_unique<settings::Setting<bool>>(
            "Enabled", m_id, m_data.defaultEnabled, m_data.reload, m_data.condition
        );
        // just to make sure onsettingchanged gets activated first
        m_enabledListener = m_enabled->listen([this] (nwo5::cache::ValueCache<bool>*) {
            this->onToggled(this->enabled());
        }, Priority::Normal + 1);
    }
}