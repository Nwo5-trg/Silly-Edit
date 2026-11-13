#pragma once

#include "settings-manager.hpp"

namespace sillyedit::settings {
    template<typename T>
    SettingBase<T>::SettingBase(
        std::string name,
        std::optional<std::string> description,
        SettingReload reload,
        SettingCondition condition,
        SettingType type,
        std::string category,
        T defaultValue
    ) 
        : nwo5::cache::ValueCache<T>(generateKey(name), defaultValue), m_name(name), m_description(description.value_or("")),
          m_reload(reload), m_condition(condition), m_type(type)
    {
        bool enableBools = true;

        switch (condition) {
            case SettingCondition::None: break;
            case SettingCondition::DesktopOnly: {
                #if !defined(GEODE_IS_DESKTOP)
                    enableBools = m_validOnPlatform = false;
                #endif
            break; }
            case SettingCondition::MobileOnly: {
                #if !defined(GEODE_IS_MOBILE)
                    enableBools = m_validOnPlatform = false;
                #endif
            break; }
            case SettingCondition::DesktopAndAndroidDisable: {
                #if !defined(GEODE_IS_DESKTOP)
                    enableBools = m_validOnPlatform = false;
                #elif defined(GEODE_IS_ANDROID)
                    enableBools = false;
                #endif
            break; }
            case SettingCondition::IOSDisable: {
                #if defined(GEODE_IS_IOS)
                    enableBools = m_validOnPlatform = false;
                #endif
            break; }
        }

        if constexpr (std::same_as<T, bool>) {
            if (!enableBools) {
                this->set(false);
            }
        }

        SettingsManager::get()->registerSetting(category, this);
    }
    template<typename T>
    SettingBase<T>::~SettingBase() {
        SettingsManager::get()->unregisterSetting(this);
    }

    template<typename T>
    class Setting;

    template<>
    class Setting<bool> : public SettingBase<bool> {
    public:
        Setting(std::string name, std::string category, bool defaultValue, std::optional<std::string> description = std::nullopt)
            : SettingBase(std::move(name), std::move(description), SettingReload::None, SettingCondition::None, SettingType::Bool, std::move(category), defaultValue) {}
        Setting(std::string name, std::string category, bool defaultValue, SettingReload reloadRequired, std::optional<std::string> description = std::nullopt)
            : SettingBase(std::move(name), std::move(description), SettingReload::None, SettingCondition::None, SettingType::Bool, std::move(category), defaultValue) {}
        Setting(std::string name, std::string category, bool defaultValue, SettingCondition condition, std::optional<std::string> description = std::nullopt)
            : SettingBase(std::move(name), std::move(description), SettingReload::None, SettingCondition::None, SettingType::Bool, std::move(category), defaultValue) {}
        Setting(std::string name, std::string category, bool defaultValue, SettingReload reloadRequired, SettingCondition condition, std::optional<std::string> description = std::nullopt)
            : SettingBase(std::move(name), std::move(description), SettingReload::None, SettingCondition::None, SettingType::Bool, std::move(category), defaultValue) {}
    };

    template<typename T>
    requires std::same_as<T, int> || std::same_as<T, float>
    class Setting<T> : public SettingBase<T> {
    public:
        Setting(std::string name, std::string category, T defaultValue, std::optional<std::string> description = std::nullopt)
            : SettingBase<T>(std::move(name), std::move(description), SettingReload::None, SettingCondition::None, (std::same_as<T, int> ? SettingType::Int : SettingType::Float), std::move(category), defaultValue) {}
        Setting(std::string name, std::string category, T defaultValue, SettingReload reloadRequired, std::optional<std::string> description = std::nullopt)
            : SettingBase<T>(std::move(name), std::move(description), SettingReload::None, SettingCondition::None, (std::same_as<T, int> ? SettingType::Int : SettingType::Float), std::move(category), defaultValue) {}
        Setting(std::string name, std::string category, T defaultValue, SettingCondition condition, std::optional<std::string> description = std::nullopt)
            : SettingBase<T>(std::move(name), std::move(description), SettingReload::None, SettingCondition::None, (std::same_as<T, int> ? SettingType::Int : SettingType::Float), std::move(category), defaultValue) {}
        Setting(std::string name, std::string category, T defaultValue, SettingReload reloadRequired, SettingCondition condition, std::optional<std::string> description = std::nullopt)
            : SettingBase<T>(std::move(name), std::move(description), SettingReload::None, SettingCondition::None, (std::same_as<T, int> ? SettingType::Int : SettingType::Float), std::move(category), defaultValue) {}
    };

    template<>
    class Setting<float> : public SettingBase<float> {
    public:
        Setting(std::string name, std::string category, float defaultValue, std::optional<std::string> description = std::nullopt)
            : SettingBase(std::move(name), std::move(description), SettingReload::None, SettingCondition::None, SettingType::Float, std::move(category), defaultValue) {}
        Setting(std::string name, std::string category, float defaultValue, SettingReload reloadRequired, std::optional<std::string> description = std::nullopt)
            : SettingBase(std::move(name), std::move(description), SettingReload::None, SettingCondition::None, SettingType::Float, std::move(category), defaultValue) {}
        Setting(std::string name, std::string category, float defaultValue, SettingCondition condition, std::optional<std::string> description = std::nullopt)
            : SettingBase(std::move(name), std::move(description), SettingReload::None, SettingCondition::None, SettingType::Float, std::move(category), defaultValue) {}
        Setting(std::string name, std::string category, float defaultValue, SettingReload reloadRequired, SettingCondition condition, std::optional<std::string> description = std::nullopt)
            : SettingBase(std::move(name), std::move(description), SettingReload::None, SettingCondition::None, SettingType::Float, std::move(category), defaultValue) {}
    };

    template<>
    class Setting<std::string> : public SettingBase<std::string> {
    protected:
        std::vector<std::string> m_strenumValues;

    public:
        Setting(std::string name, std::string category, std::string defaultValue, std::optional<std::string> description = std::nullopt)
            : SettingBase(std::move(name), std::move(description), SettingReload::None, SettingCondition::None, SettingType::String, std::move(category), std::move(defaultValue)) {}
        Setting(std::string name, std::string category, std::string defaultValue, SettingReload reloadRequired, std::optional<std::string> description = std::nullopt)
            : SettingBase(std::move(name), std::move(description), SettingReload::None, SettingCondition::None, SettingType::String, std::move(category), std::move(defaultValue)) {}
        Setting(std::string name, std::string category, std::string defaultValue, SettingCondition condition, std::optional<std::string> description = std::nullopt)
            : SettingBase(std::move(name), std::move(description), SettingReload::None, SettingCondition::None, SettingType::String, std::move(category), std::move(defaultValue)) {}
        Setting(std::string name, std::string category, std::string defaultValue, SettingReload reloadRequired, SettingCondition condition, std::optional<std::string> description = std::nullopt)
            : SettingBase(std::move(name), std::move(description), SettingReload::None, SettingCondition::None, SettingType::String, std::move(category), std::move(defaultValue)) {}
        Setting(std::string name, std::string category, std::string defaultValue, std::vector<std::string> strenumValues, std::optional<std::string> description = std::nullopt)
            : SettingBase(std::move(name), std::move(description), SettingReload::None, SettingCondition::None, SettingType::String, std::move(category), std::move(defaultValue)), m_strenumValues(std::move(strenumValues)) {}
        Setting(std::string name, std::string category, std::string defaultValue, std::vector<std::string> strenumValues, SettingReload reloadRequired, std::optional<std::string> description = std::nullopt)
            : SettingBase(std::move(name), std::move(description), SettingReload::None, SettingCondition::None, SettingType::String, std::move(category), std::move(defaultValue)), m_strenumValues(std::move(strenumValues)) {}
        Setting(std::string name, std::string category, std::string defaultValue, std::vector<std::string> strenumValues, SettingCondition condition, std::optional<std::string> description = std::nullopt)
            : SettingBase(std::move(name), std::move(description), SettingReload::None, SettingCondition::None, SettingType::String, std::move(category), std::move(defaultValue)), m_strenumValues(std::move(strenumValues)) {}
        Setting(std::string name, std::string category, std::string defaultValue, std::vector<std::string> strenumValues, SettingReload reloadRequired, SettingCondition condition, std::optional<std::string> description = std::nullopt)
            : SettingBase(std::move(name), std::move(description), SettingReload::None, SettingCondition::None, SettingType::String, std::move(category), std::move(defaultValue)), m_strenumValues(std::move(strenumValues)) {}

        const auto& strenumValues() const {
            return m_strenumValues;
        }
        bool isStrenum() const {
            return m_strenumValues.size();
        }
    };

    template<>
    class Setting<cocos2d::ccColor3B> : public SettingBase<cocos2d::ccColor3B> {
    public:
        Setting(std::string name, std::string category, cocos2d::ccColor3B defaultValue, std::optional<std::string> description = std::nullopt)
            : SettingBase(std::move(name), std::move(description), SettingReload::None, SettingCondition::None, SettingType::RGB, std::move(category), defaultValue) {}
        Setting(std::string name, std::string category, cocos2d::ccColor3B defaultValue, SettingReload reloadRequired, std::optional<std::string> description = std::nullopt)
            : SettingBase(std::move(name), std::move(description), SettingReload::None, SettingCondition::None, SettingType::RGB, std::move(category), defaultValue) {}
        Setting(std::string name, std::string category, cocos2d::ccColor3B defaultValue, SettingCondition condition, std::optional<std::string> description = std::nullopt)
            : SettingBase(std::move(name), std::move(description), SettingReload::None, SettingCondition::None, SettingType::RGB, std::move(category), defaultValue) {}
        Setting(std::string name, std::string category, cocos2d::ccColor3B defaultValue, SettingReload reloadRequired, SettingCondition condition, std::optional<std::string> description = std::nullopt)
            : SettingBase(std::move(name), std::move(description), SettingReload::None, SettingCondition::None, SettingType::RGB, std::move(category), defaultValue) {}
    };

    template<>
    class Setting<cocos2d::ccColor4B> : public SettingBase<cocos2d::ccColor4B> {
    public:
        Setting(std::string name, std::string category, cocos2d::ccColor4B defaultValue, std::optional<std::string> description = std::nullopt)
            : SettingBase(std::move(name), std::move(description), SettingReload::None, SettingCondition::None, SettingType::RGBA, std::move(category), defaultValue) {}
        Setting(std::string name, std::string category, cocos2d::ccColor4B defaultValue, SettingReload reloadRequired, std::optional<std::string> description = std::nullopt)
            : SettingBase(std::move(name), std::move(description), SettingReload::None, SettingCondition::None, SettingType::RGBA, std::move(category), defaultValue) {}
        Setting(std::string name, std::string category, cocos2d::ccColor4B defaultValue, SettingCondition condition, std::optional<std::string> description = std::nullopt)
            : SettingBase(std::move(name), std::move(description), SettingReload::None, SettingCondition::None, SettingType::RGBA, std::move(category), defaultValue) {}
        Setting(std::string name, std::string category, cocos2d::ccColor4B defaultValue, SettingReload reloadRequired, SettingCondition condition, std::optional<std::string> description = std::nullopt)
            : SettingBase(std::move(name), std::move(description), SettingReload::None, SettingCondition::None, SettingType::RGBA, std::move(category), defaultValue) {}
    };

    template<>
    class Setting<ModifierType> : public Setting<std::string> {
    public:
        Setting(std::string name, std::string category, ModifierType defaultValue, std::optional<std::string> description = std::nullopt)
            : Setting<std::string>(std::move(name), std::move(category), modiferTypeToString(defaultValue), {"Shift", "Ctrl+Command", "Alt", "Ctrl", "Command", "None"}, std::move(description)) {}
        Setting(std::string name, std::string category, ModifierType defaultValue, SettingReload reloadRequired, std::optional<std::string> description = std::nullopt)
            : Setting<std::string>(std::move(name), std::move(category), modiferTypeToString(defaultValue), {"Shift", "Ctrl+Command", "Alt", "Ctrl", "Command", "None"}, reloadRequired, std::move(description)) {}
        Setting(std::string name, std::string category, ModifierType defaultValue, SettingCondition condition, std::optional<std::string> description = std::nullopt)
            : Setting<std::string>(std::move(name), std::move(category), modiferTypeToString(defaultValue), {"Shift", "Ctrl+Command", "Alt", "Ctrl", "Command", "None"}, condition, std::move(description)) {}
        Setting(std::string name, std::string category, ModifierType defaultValue, SettingReload reloadRequired, SettingCondition condition, std::optional<std::string> description = std::nullopt)
            : Setting<std::string>(std::move(name), std::move(category), modiferTypeToString(defaultValue), {"Shift", "Ctrl+Command", "Alt", "Ctrl", "Command", "None"}, reloadRequired, condition, std::move(description)) {}
    };

    // template<>
    // class Setting<OpacityType> : public Setting<int> {
    // public:
    //     Setting(std::string name, std::string category, unsigned char defaultValue, std::optional<std::string> description = std::nullopt)
    //         : Setting<int>(std::move(name), std::move(category), defaultValue, {0, 255}, std::move(description)) {}
    //     Setting(std::string name, std::string category, unsigned char defaultValue, SettingReload reloadRequired, std::optional<std::string> description = std::nullopt)
    //         : Setting<int>(std::move(name), std::move(category), defaultValue, {0, 255}, reloadRequired, std::move(description)) {}
    //     Setting(std::string name, std::string category, unsigned char defaultValue, SettingCondition condition, std::optional<std::string> description = std::nullopt)
    //         : Setting<int>(std::move(name), std::move(category), defaultValue, {0, 255}, condition, std::move(description)) {}
    //     Setting(std::string name, std::string category, unsigned char defaultValue, SettingReload reloadRequired, SettingCondition condition, std::optional<std::string> description = std::nullopt)
    //         : Setting<int>(std::move(name), std::move(category),defaultValue, {0, 255}, reloadRequired, condition, std::move(description)) {}
    // };
}