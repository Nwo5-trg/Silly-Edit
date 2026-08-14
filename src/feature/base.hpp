#pragma once

#include <settings/include.hpp>
#include <features/registry.hpp>

namespace Features {
    class FeatureBase {
    protected:
        std::string m_id;
        Settings::SillySetting<bool>* m_enabled;

        std::unordered_set<std::string> m_disabledBy;

    public:
        FeatureBase(FeatureEnum pEnum, bool pDefaultEnabled);

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

    template<FeatureEnum Enum, bool DefaultEnabled = true>
    class FeatureTemplate : public FeatureBase {
    protected:
        FeatureTemplate()
            : FeatureBase(Enum, DefaultEnabled) {}
    };
}