#pragma once

#include <nwo5.silly-api/include/settings/include.hpp>

using namespace nwo5::settings::prelude;

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
    SillySettingBase(std::string pName, std::string pCategory, T pDefault, std::optional<std::string> pDescription, SettingReload pReloadRequired)
        : SavedSetting<T>(std::move(pName), std::move(pCategory), pDefault, std::move(pDescription)), m_editorReloadRequired(pReloadRequired) {}

    operator const T&() = delete;

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
        : SillySettingBase<T>(std::move(pName), std::move(pCategory), std::move(pDefault), std::move(pDescription), SettingReload::None) {}
    SillySetting(std::string pName, std::string pCategory, T pDefault, SettingReload pReloadRequired, std::optional<std::string> pDescription = std::nullopt)
        : SillySettingBase<T>(std::move(pName), std::move(pCategory), std::move(pDefault), std::move(pDescription), pReloadRequired) {}
};

template<>
class SillySetting<std::string> : public SillySettingBase<std::string> {
protected:
    std::vector<std::string> m_enumOptions;

public:
    SillySetting(std::string pName, std::string pCategory, std::string pDefault, std::optional<std::string> pDescription = std::nullopt)
        : SillySettingBase<std::string>(std::move(pName), std::move(pCategory), std::move(pDefault), std::move(pDescription), SettingReload::None) {}
    SillySetting(std::string pName, std::string pCategory, std::string pDefault, SettingReload pReloadRequired, std::optional<std::string> pDescription = std::nullopt)
        : SillySettingBase<std::string>(std::move(pName), std::move(pCategory), std::move(pDefault), std::move(pDescription), pReloadRequired) {}
        SillySetting(std::string pName, std::string pCategory, std::string pDefault, std::vector<std::string> pOptions, std::optional<std::string> pDescription = std::nullopt)
        : SillySettingBase<std::string>(std::move(pName), std::move(pCategory), std::move(pDefault), std::move(pDescription), SettingReload::None), m_enumOptions(std::move(pOptions)) {}
    SillySetting(std::string pName, std::string pCategory, std::string pDefault, std::vector<std::string> pOptions, SettingReload pReloadRequired, std::optional<std::string> pDescription = std::nullopt)
        : SillySettingBase<std::string>(std::move(pName), std::move(pCategory), std::move(pDefault), std::move(pDescription), pReloadRequired), m_enumOptions(std::move(pOptions)) {}

    const auto& enumOptions() const {
        return m_enumOptions;
    }
    bool isEnum() const {
        return !m_enumOptions.empty();
    }
};

enum class SettingCategory {
    General,
    Keybinds,
    DefaultObjectOptions,
    Ruler,
    FloodFill,
    SetupStartpos,
    BetterScale,
    ReplaceObjects,
    FreeSnap,
    BetterLayers,
    TextObjectUtils,
    ZoomInput,
    CopyPasteObjectStrings,
    BetterSelectAll,
    ObjectTabIcons,
    Templates,
    Compat
};

namespace Settings::General {
    SILLY_API_INLINE_CATEGORY("General", std::nullopt, "settings-logo.png"_spr, SettingCategory::General)
    
    inline SillySetting<bool> saveSettingPage{"Save Setting Page", "General", true, "remembers what page u were on since last time the settings popup was opened"};
    inline SillySetting<bool> useLogosForDots{"Use Logos For Dots", "General", true, SettingReload::Popup, "if disabled will use small dots like object menu"};
    inline SillySetting<bool> showPageArrows{"Show Page Arrows", "General", true, SettingReload::Popup};
    inline SillySetting<bool> showSettingsButton{"Show Settings Button", "General", true, SettingReload::Pause, "show settings button in editor pause menu"};
    inline SillySetting<bool> showReloadWarnings{"Show Reload Warnings", "General", true};
    inline SillySetting<std::string> settingsButtonTexture{"Settings Button Texture", "General", "Rainbow", {"Bi", "Enby", "Femboy", "Gay", "Genderqueer", "Intersex", "Pan", "Rainbow", "Trans"}, SettingReload::Pause};
    inline SillySetting<float> sayoDeviceSensitivity{"Sayo Device Sensitivity", "General", 1.5f, "gay speed"};
    inline SillySetting<float> sayoDeviceScreenBrightness{"Sayo Device Screen Brightness", "General", 0.5f, "gay saturation"};

    SILLY_API_INLINE_CATEGORY("Keybinds", std::nullopt, "keybinds-logo.png"_spr, SettingCategory::Keybinds)
}