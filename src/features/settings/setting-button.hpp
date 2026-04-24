#pragma once

#include <internal/settings.hpp>

namespace Settings {
    class SettingsPopup;

    class SettingButtonBase : public cocos2d::CCNode {
    protected:
        GenericSetting* m_setting = nullptr;
        cocos2d::CCLabelBMFont* m_label = nullptr;
        cocos2d::CCMenu* m_inputMenu = nullptr;
        cocos2d::CCMenu* m_helpMenu = nullptr;
        CCMenuItemSpriteExtra* m_helpButton = nullptr;
        CCMenuItemSpriteExtra* m_reloadIndicator = nullptr;

        SettingsPopup* m_popup = nullptr;

        static constexpr cocos2d::CCSize DEFAULT_SETTING_INPUT_MENU_SIZE{35.0f, 40.0f};
        static constexpr cocos2d::CCSize DEFAULT_SETTING_LABEL_SIZE{85.0f, 40.0f};
        static constexpr float HELP_BUTTON_SIZE = 10.0f;
        static constexpr float HELP_GAP = 5.0f;

        bool init(GenericSetting* pSettingpSetting, SettingsPopup* pPopup);

        void setupReloadIndicator(SettingReload pReload);
        void trySubmitReloadSettingChanged(SettingReload pReload);
        
        void onHelp(cocos2d::CCObject* pSender);

        template<typename T>
        SillySetting<T>* setting() const {
            return static_cast<SillySetting<T>*>(m_setting);
        }
    public:
        static constexpr cocos2d::CCSize SETTING_BUTTON_SIZE{120.0f, 40.0f};

        auto getSetting() const;
    };
    class NumberSettingButtonBase : public SettingButtonBase {
    protected:
        geode::TextInput* m_input;

        bool init(GenericSetting* pSetting, SettingsPopup* pPopup);
    };
    class ColorSettingButtonBase : public SettingButtonBase {
    protected:
        cocos2d::CCSprite* m_colorFill;

        bool init(GenericSetting* pSetting, SettingsPopup* pPopup);

        void onColorPick(cocos2d::CCObject* pSender);
        virtual void setupColorPicker() = 0;
    };

    class BoolSettingButton final : public SettingButtonBase {
    private:
        using T = bool;

        bool init(GenericSetting* pSetting, SettingsPopup* pPopup);

        void onToggle(cocos2d::CCObject* pSender);
    public:
        static BoolSettingButton* create(GenericSetting* pSetting, SettingsPopup* pPopup);
    };
    class IntSettingButton final : public NumberSettingButtonBase {
    private:
        using T = int;

        bool init(GenericSetting* pSetting, SettingsPopup* pPopup);

    public:
        static IntSettingButton* create(GenericSetting* pSetting, SettingsPopup* pPopup);
    };
    class FloatSettingButton final : public NumberSettingButtonBase {
    private:
        using T = float;

        bool init(GenericSetting* pSetting, SettingsPopup* pPopup);
        
    public:
        static FloatSettingButton* create(GenericSetting* pSetting, SettingsPopup* pPopup);
    };
    class StringSettingButton final : public SettingButtonBase {
    private:
        static constexpr float PADDING = 5.0f;
        
        using T = std::string;

        bool init(GenericSetting* pSetting, SettingsPopup* pPopup);
        
    public:
        static StringSettingButton* create(GenericSetting* pSetting, SettingsPopup* pPopup);
    };
    class StrenumSettingButton final : public SettingButtonBase {
    private:
        CCMenuItemSpriteExtra* m_nextArrow;
        CCMenuItemSpriteExtra* m_prevArrow;
        cocos2d::CCLabelBMFont* m_currentLabel;

        int m_currentOption;

        static constexpr float PADDING = 5.0f;
        static constexpr float ARROW_SIZE = 15.0f;
        static constexpr float ARROW_GAP = 15.0f;

        using T = std::string;

        bool init(GenericSetting* pSetting, SettingsPopup* pPopup);

        void setOption(int pOption, bool pSet);
        
        void onNext(cocos2d::CCObject* pSender);
        void onPrevious(cocos2d::CCObject* pSender);
        
    public:
        static StrenumSettingButton* create(GenericSetting* pSetting, SettingsPopup* pPopup);
    };
    class RGBSettingButton final : public ColorSettingButtonBase {
    private:
        using T = cocos2d::ccColor3B;

        bool init(GenericSetting* pSetting, SettingsPopup* pPopup);

        virtual void setupColorPicker() override;
    public:
        static RGBSettingButton* create(GenericSetting* pSetting, SettingsPopup* pPopup);
    };
    class RGBASettingButton final : public ColorSettingButtonBase {
    private:
        using T = cocos2d::ccColor4B;

        bool init(GenericSetting* pSetting, SettingsPopup* pPopup);

        virtual void setupColorPicker() override;
    public:
        static RGBASettingButton* create(GenericSetting* pSetting, SettingsPopup* pPopup);
    };

    SettingButtonBase* createSettingButton(GenericSetting* pSetting, SettingsPopup* pPopup);
}