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
template<typename... Args>
    SillySettingBase(SettingReload pReloadRequired, std::string pName, std::string pCategory, Args... pArgs)
        : SavedSetting<T>(pName, pCategory, nwo5::settings::generateKey(pName, pCategory), std::forward<Args>(pArgs)...), m_editorReloadRequired(pReloadRequired) {}

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
    HideWithPlaytest,
    TextObjectUtils,
    ZoomInput,
    CopyPasteObjectStrings,
    BetterSelectAll,
    ObjectTabIcons,
    Templates,
    Miscellaneous,
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
    inline SillySetting<float> sayoDeviceSensitivity{"Sayo Device Sensitivity", "General", 1.5f, {0.1f, std::nullopt}, "gay speed"};
    inline SillySetting<float> sayoDeviceScreenBrightness{"Sayo Device Screen Brightness", "General", 0.5f, {0.0f, 1.0f}, "gay saturation"};
    inline SillySetting<bool> disableModWarningPopup{"Disable Mod Warning Popup", "General", false, " stops showing a warning every time you start the game about the mod being in beta or wtv"};

    SILLY_API_INLINE_CATEGORY("Keybinds", std::nullopt, "keybinds-logo.png"_spr, SettingCategory::Keybinds)
}