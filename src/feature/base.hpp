#pragma once

#include <features/registry.hpp>

namespace sillyedit::features {
    class FeatureBase {
    protected:
        struct Data {
            sillyedit::settings::SettingCondition condition = sillyedit::settings::SettingCondition::None;
            sillyedit::settings::SettingReload reload = sillyedit::settings::SettingReload::None;
            bool defaultEnabled = true;
            std::string incompatMessage;
            // we get a lil silly with our syntax sugar around here
            struct IncompatibleModsList{
                std::vector<std::string> list;

                IncompatibleModsList() = default;
                IncompatibleModsList(std::string pMod) {
                    list.push_back(std::move(pMod));
                }
                IncompatibleModsList(std::initializer_list<std::string> pMods) {
                    list = pMods;
                }
            }incompatMods;
            
            bool hasIncompats() const {
                return incompatMods.list.size();
            }
        } m_data;

        std::string m_id;
        sillyedit::settings::SillySetting<bool>* m_enabled = nullptr;
        std::unordered_set<std::string> m_disabledBy;

        virtual void setup();

        virtual void onEditor();
        virtual void onUpdate();
        /// also activates right after onEditor
        virtual void onToggled(bool pEnabled);
        virtual void onUIUpdated(float pScale);
        virtual void onSettingChanged(std::string pName, nwo5::settings::GenericSetting* pSetting);

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

        void init();

        friend class FeatureManager;
    public:
        FeatureBase(FeatureEnum pEnum);
        ~FeatureBase();

        // lol
        operator std::string_view() const {
            return m_id;
        }
        operator const std::string&() const {
            return m_id;
        }

        geode::ZStringView id() const;
        geode::ZStringView keybindsEnabledKey() const;
        const Data& data() const;

        void setEnabled(bool pEnabled, geode::Mod* pMod);
        bool enabled() const;
        bool forceDisabled() const;
    };

    // macro shenanigans so instead of constructor args i just need to pass template params
    template<FeatureEnum Enum>
    class FeatureTemplate : public FeatureBase {
    protected:
        FeatureTemplate()
            : FeatureBase(Enum) {}
    };
}