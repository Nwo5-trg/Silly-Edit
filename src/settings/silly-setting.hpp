#pragma once

#include <nwo5.silly-api/include/settings/include.hpp>

using namespace nwo5::settings::prelude;

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
    
    struct SettingWithReloadChanged final : geode::Event<SettingWithReloadChanged, bool(std::string pID, SettingReload pReload)> {
        using Event::Event;
    };

    template<typename T>
    class SillySettingBase : public SavedSetting<T>{
    protected:
        SettingReload m_editorReloadRequired;
        bool m_validOnPlatform = true;

    public:
    template<typename... Args>
        SillySettingBase(SettingReload pReloadRequired, SettingCondition pCondition, std::string pName, std::string pCategory, Args... pArgs)
            : SavedSetting<T>(pName, pCategory, nwo5::settings::generateKey(pName, pCategory), std::forward<Args>(pArgs)...), m_editorReloadRequired(pReloadRequired)
        {
            bool enableBools = true;

            switch (pCondition) {
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

            if constexpr (nwo5::settings::getSettingType<T>() == static_cast<int>(SettingType::Bool)) {
                if (!enableBools) {
                    this->set(false);
                }
            }
        }

        SettingReload reloadType() const {
            return m_editorReloadRequired;
        }
        bool reloadRequired() const {
            return m_editorReloadRequired != SettingReload::None;
        }

        bool validOnPlatform() {
            return m_validOnPlatform;
        }
    };

    template<typename T>
    class SillySetting : public SillySettingBase<T> {
    public:
        SillySetting(std::string pName, std::string pCategory, T pDefault, std::optional<std::string> pDescription = std::nullopt)
            : SillySettingBase<T>(SettingReload::None, SettingCondition::None, std::move(pName), std::move(pCategory), std::move(pDefault), std::move(pDescription)) {}
        SillySetting(std::string pName, std::string pCategory, T pDefault, SettingReload pReloadRequired, std::optional<std::string> pDescription = std::nullopt)
            : SillySettingBase<T>(pReloadRequired, SettingCondition::None, std::move(pName), std::move(pCategory), std::move(pDefault), std::move(pDescription)) {}
        SillySetting(std::string pName, std::string pCategory, T pDefault, SettingCondition pCondition, std::optional<std::string> pDescription = std::nullopt)
            : SillySettingBase<T>(SettingReload::None, pCondition, std::move(pName), std::move(pCategory), std::move(pDefault), std::move(pDescription)) {}
        SillySetting(std::string pName, std::string pCategory, T pDefault, SettingReload pReloadRequired, SettingCondition pCondition, std::optional<std::string> pDescription = std::nullopt)
            : SillySettingBase<T>(pReloadRequired, pCondition, std::move(pName), std::move(pCategory), std::move(pDefault), std::move(pDescription)) {}
    };

    template<IsStringSettingType T>
    class SillySetting<T> : public SillySettingBase<T> {
    public:
        SillySetting(std::string pName, std::string pCategory, T pDefault, std::optional<std::string> pDescription = std::nullopt)
            : SillySettingBase<T>(SettingReload::None, SettingCondition::None, std::move(pName), std::move(pCategory), std::move(pDefault), std::vector<T>{}, std::move(pDescription)) {}
        SillySetting(std::string pName, std::string pCategory, T pDefault, SettingReload pReloadRequired, std::optional<std::string> pDescription = std::nullopt)
            : SillySettingBase<T>(pReloadRequired, SettingCondition::None, std::move(pName), std::move(pCategory), std::move(pDefault), std::vector<T>{}, std::move(pDescription)) {}
        SillySetting(std::string pName, std::string pCategory, T pDefault, SettingCondition pCondition, std::optional<std::string> pDescription = std::nullopt)
            : SillySettingBase<T>(SettingReload::None, pCondition, std::move(pName), std::move(pCategory), std::move(pDefault), std::vector<T>{}, std::move(pDescription)) {}
        SillySetting(std::string pName, std::string pCategory, T pDefault, SettingReload pReloadRequired, SettingCondition pCondition, std::optional<std::string> pDescription = std::nullopt)
            : SillySettingBase<T>(pReloadRequired, pCondition, std::move(pName), std::move(pCategory), std::move(pDefault), std::vector<T>{}, std::move(pDescription)) {}
        SillySetting(std::string pName, std::string pCategory, T pDefault, std::vector<T> pOptions, std::optional<std::string> pDescription = std::nullopt)
            : SillySettingBase<T>(SettingReload::None, SettingCondition::None, std::move(pName), std::move(pCategory), std::move(pDefault), std::move(pOptions), std::move(pDescription)) {}
        SillySetting(std::string pName, std::string pCategory, T pDefault, std::vector<T> pOptions, SettingReload pReloadRequired, std::optional<std::string> pDescription = std::nullopt)
            : SillySettingBase<T>(pReloadRequired, SettingCondition::None, std::move(pName), std::move(pCategory), std::move(pDefault), std::move(pOptions), std::move(pDescription)) {}
        SillySetting(std::string pName, std::string pCategory, T pDefault, std::vector<T> pOptions, SettingCondition pCondition, std::optional<std::string> pDescription = std::nullopt)
            : SillySettingBase<T>(SettingReload::None, pCondition, std::move(pName), std::move(pCategory), std::move(pDefault), std::move(pOptions), std::move(pDescription)) {}
        SillySetting(std::string pName, std::string pCategory, T pDefault, std::vector<T> pOptions, SettingReload pReloadRequired, SettingCondition pCondition, std::optional<std::string> pDescription = std::nullopt)
            : SillySettingBase<T>(pReloadRequired, pCondition, std::move(pName), std::move(pCategory), std::move(pDefault), std::move(pOptions), std::move(pDescription)) {}
    };

    template<IsNumberSettingType T>
    class SillySetting<T> : public SillySettingBase<T> {
    public:
        SillySetting(std::string pName, std::string pCategory, T pDefault, std::optional<std::string> pDescription = std::nullopt)
            : SillySettingBase<T>(SettingReload::None, SettingCondition::None, std::move(pName), std::move(pCategory), std::move(pDefault), typename SillySetting::Range{}, std::nullopt, std::move(pDescription)) {}
        SillySetting(std::string pName, std::string pCategory, T pDefault, SettingReload pReloadRequired, std::optional<std::string> pDescription = std::nullopt)
            : SillySettingBase<T>(pReloadRequired, SettingCondition::None, std::move(pName), std::move(pCategory), std::move(pDefault), typename SillySetting::Range{}, std::nullopt, std::move(pDescription)) {}
        SillySetting(std::string pName, std::string pCategory, T pDefault, SettingCondition pCondition, std::optional<std::string> pDescription = std::nullopt)
            : SillySettingBase<T>(SettingReload::None, pCondition, std::move(pName), std::move(pCategory), std::move(pDefault), typename SillySetting::Range{}, std::nullopt, std::move(pDescription)) {}
        SillySetting(std::string pName, std::string pCategory, T pDefault, SettingReload pReloadRequired, SettingCondition pCondition, std::optional<std::string> pDescription = std::nullopt)
            : SillySettingBase<T>(pReloadRequired, pCondition, std::move(pName), std::move(pCategory), std::move(pDefault), typename SillySetting::Range{}, std::nullopt, std::move(pDescription)) {}
        SillySetting(std::string pName, std::string pCategory, T pDefault, typename SillySetting::Range pRange, std::optional<std::string> pDescription = std::nullopt)
            : SillySettingBase<T>(SettingReload::None, SettingCondition::None, std::move(pName), std::move(pCategory), std::move(pDefault), std::move(pRange), std::nullopt, std::move(pDescription)) {}
        SillySetting(std::string pName, std::string pCategory, T pDefault, typename SillySetting::Range pRange, SettingReload pReloadRequired, std::optional<std::string> pDescription = std::nullopt)
            : SillySettingBase<T>(pReloadRequired, SettingCondition::None, std::move(pName), std::move(pCategory), std::move(pDefault), std::move(pRange), std::nullopt, std::move(pDescription)) {}
        SillySetting(std::string pName, std::string pCategory, T pDefault, typename SillySetting::Range pRange, SettingCondition pCondition, std::optional<std::string> pDescription = std::nullopt)
            : SillySettingBase<T>(SettingReload::None, pCondition, std::move(pName), std::move(pCategory), std::move(pDefault), std::move(pRange), std::nullopt, std::move(pDescription)) {}
        SillySetting(std::string pName, std::string pCategory, T pDefault, typename SillySetting::Range pRange, SettingReload pReloadRequired, SettingCondition pCondition, std::optional<std::string> pDescription = std::nullopt)
            : SillySettingBase<T>(pReloadRequired, pCondition, std::move(pName), std::move(pCategory), std::move(pDefault), std::move(pRange), std::nullopt, std::move(pDescription)) {}
    };

    using Modifier = std::string;

    constexpr auto modifierSettingOptions() {
        static std::vector<std::string> val{
            "Shift", "Ctrl+Command", "Alt", "Ctrl", "Command", "None"
        };

        return val;
    }
}