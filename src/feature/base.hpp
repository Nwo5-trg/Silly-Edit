#pragma once

#include <features/registry.hpp>

namespace sillyedit::features {
    class FeatureBase {
    protected:
        struct Data {
            settings::SettingCondition condition = settings::SettingCondition::None;
            settings::SettingReload reload = settings::SettingReload::None;
            bool defaultEnabled = true;
            std::string incompatMessage;
            // we get a lil silly with our syntax sugar around here
            struct IncompatibleModsList{
                std::vector<std::string> list;

                IncompatibleModsList() = default;
                IncompatibleModsList(std::string mod) {
                    list.push_back(std::move(mod));
                }
                IncompatibleModsList(std::initializer_list<std::string> mods) {
                    list = mods;
                }
            }incompatMods;
            
            bool hasIncompats() const {
                return incompatMods.list.size();
            }
        } m_data;

        std::string m_id;
        std::unique_ptr<settings::Setting<bool>> m_enabled;
        std::unordered_set<std::string> m_disabledBy;
        geode::ListenerHandle m_settingListener;
        geode::ListenerHandle m_enabledListener;

        virtual void setup();

        virtual void onEditor();
        virtual void onUpdate();
        /// also activates right after onEditor
        virtual void onToggled(bool enabled);
        virtual void onUIUpdated(float scale);
        virtual void onSettingChanged(std::string name, settings::GenericSetting* setting);

        /// listen to keybind, callback doesnt activate if feature or feature keybinds disabled
        template<geode::utils::string::ConstexprString Keybind, typename Callback>
        void registerKeybind(Callback&& callback, cocos2d::CCNode* node = LevelEditorLayer::get(), int priority = geode::Priority::Normal) {
            const static auto enabledkey = fmt::format("{}-Enabled", geode::utils::string::replace(m_id, " ", "-"));

            node->addEventListener(geode::KeybindSettingPressedEvent(geode::Mod::get(), fmt::format("{}-{}", m_id, std::string{Keybind.data()})), 
            [this, callback = std::move(callback)] (const geode::Keybind&, bool down, bool repeat, double) {
                if (this->enabled() && geode::Mod::get()->getSettingValue<bool>(enabledkey)) {
                    callback(down, repeat);
                }
            }, priority);
        }

        void init();

        friend class FeatureManager;
    public:
        FeatureBase(FeatureEnum enum);
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

        void setEnabled(bool enabled, geode::Mod* mod);
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