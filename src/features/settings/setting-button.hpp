#pragma once

#include <internal/settings.hpp>

namespace Settings {
    class SettingButtonBase : public cocos2d::CCNode {
    protected:
        GenericSetting* m_setting = nullptr;
        cocos2d::CCLabelBMFont* m_label = nullptr;
        cocos2d::CCMenu* m_inputMenu = nullptr;
        cocos2d::CCMenu* m_helpMenu = nullptr;
        CCMenuItemSpriteExtra* m_helpButton = nullptr;

        static constexpr cocos2d::CCSize DEFAULT_SETTING_INPUT_MENU_SIZE{35.0f, 40.0f};
        static constexpr cocos2d::CCSize DEFAULT_SETTING_LABEL_SIZE{85.0f, 40.0f};
        static constexpr float HELP_BUTTON_SIZE = 10.0f;

        bool init(GenericSetting* pSetting);
        
        void onHelp(cocos2d::CCObject* pSender);

        template<typename T>
        SavedSetting<T>* setting() const {
            return static_cast<SavedSetting<T>*>(m_setting);
        }
    public:
        static constexpr cocos2d::CCSize SETTING_BUTTON_SIZE{120.0f, 40.0f};

        auto getSetting() const;
    };
    class NumberSettingButtonBase : public SettingButtonBase {
    protected:
        geode::TextInput* m_input;

        bool init(GenericSetting* pSetting);
    };
    class ColorSettingButtonBase : public SettingButtonBase {
    protected:
        cocos2d::CCSprite* m_colorFill;

        bool init(GenericSetting* pSetting);

        void onColorPick(cocos2d::CCObject* pSender);
        virtual void setupColorPicker() = 0;
    };

    class BoolSettingButton final : public SettingButtonBase {
    private:
        using T = bool;

        bool init(GenericSetting* pSetting);

        void onToggle(cocos2d::CCObject* pSender);
    public:
        static BoolSettingButton* create(GenericSetting* pSetting);
    };
    class IntSettingButton final : public NumberSettingButtonBase {
    private:
        using T = int;

        bool init(GenericSetting* pSetting);

    public:
        static IntSettingButton* create(GenericSetting* pSetting);
    };
    class FloatSettingButton final : public NumberSettingButtonBase {
    private:
        using T = float;

        bool init(GenericSetting* pSetting);
        
    public:
        static FloatSettingButton* create(GenericSetting* pSetting);
    };
    class StringSettingButton final : public SettingButtonBase {
    private:
        using T = std::string;

        bool init(GenericSetting* pSetting);
        
    public:
        static StringSettingButton* create(GenericSetting* pSetting);
    };
    class RGBSettingButton final : public ColorSettingButtonBase {
    private:
        using T = cocos2d::ccColor3B;

        bool init(GenericSetting* pSetting);

        virtual void setupColorPicker() override;
    public:
        static RGBSettingButton* create(GenericSetting* pSetting);
    };
    class RGBASettingButton final : public ColorSettingButtonBase {
    private:
        using T = cocos2d::ccColor4B;

        bool init(GenericSetting* pSetting);

        virtual void setupColorPicker() override;
    public:
        static RGBASettingButton* create(GenericSetting* pSetting);
    };

    SettingButtonBase* createSettingButton(GenericSetting* pSetting);
}