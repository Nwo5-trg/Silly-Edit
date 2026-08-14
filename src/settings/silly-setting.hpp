#pragma once

#include <nwo5.silly-api/include/settings/include.hpp>

using namespace nwo5::settings::prelude;

namespace Settings {
    enum class SettingReload {
        None,
        Editor,
        Pause,
        Popup,
        Game
    };

    template<typename T>
    class SillySettingBase : public SavedSetting<T>{
    protected:
        SettingReload m_editorReloadRequired;

    public:
    template<typename... Args>
        SillySettingBase(SettingReload pReloadRequired, std::string pName, std::string pCategory, Args... pArgs)
            : SavedSetting<T>(pName, pCategory, nwo5::settings::generateKey(pName, pCategory), std::forward<Args>(pArgs)...), m_editorReloadRequired(pReloadRequired) {}

        SettingReload reloadType() {
            return m_editorReloadRequired;
        }
        bool reloadRequired() {
            return m_editorReloadRequired != SettingReload::None;
        }
    };

    template<typename T>
    class SillySetting : public SillySettingBase<T> {
    public:
        SillySetting(std::string pName, std::string pCategory, T pDefault, std::optional<std::string> pDescription = std::nullopt)
            : SillySettingBase<T>(SettingReload::None, std::move(pName), std::move(pCategory), std::move(pDefault), std::move(pDescription)) {}
        SillySetting(std::string pName, std::string pCategory, T pDefault, SettingReload pReloadRequired, std::optional<std::string> pDescription = std::nullopt)
            : SillySettingBase<T>(pReloadRequired, std::move(pName), std::move(pCategory), std::move(pDefault), std::move(pDescription)) {}
    };

    template<IsStringSettingType T>
    class SillySetting<T> : public SillySettingBase<T> {
    public:
        SillySetting(std::string pName, std::string pCategory, T pDefault, std::optional<std::string> pDescription = std::nullopt)
            : SillySettingBase<T>(SettingReload::None, std::move(pName), std::move(pCategory), std::move(pDefault), std::vector<T>{}, std::move(pDescription)) {}
        SillySetting(std::string pName, std::string pCategory, T pDefault, SettingReload pReloadRequired, std::optional<std::string> pDescription = std::nullopt)
            : SillySettingBase<T>(pReloadRequired, std::move(pName), std::move(pCategory), std::move(pDefault), std::vector<T>{}, std::move(pDescription)) {}
        SillySetting(std::string pName, std::string pCategory, T pDefault, std::vector<T> pOptions, std::optional<std::string> pDescription = std::nullopt)
            : SillySettingBase<T>(SettingReload::None, std::move(pName), std::move(pCategory), std::move(pDefault), std::move(pOptions), std::move(pDescription)) {}
        SillySetting(std::string pName, std::string pCategory, T pDefault, std::vector<T> pOptions, SettingReload pReloadRequired, std::optional<std::string> pDescription = std::nullopt)
            : SillySettingBase<T>(pReloadRequired, std::move(pName), std::move(pCategory), std::move(pDefault), std::move(pOptions), std::move(pDescription)) {}
    };

    template<IsNumberSettingType T>
    class SillySetting<T> : public SillySettingBase<T> {
    public:
        SillySetting(std::string pName, std::string pCategory, T pDefault, std::optional<std::string> pDescription = std::nullopt)
            : SillySettingBase<T>(SettingReload::None, std::move(pName), std::move(pCategory), std::move(pDefault), typename SillySetting::Range{}, std::nullopt, std::move(pDescription)) {}
        SillySetting(std::string pName, std::string pCategory, T pDefault, SettingReload pReloadRequired, std::optional<std::string> pDescription = std::nullopt)
            : SillySettingBase<T>(pReloadRequired, std::move(pName), std::move(pCategory), std::move(pDefault), typename SillySetting::Range{}, std::nullopt, std::move(pDescription)) {}
        SillySetting(std::string pName, std::string pCategory, T pDefault, typename SillySetting::Range pRange, std::optional<std::string> pDescription = std::nullopt)
            : SillySettingBase<T>(SettingReload::None, std::move(pName), std::move(pCategory), std::move(pDefault), std::move(pRange), std::nullopt, std::move(pDescription)) {}
        SillySetting(std::string pName, std::string pCategory, T pDefault, typename SillySetting::Range pRange, SettingReload pReloadRequired, std::optional<std::string> pDescription = std::nullopt)
            : SillySettingBase<T>(pReloadRequired, std::move(pName), std::move(pCategory), std::move(pDefault), std::move(pRange), std::nullopt, std::move(pDescription)) {}
    };
}