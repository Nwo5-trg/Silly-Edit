#include <internal/utils/utils.hpp>
#include "setting-button.hpp"

using namespace geode::prelude;
using namespace nwo5::syntax;

namespace Settings {
    bool SettingButtonBase::init(GenericSetting* pSetting) {
        if (!CCNode::init()) {
            return false;
        }

        m_setting = pSetting;

        nwo5::utils::setupNode(
            this,

            SetNodeID{"{}-setting"_spr, pSetting->key()},
            SetNodeSize{SETTING_BUTTON_SIZE}
        );

        m_label = nwo5::utils::setupNode(
            CCLabelBMFont::create(m_setting->name().c_str(), "bigFont.fnt"),

            SetNodeID{"label"_spr},
            SetNodeAnchor{LEFT_CENTER_ANCHOR},
            SetNodeScaleWithWidth{DEFAULT_SETTING_LABEL_SIZE.width * (5.0f/6.0f)},
            LimitNodeScaleWithHeight{DEFAULT_SETTING_LABEL_SIZE.height *  (2.0f/3.0f)},
            SetNodePosition{
                DEFAULT_SETTING_INPUT_MENU_SIZE.width + (DEFAULT_SETTING_LABEL_SIZE.width - (DEFAULT_SETTING_LABEL_SIZE.width * (5.0f/6.0f))) / 2,
                DEFAULT_SETTING_LABEL_SIZE.height / 2
            },
            SetNodeParent{this}
        );

        m_inputMenu = nwo5::utils::setupNode(
            CCMenu::create(),

            SetNodeID{"input-menu"_spr},
            SetNodeSize{DEFAULT_SETTING_INPUT_MENU_SIZE},
            SetNodePosition{DEFAULT_SETTING_INPUT_MENU_SIZE / 2},
            SetNodeParent{this}
        );

        m_helpMenu = nwo5::utils::setupNode(
            CCMenu::create(),
            
            SetNodeID{"help-menu"_spr},
            SetNodeSize{CCSizeZero},
            SetNodePosition{SETTING_BUTTON_SIZE},
            SetNodeParent{this},
            SetNodeChildren{
                (m_helpButton = nwo5::utils::setupNode(
                    CCMenuItemSpriteExtra::create(
                        CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"),
                        this, menu_selector(SettingButtonBase::onHelp)
                    ),

                    SetNodeID{"help-button"_spr},
                    SetNodeScaleWithSize{HELP_BUTTON_SIZE},
                    SetNodePosition{CCPointZero}
                ))
            }
        );

        // idek it wasnt working now it is and im not sure this is a reason - it shoudlnt be the reason - but im scared
        const auto shouldShowHelp = pSetting->hasDescription();
        m_helpMenu->setVisible(shouldShowHelp);

        return true;
    }
    void SettingButtonBase::onHelp(CCObject* pSender) {
        FLAlertLayer::create(
            "Help",
            m_setting->description(),
            "OK"
        )->show();
    }
    auto SettingButtonBase::getSetting() const {
        return m_setting;
    }

    bool NumberSettingButtonBase::init(GenericSetting* pSetting) {
        if (!SettingButtonBase::init(pSetting)) {
            return false;
        }

        m_input = nwo5::utils::setupNode(
            nwo5::utils::createTextInput(DEFAULT_SETTING_INPUT_MENU_SIZE.width * (5.0f/6.0f), DEFAULT_SETTING_INPUT_MENU_SIZE.height / 2),

            SetNodeID{"input"_spr},
            SetNodePosition{CCPointZero},
            SetNodeParent{m_inputMenu}
        );

        return true;
    }

    bool ColorSettingButtonBase::init(GenericSetting* pSetting) {
        if (!SettingButtonBase::init(pSetting)) {
            return false;
        }

        nwo5::utils::setupNode(
            CCMenuItemSpriteExtra::create
                ((m_colorFill = CCSprite::create("color-button-fill.png"_spr)), 
                this, menu_selector(ColorSettingButtonBase::onColorPick)
            ),

            SetNodeID{"color_button"_spr},
            SetNodeScaleWithSize{DEFAULT_SETTING_INPUT_MENU_SIZE.width * (2.0f/3.0f)},
            SetNodePosition{CCPointZero},
            SetNodeParent{m_inputMenu}
        );

        m_colorFill->addChildAtPosition(CCSprite::create("color-button-frame.png"_spr), Anchor::Center);

        return true;
    }
    void ColorSettingButtonBase::onColorPick(CCObject* pSender) {
        setupColorPicker();
    }

    #define SE_SETUP_SETTING_BUTTON_CREATE(pName) \
    pName * pName ::create(GenericSetting* pSetting) { \
        auto ret = new pName ; \
    \
        if (!ret->init(pSetting)) { \
            delete ret; \
    \
            return nullptr; \
        } \
    \
        ret->autorelease(); \
    \
        return ret; \
    }

    bool BoolSettingButton::init(GenericSetting* pSetting) {
        if (!SettingButtonBase::init(pSetting)) {
            return false;
        }

        auto toggler = nwo5::utils::setupNode(
            CCMenuItemToggler::create(
                CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"),
                CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"),
                this, menu_selector(BoolSettingButton::onToggle)
            ),

            SetNodeID{"toggle"_spr},
            SetNodeScaleWithSize{DEFAULT_SETTING_INPUT_MENU_SIZE.width * (2.0f/3.0f)},
            SetNodePosition{CCPointZero},
            SetNodeParent{m_inputMenu}
        );

        toggler->toggle(setting<T>()->get());

        return true;
    }
    void BoolSettingButton::onToggle(cocos2d::CCObject* pSender) {
        setting<T>()->set(nwo5::utils::isToggled(pSender));
    }
    SE_SETUP_SETTING_BUTTON_CREATE(BoolSettingButton)

    bool IntSettingButton::init(GenericSetting* pSetting) {
        if (!NumberSettingButtonBase::init(pSetting)) {
            return false;
        }

        m_input->setPlaceholder(nwo5::utils::numToString(setting<T>()->getDefault()));
        m_input->setCommonFilter(CommonFilter::Int);
        m_input->setString(nwo5::utils::numToString(setting<T>()->get()));
        m_input->setCallback([this] (const std::string& pStr) {
            if (pStr.empty()) {
                setting<T>()->set(setting<T>()->getDefault());
            }
            else {
                setting<T>()->set(utils::numFromString<T>(pStr).unwrapOrDefault());
            }
        });

        return true;
    }
    SE_SETUP_SETTING_BUTTON_CREATE(IntSettingButton)

    bool FloatSettingButton::init(GenericSetting* pSetting) {
        if (!NumberSettingButtonBase::init(pSetting)) {
            return false;
        }

        m_input->setPlaceholder(nwo5::utils::numToString(setting<T>()->getDefault()));
        m_input->setCommonFilter(CommonFilter::Float);
        m_input->setString(nwo5::utils::numToString(setting<T>()->get()));
        m_input->setCallback([this] (const std::string& pStr) {
            if (pStr.empty()) {
                setting<T>()->set(setting<T>()->getDefault());
            }
            else {
                setting<T>()->set(utils::numFromString<T>(pStr).unwrapOrDefault());
            }
        });

        return true;
    }
    SE_SETUP_SETTING_BUTTON_CREATE(FloatSettingButton)

    bool StringSettingButton::init(GenericSetting* pSetting) {
        if (!SettingButtonBase::init(pSetting)) {
            return false;
        }

        constexpr float PADDING = 5.0f;

        m_inputMenu->setPosition(CCPointZero);

        auto input = nwo5::utils::setupNode(
            nwo5::utils::createTextInput(
                SETTING_BUTTON_SIZE.width - PADDING * 2, SETTING_BUTTON_SIZE.height / 2 - PADDING, 
                setting<T>()->getDefault(), 
            [this] (const std::string& pStr) {
                if (pStr.empty()) {
                    setting<T>()->set(setting<T>()->getDefault());
                }
                else if (pStr == "\\0") {
                    setting<T>()->set("");
                }
                else {
                    setting<T>()->set(pStr);
                }
            }),

            SetNodeID{"input"_spr},
            SetNodePosition{SETTING_BUTTON_SIZE.width / 2, SETTING_BUTTON_SIZE.height * (3.0f/4.0f)},
            SetNodeParent{m_inputMenu}
        );
        input->setCommonFilter(CommonFilter::Any);
        input->setString(setting<T>()->get());

        nwo5::utils::setupNode(
            m_label,

            SetNodeAnchor{CENTER_ANCHOR},
            SetNodeScaleWithHeight{SETTING_BUTTON_SIZE.height / 2 - PADDING},
            LimitNodeScaleWithWidth{SETTING_BUTTON_SIZE.width - PADDING * 2},
            SetNodePosition{SETTING_BUTTON_SIZE.width / 2, SETTING_BUTTON_SIZE.height * (1.0f/4.0f)}
        );

        return true;
    }
    SE_SETUP_SETTING_BUTTON_CREATE(StringSettingButton)

    bool RGBSettingButton::init(GenericSetting* pSetting) {
        if (!ColorSettingButtonBase::init(pSetting)) {
            return false;
        }

        m_colorFill->setColor(setting<T>()->get());

        return true;
    }
    void RGBSettingButton::setupColorPicker() {
        auto popup = ColorPickPopup::create(setting<T>()->get());

        popup->setCallback([this] (const ccColor4B& pCol) {
            const auto col = to3B(pCol);

            m_colorFill->setColor(col);
            setting<T>()->set(col);
        });
        popup->show();
    }
    SE_SETUP_SETTING_BUTTON_CREATE(RGBSettingButton)

    bool RGBASettingButton::init(GenericSetting* pSetting) {
        if (!ColorSettingButtonBase::init(pSetting)) {
            return false;
        }

        m_colorFill->setColor(to3B(setting<T>()->get()));
        m_colorFill->setOpacity(setting<T>()->get().a);

        return true;
    }
    void RGBASettingButton::setupColorPicker() {
        auto popup = ColorPickPopup::create(setting<T>()->get());

        popup->setCallback([this] (const ccColor4B& pCol) {
            m_colorFill->setColor(to3B(pCol));
            m_colorFill->setOpacity(pCol.a);
            setting<T>()->set(pCol);
        });
        popup->show();
    }
    SE_SETUP_SETTING_BUTTON_CREATE(RGBASettingButton)

    SettingButtonBase* createSettingButton(GenericSetting* pSetting) {
        switch (pSetting->type()) {
            case SettingType::Bool: return BoolSettingButton::create(pSetting);
            case SettingType::Int: return IntSettingButton::create(pSetting);
            case SettingType::Float: return FloatSettingButton::create(pSetting);
            case SettingType::String: return StringSettingButton::create(pSetting);
            case SettingType::RGB: return RGBSettingButton::create(pSetting);
            case SettingType::RGBA: return RGBASettingButton::create(pSetting);
            default: return nullptr;
        }
    }
}