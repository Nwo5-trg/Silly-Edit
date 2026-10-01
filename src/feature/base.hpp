#pragma once

#include <features/registry.hpp>

namespace sillyedit::features {
    class FeatureBase {
    protected:
        std::string m_id;
        sillyedit::settings::SillySetting<bool>* m_enabled;

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

        /// also activates right after onEditor
        virtual void onToggled(bool pEnabled);

        virtual void onUpdate();
        virtual void onSettingChanged(std::string pName, nwo5::settings::GenericSetting* pSetting);

        virtual void onUIUpdated(float pScale);

        geode::ZStringView id() const;

        void setEnabled(bool pEnabled, geode::Mod* pMod);
        bool enabled() const;
        bool forceDisabled() const;

        /// listen to keybind, callback doesnt activate if feature or feature keybinds disabled
        template<geode::utils::string::ConstexprString Keybind, typename Callback>
        void registerKeybind(Callback&& pCallback, cocos2d::CCNode* pNode = LevelEditorLayer::get(), int pPriority = geode::Priority::Normal) {
            const static auto enabledkey = fmt::format("{}-Enabled", geode::utils::string::replace(m_id, " ", "-"));

            pNode->addEventListener(geode::KeybindSettingPressedEvent(geode::Mod::get(), fmt::format("{}-{}", m_id, std::string{Keybind.data()})), 
            [this, callback = std::move(pCallback)] (const geode::Keybind&, bool pDown, bool pRepeat, double) {
                if (this->enabled() && geode::Mod::get()->getSettingValue<bool>(enabledkey)) {
                    callback(pDown, pRepeat);
                }
            }, pPriority);
        }
    };

    // macro shenanigans so instead of constructor args i just need to pass template params
    template<FeatureEnum Enum, sillyedit::settings::SettingCondition Condition = sillyedit::settings::SettingCondition::None, sillyedit::settings::SettingReload Reload = sillyedit::settings::SettingReload::None,  bool DefaultEnabled = true>
    class FeatureTemplate : public FeatureBase {
    protected:
        FeatureTemplate()
            : FeatureBase(Enum) 
        {
            // technically i could just leak the setting and do this in featurebase constuctor, but this works too !!!
            static sillyedit::settings::SillySetting<bool> enabled{"Enabled", m_id, DefaultEnabled, Reload, Condition};
            m_enabled = &enabled;

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
    };
}