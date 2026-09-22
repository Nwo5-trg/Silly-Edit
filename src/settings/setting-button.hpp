#pragma once

#include <nwo5.silly-api/include/utils/include.hpp>
#include <nwo5.silly-api/include/ui/include.hpp>
#include "include.hpp"

namespace sillyedit::settings {
    class SettingButtonBase : public cocos2d::CCNode {
    public:
        static constexpr cocos2d::CCSize SIZE{90.0f, 30.0f};

    protected:
        GenericSetting* m_setting = nullptr;
        geode::Label* m_label = nullptr;
        cocos2d::CCMenu* m_inputMenu = nullptr;
        cocos2d::CCMenu* m_helpMenu = nullptr;
        CCMenuItemSpriteExtra* m_helpButton = nullptr;
        CCMenuItemSpriteExtra* m_resetButton = nullptr;
        CCMenuItemSpriteExtra* m_reloadIndicator = nullptr;

        static constexpr cocos2d::CCSize INPUT_SIZE = {SIZE.width * (3.0f / 10.0f), SIZE.height};
        static constexpr float INPUT_PADDING = 5.0f;

        static constexpr cocos2d::CCSize LABEL_SIZE = {SIZE.width * (7.0f / 10.0f), SIZE.height};
        static constexpr float LABEL_PADDING = 5.0f;

        static constexpr float HELP_BUTTON_SIZE = 7.5f;
        static constexpr float HELP_GAP = 5.0f;

        bool init(GenericSetting* pSettingpSetting);
        
        void onHelp(cocos2d::CCObject*);
        void onReset(cocos2d::CCObject*);

        template<typename T>
        void setup() {
            auto cast = this->setting<T>();

            m_resetButton->setVisible(cast->get() != cast->getDefault());

            // idek it wasnt working now it is and im not sure this is a reason - it shoudlnt be the reason - but im scared
            const auto shouldShowHelp = m_setting->hasDescription();
            m_helpButton->setVisible(shouldShowHelp);

            if (shouldShowHelp) {
                const auto str = m_setting->description();
                m_helpButton->setUserObject("nwo5.silly-api/tooltip", nwo5::ui::TooltipInfo::create(
                    geode::utils::string::toUpper(str.substr(0, 1)) + str.substr(1)
                ));
            }

            const auto shouldShowReload = cast->reloadRequired();
            m_reloadIndicator->setVisible(shouldShowReload);

            m_helpMenu->updateLayout();
            
            if (!shouldShowReload) {
                return;
            }

            switch (cast->reloadType()) {
                case SettingReload::Editor: {
                    nwo5::ui::Setup(m_reloadIndicator)
                        .color(nwo5::utils::Col::Orange)
                        .callback([] (auto) {
                            geode::Notification::create("editor reload is required to apply setting !", geode::NotificationIcon::Info)->show();
                        })
                        .userObject("nwo5.silly-api/tooltip", nwo5::ui::TooltipInfo::create("Editor reload required"));
                break; }
                case SettingReload::Pause: {
                    nwo5::ui::Setup(m_reloadIndicator)
                        .color(nwo5::utils::Col::Red)
                        .callback([] (auto) {
                            geode::Notification::create("pause menu reload is required to apply setting !", geode::NotificationIcon::Info)->show();
                        })
                        .userObject("nwo5.silly-api/tooltip", nwo5::ui::TooltipInfo::create("Pause reload required"));
                break; }
                case SettingReload::Popup: {
                    nwo5::ui::Setup(m_reloadIndicator)
                        .color(nwo5::utils::Col::Blue)
                        .callback([] (auto) {
                            geode::Notification::create("settings popup reload is required to apply setting !", geode::NotificationIcon::Info)->show();
                        })
                        .userObject("nwo5.silly-api/tooltip", nwo5::ui::TooltipInfo::create("Popup reload required"));
                break; }
                case SettingReload::Game: {
                    nwo5::ui::Setup(m_reloadIndicator)
                        .color(nwo5::utils::Col::Gray)
                        .callback([] (auto) {
                            geode::Notification::create("game reload is required to apply setting !", geode::NotificationIcon::Info)->show();
                        })
                        .userObject("nwo5.silly-api/tooltip", nwo5::ui::TooltipInfo::create("Game reload required"));
                break; }
                default: return;
            }
        }

        template<typename T>
        void set(const T& pVal) {
            auto cast = static_cast<SillySetting<T>*>(m_setting);

            cast->set(pVal);
            m_resetButton->setVisible(pVal != cast->getDefault());
            m_helpMenu->updateLayout();
        }

        template<typename T>
        SillySetting<T>* setting() const {
            return static_cast<SillySetting<T>*>(m_setting);
        }
    
    public:
        auto getSetting() const;

        virtual void resetSetting() = 0;
    };
    class NumberSettingButtonBase : public SettingButtonBase {
    protected:
        geode::TextInput* m_input;

        bool init(GenericSetting* pSetting);
        virtual void resetSetting() = 0;
    };
    class ColorSettingButtonBase : public SettingButtonBase {
    protected:
        cocos2d::CCSprite* m_colorFill;

        bool init(GenericSetting* pSetting);
        virtual void resetSetting() = 0;

        void onColorPick(cocos2d::CCObject* pSender);
        virtual void setupColorPicker() = 0;
    };

    class BoolSettingButton final : public SettingButtonBase {
    private:
        CCMenuItemToggler* m_toggler = nullptr;

        using T = bool;

        bool init(GenericSetting* pSetting);
        void resetSetting() override;

        void onToggle(cocos2d::CCObject* pSender);
    public:
        static BoolSettingButton* create(GenericSetting* pSetting);
    };
    class IntSettingButton final : public NumberSettingButtonBase {
    private:
        using T = int;

        bool init(GenericSetting* pSetting);
        void resetSetting() override;

    public:
        static IntSettingButton* create(GenericSetting* pSetting);
    };
    class FloatSettingButton final : public NumberSettingButtonBase {
    private:
        using T = float;

        bool init(GenericSetting* pSetting);
        void resetSetting() override;
        
    public:
        static FloatSettingButton* create(GenericSetting* pSetting);
    };
    class OpacitySettingButton final : public SettingButtonBase {
    private:
        geode::Label* m_opacityLabel = nullptr;
        geode::SliderNode* m_slider = nullptr;

        static constexpr float PADDING = 2.5f;
        static constexpr float SLIDER_OFFSET = 5.0f;
        
        using T = int;

        bool init(GenericSetting* pSetting);
        void resetSetting() override;

        void updateVisuals();
        
    public:
        static OpacitySettingButton* create(GenericSetting* pSetting);
    };
    class StringSettingButton final : public SettingButtonBase {
    private:
        geode::TextInput* m_input = nullptr;

        static constexpr float PADDING = 2.5f;
        
        using T = std::string;

        bool init(GenericSetting* pSetting);
        void resetSetting() override;
        
    public:
        static StringSettingButton* create(GenericSetting* pSetting);
    };
    class StrenumSettingButton final : public SettingButtonBase {
    private:
        CCMenuItemSpriteExtra* m_nextArrow;
        CCMenuItemSpriteExtra* m_prevArrow;
        geode::Label* m_currentLabel;

        int m_currentOption;

        static constexpr float PADDING = 2.5f;
        static constexpr float ARROW_SIZE = 10.0f;
        // static constexpr float ARROW_GAP = 10.0f;

        using T = std::string;

        bool init(GenericSetting* pSetting);
        void resetSetting() override;

        void setOption(int pOption, bool pSet);
        
        void onNext(cocos2d::CCObject*);
        void onPrevious(cocos2d::CCObject*);
        
    public:
        static StrenumSettingButton* create(GenericSetting* pSetting);
    };
    class RGBSettingButton final : public ColorSettingButtonBase {
    private:
        using T = cocos2d::ccColor3B;

        bool init(GenericSetting* pSetting);
        void resetSetting() override;

        virtual void setupColorPicker() override;
    public:
        static RGBSettingButton* create(GenericSetting* pSetting);
    };
    class RGBASettingButton final : public ColorSettingButtonBase {
    private:
        using T = cocos2d::ccColor4B;

        bool init(GenericSetting* pSetting);
        void resetSetting() override;

        virtual void setupColorPicker() override;
    public:
        static RGBASettingButton* create(GenericSetting* pSetting);
    };

    SettingButtonBase* createSettingButton(GenericSetting* pSetting);
}