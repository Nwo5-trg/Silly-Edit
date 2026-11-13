#pragma once

#include <nwo5.silly-api/cache/include.hpp>

namespace sillyedit::settings {
    enum class SettingReload {
        /// dynamically toggled
        None,
        /// requires editor reload
        Editor,
        /// requires editor pause reload
        Pause,
        /// requires settings popup reload
        Popup,
        /// requires game reload
        Game
    };

    enum class SettingCondition {
        /// always show setting
        None,
        /// only show setting on desktop (if bool setting it gets automatically disabled on unsupported platform)
        DesktopOnly,
        /// only show setting on mobile (if bool setting it gets automatically disabled on unsupported platform)
        MobileOnly,
        /// only show setting on desktop or android but disable bool settings by default on android (if bool setting it gets automatically disabled on unsupported platform)
        DesktopAndAndroidDisable,
        /// show on everything but ios (if bool setting it gets automatically disabled on unsupported platform)
        IOSDisable
    };

    enum class SettingType {
        Bool,
        Int,
        Float,
        String,
        Strenum,
        RGB,
        RGBA,
        Opacity,
        Path
    };

    struct OpacityType {};
    enum class ModifierType {
        Shift,
        CtrlCommand,
        Alt,
        Ctrl,
        Command,
        None
    };
    constexpr auto modiferTypeToString(ModifierType type) {
        switch (type) {
            case ModifierType::Shift: return "Shift";
            case ModifierType::CtrlCommand: return "Ctrl+Command";
            case ModifierType::Alt: return "Alt";
            case ModifierType::Ctrl: return "Ctrl";
            case ModifierType::Command: return "Command";
            case ModifierType::None: return "None";
        }
    }

    class GenericSetting {
    public:
        virtual ~GenericSetting() = default;
        
        virtual const std::string& name() const = 0;
        virtual const std::string& key() const = 0;
        virtual const std::string& description() const = 0;
        virtual bool hasDescription() const = 0;
        virtual SettingReload reload() const = 0;
        virtual bool requiresReload() const = 0;
        virtual SettingCondition condition() const = 0;
        virtual SettingType type() const = 0;
        virtual bool validOnPlatform() const = 0;
    };

    template<typename T>
    class SettingBase : public GenericSetting, public nwo5::cache::ValueCache<T> {
    protected:
        std::string m_name;
        std::string m_description;
        SettingReload m_reload;
        SettingCondition m_condition;
        SettingType m_type;
        bool m_validOnPlatform;

        SettingBase(
            std::string name,
            std::optional<std::string> description,
            SettingReload reload,
            SettingCondition condition,
            SettingType type,
            std::string category,
            T defaultValue
        );
        ~SettingBase();

    public:
        const std::string& name() const {
            return m_name;
        }
        const std::string& key() const {
           return this->m_key; 
        }
        const std::string& description() const {
            return m_description;
        }
        bool hasDescription() const {
            return m_description.size();
        }
        SettingReload reload() const {
            return m_reload;
        }
        bool requiresReload() const {
            return m_reload != SettingReload::None;
        }
        SettingCondition condition() const {
            return m_condition;
        }
        SettingType type() const {
            return m_type;
        }
        bool validOnPlatform() const {
            return m_validOnPlatform;
        }
    };

    std::string generateKey(std::string key);
}