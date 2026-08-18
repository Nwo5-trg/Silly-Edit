#pragma once

#include "include.hpp"

namespace Settings {
    class SettingButtonBase : public cocos2d::CCNode {
    public:
        static constexpr cocos2d::CCSize SIZE{90.0f, 30.0f};

    protected:
        GenericSetting* m_setting = nullptr;
        cocos2d::CCLabelBMFont* m_label = nullptr;
        cocos2d::CCMenu* m_inputMenu = nullptr;
        cocos2d::CCMenu* m_helpMenu = nullptr;
        CCMenuItemSpriteExtra* m_helpButton = nullptr;
        CCMenuItemSpriteExtra* m_reloadIndicator = nullptr;

        static constexpr cocos2d::CCSize INPUT_SIZE = {SIZE.width * (3.0f / 10.0f), SIZE.height};
        static constexpr float INPUT_PADDING = 5.0f;

        static constexpr cocos2d::CCSize LABEL_SIZE = {SIZE.width * (7.0f / 10.0f), SIZE.height};
        static constexpr float LABEL_PADDING = 5.0f;

        static constexpr float HELP_BUTTON_SIZE = 7.5f;
        static constexpr float HELP_GAP = 5.0f;

        bool init(GenericSetting* pSettingpSetting);

        void setupReloadIndicator(SettingReload pReload);
        
        void onHelp(cocos2d::CCObject* pSender);

        template<typename T>
        SillySetting<T>* setting() const {
            return static_cast<SillySetting<T>*>(m_setting);
        }
    
    public:
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
        static constexpr float PADDING = 2.5f;
        
        using T = std::string;

        bool init(GenericSetting* pSetting);
        
    public:
        static StringSettingButton* create(GenericSetting* pSetting);
    };
    class StrenumSettingButton final : public SettingButtonBase {
    private:
        CCMenuItemSpriteExtra* m_nextArrow;
        CCMenuItemSpriteExtra* m_prevArrow;
        cocos2d::CCLabelBMFont* m_currentLabel;

        int m_currentOption;

        static constexpr float PADDING = 2.5f;
        static constexpr float ARROW_SIZE = 10.0f;
        // static constexpr float ARROW_GAP = 10.0f;

        using T = std::string;

        bool init(GenericSetting* pSetting);

        void setOption(int pOption, bool pSet);
        
        void onNext(cocos2d::CCObject* pSender);
        void onPrevious(cocos2d::CCObject* pSender);
        
    public:
        static StrenumSettingButton* create(GenericSetting* pSetting);
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