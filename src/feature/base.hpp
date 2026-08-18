#pragma once

#include <features/registry.hpp>

namespace Features {
    class FeatureBase {
    protected:
        std::string m_id;
        Settings::SillySetting<bool>* m_enabled;

        std::unordered_set<std::string> m_disabledBy;

    public:
        FeatureBase(FeatureEnum pEnum);

        // lol
        operator std::string_view() const {
            return m_id;
        }
        operator const std::string&() const {
            return m_id;
        }

        virtual void onEditor();

        virtual void onToggled(bool pEnabled);

        virtual void onUpdate();
        virtual void onSettingChanged(std::string pName, nwo5::settings::GenericSetting* pSetting);

        virtual void onUIUpdated(float pScale);

        geode::ZStringView id() const;

        void setEnabled(bool pEnabled, geode::Mod* pMod);
        bool enabled() const;
        bool forceDisabled() const;
    };

    template<FeatureEnum Enum, Settings::SettingCondition Condition = Settings::SettingCondition::None, Settings::SettingReload Reload = Settings::SettingReload::None,  bool DefaultEnabled = true>
    class FeatureTemplate : public FeatureBase {
    protected:
        FeatureTemplate()
            : FeatureBase(Enum) 
        {
            static Settings::SillySetting<bool> enabled{"Enabled", m_id, DefaultEnabled, Reload, Condition};
            m_enabled = &enabled;

            nwo5::settings::listenForAllSavedSettingChanges([this] (std::string_view pKey, GenericSetting* pSetting) {
                if (!GameManager::get()->m_levelEditorLayer) {
                    return;
                }

                if (const auto name = pSetting->name(); name != "Enabled") {
                    this->onSettingChanged(name, pSetting);
                }
                else {
                    this->onToggled(static_cast<Settings::SillySetting<bool>*>(pSetting)->get());
                }
            }, m_id).leak();
        }
    };
}