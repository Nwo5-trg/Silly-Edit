#pragma once

#include <internal/settings.hpp>

namespace Settings {
    class SettingButtonBase : public cocos2d::CCNode {
    protected:
        SettingBase* m_setting = nullptr;
        cocos2d::CCLabelBMFont* m_label = nullptr;
        cocos2d::CCMenu* m_inputMenu = nullptr;
        cocos2d::CCMenu* m_helpMenu = nullptr;
        CCMenuItemSpriteExtra* m_helpButton = nullptr;

        static constexpr cocos2d::CCSize DEFAULT_SETTING_INPUT_MENU_SIZE{35.0f, 40.0f};
        static constexpr cocos2d::CCSize DEFAULT_SETTING_LABEL_SIZE{85.0f, 40.0f};
        static constexpr float HELP_BUTTON_SIZE = 10.0f;

        bool init(SettingBase* pSetting);
        
        void onHelp(cocos2d::CCObject* pSender);

        template<typename T>
        Setting<T>* setting() const {
            return static_cast<Setting<T>*>(m_setting);
        }
    public:
        static constexpr cocos2d::CCSize SETTING_BUTTON_SIZE{120.0f, 40.0f};

        auto getSetting() const;
    };
    class NumberSettingButtonBase : public SettingButtonBase {
    protected:
        geode::TextInput* m_input;

        bool init(SettingBase* pSetting);
    };
    class ColorSettingButtonBase : public SettingButtonBase {
    protected:
        cocos2d::CCSprite* m_colorFill;

        bool init(SettingBase* pSetting);

        void onColorPick(cocos2d::CCObject* pSender);
        virtual void setupColorPicker() = 0;
    };

    class BoolSettingButton final : public SettingButtonBase {
    private:
        using T = bool;

        bool init(SettingBase* pSetting);

        void onToggle(cocos2d::CCObject* pSender);
    public:
        static BoolSettingButton* create(SettingBase* pSetting);
    };
    class IntSettingButton final : public NumberSettingButtonBase {
    private:
        using T = int;

        bool init(SettingBase* pSetting);

    public:
        static IntSettingButton* create(SettingBase* pSetting);
    };
    class FloatSettingButton final : public NumberSettingButtonBase {
    private:
        using T = float;

        bool init(SettingBase* pSetting);
        
    public:
        static FloatSettingButton* create(SettingBase* pSetting);
    };
    class StringSettingButton final : public SettingButtonBase {
    private:
        using T = std::string;

        bool init(SettingBase* pSetting);
        
    public:
        static StringSettingButton* create(SettingBase* pSetting);
    };
    class RGBSettingButton final : public ColorSettingButtonBase {
    private:
        using T = cocos2d::ccColor3B;

        bool init(SettingBase* pSetting);

        virtual void setupColorPicker() override;
    public:
        static RGBSettingButton* create(SettingBase* pSetting);
    };
    class RGBASettingButton final : public ColorSettingButtonBase {
    private:
        using T = cocos2d::ccColor4B;

        bool init(SettingBase* pSetting);

        virtual void setupColorPicker() override;
    public:
        static RGBASettingButton* create(SettingBase* pSetting);
    };

    SettingButtonBase* createSettingButton(SettingBase* pSetting);
}