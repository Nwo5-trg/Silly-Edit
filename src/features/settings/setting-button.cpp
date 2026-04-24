#include <internal/utils/utils.hpp>
#include "setting-button.hpp"
#include "popup.hpp"

using namespace geode::prelude;
using namespace nwo5::syntax;

namespace Settings {
    bool SettingButtonBase::init(GenericSetting* pSetting, SettingsPopup* pPopup) {
        if (!CCNode::init()) {
            return false;
        }

        m_popup = pPopup;

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
            SetNodeSize{HELP_BUTTON_SIZE, HELP_BUTTON_SIZE},
            SetNodeAnchor{RIGHT_CENTER_ANCHOR},
            SetNodePosition{SETTING_BUTTON_SIZE.width + HELP_BUTTON_SIZE / 2, SETTING_BUTTON_SIZE.height},
            SetNodeParent{this},
            SetNodeChildren{
                (m_helpButton = nwo5::utils::setupNode(
                    nwo5::utils::createButtonFrame("GJ_infoIcon_001.png", this, menu_selector(SettingButtonBase::onHelp)),

                    SetNodeID{"help-button"_spr},
                    SetNodeScaleWithSize{HELP_BUTTON_SIZE}
                )),
                (m_reloadIndicator = nwo5::utils::setupNode(
                    nwo5::utils::createButtonFrame("edit_ccwBtn_001.png", this, nullptr),

                    SetNodeID{"reload-button"_spr},
                    SetNodeScaleWithSize{HELP_BUTTON_SIZE}
                ))
            }
        );
        m_helpMenu->setLayout(AxisLayout::create()
            ->setAutoScale(false)
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::Start)
            ->setGrowCrossAxis(false)
            ->setAutoGrowAxis(0.0f)
            ->setGap(HELP_GAP)
        );

        return true;
    }
    void SettingButtonBase::setupReloadIndicator(SettingReload pReload) {
        // idek it wasnt working now it is and im not sure this is a reason - it shoudlnt be the reason - but im scared
        const auto shouldShowHelp = m_setting->hasDescription();
        m_helpButton->setVisible(shouldShowHelp);

        if (pReload == SettingReload::None) {
            m_reloadIndicator->setVisible(false);

            return m_helpMenu->updateLayout();
        }

        m_helpMenu->updateLayout();

        // i love template deduction
        CCMenuItemExt::assignCallback<CCNode>(m_reloadIndicator, [pReload] (CCNode*) {
            switch (pReload) {
                case SettingReload::Editor: return Notification::create("editor reload is required to apply setting !", NotificationIcon::Info)->show();
                case SettingReload::Pause: return Notification::create("pause menu reload is required to apply setting !", NotificationIcon::Info)->show();
                case SettingReload::Popup: return Notification::create("settings popup reload is required to apply setting !", NotificationIcon::Info)->show();
                case SettingReload::Game: return Notification::create("game reload is required to apply setting !", NotificationIcon::Info)->show();
                default: return;
            }
        });

        switch (pReload) {
            case SettingReload::Editor: return m_reloadIndicator->setColor(ccRED);
            case SettingReload::Pause: return m_reloadIndicator->setColor(ccORANGE);
            case SettingReload::Popup: return m_reloadIndicator->setColor(ccBLUE);
            case SettingReload::Game: return m_reloadIndicator->setColor(ccGRAY);
            default: return;
        }
    }
    void SettingButtonBase::trySubmitReloadSettingChanged(SettingReload pReload) {
        m_popup->settingChanged(m_setting, pReload);
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

    bool NumberSettingButtonBase::init(GenericSetting* pSetting, SettingsPopup* pPopup) {
        if (!SettingButtonBase::init(pSetting, pPopup)) {
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

    bool ColorSettingButtonBase::init(GenericSetting* pSetting, SettingsPopup* pPopup) {
        if (!SettingButtonBase::init(pSetting, pPopup)) {
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
    pName * pName ::create(GenericSetting* pSetting, SettingsPopup* pPopup) { \
        auto ret = new pName ; \
    \
        if (!ret->init(pSetting, pPopup)) { \
            delete ret; \
    \
            return nullptr; \
        } \
    \
        ret->autorelease(); \
    \
        return ret; \
    }

    bool BoolSettingButton::init(GenericSetting* pSetting, SettingsPopup* pPopup) {
        if (!SettingButtonBase::init(pSetting, pPopup)) {
            return false;
        }

        setupReloadIndicator(setting<T>()->reloadType());

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
        
        trySubmitReloadSettingChanged(setting<T>()->reloadType());
    }
    SE_SETUP_SETTING_BUTTON_CREATE(BoolSettingButton)

    bool IntSettingButton::init(GenericSetting* pSetting, SettingsPopup* pPopup) {
        if (!NumberSettingButtonBase::init(pSetting, pPopup)) {
            return false;
        }

        setupReloadIndicator(setting<T>()->reloadType());

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

            trySubmitReloadSettingChanged(setting<T>()->reloadType());
        });

        return true;
    }
    SE_SETUP_SETTING_BUTTON_CREATE(IntSettingButton)

    bool FloatSettingButton::init(GenericSetting* pSetting, SettingsPopup* pPopup) {
        if (!NumberSettingButtonBase::init(pSetting, pPopup)) {
            return false;
        }

        setupReloadIndicator(setting<T>()->reloadType());

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

            trySubmitReloadSettingChanged(setting<T>()->reloadType());
        });

        return true;
    }
    SE_SETUP_SETTING_BUTTON_CREATE(FloatSettingButton)

    bool StringSettingButton::init(GenericSetting* pSetting, SettingsPopup* pPopup) {
        if (!SettingButtonBase::init(pSetting, pPopup)) {
            return false;
        }

        setupReloadIndicator(setting<T>()->reloadType());

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

                trySubmitReloadSettingChanged(setting<T>()->reloadType());
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

    bool StrenumSettingButton::init(GenericSetting* pSetting, SettingsPopup* pPopup) {
        if (!SettingButtonBase::init(pSetting, pPopup)) {
            return false;
        }

        setupReloadIndicator(setting<T>()->reloadType());

        m_inputMenu->setPosition(CCPointZero);

        m_currentLabel = nwo5::utils::setupNode(
            CCLabelBMFont::create("", "bigFont.fnt"),

            SetNodeID{"current-label"_spr},
            SetNodePosition{SETTING_BUTTON_SIZE.width / 2, SETTING_BUTTON_SIZE.height * (3.0f/4.0f)},
            SetNodeParent{m_inputMenu}
        );

        m_nextArrow = nwo5::utils::setupNode(
            nwo5::utils::createButtonFrame("GJ_arrow_02_001.png", this, menu_selector(StrenumSettingButton::onNext)),

            SetNodeID{"next-button"_spr},
            SetNodePositionY{SETTING_BUTTON_SIZE.height * (3.0f/4.0f)},
            SetNodeScaleWithSize{ARROW_SIZE},
            SetNodeParent{m_inputMenu}
        );
        m_nextArrow->setRotationY(180.0f);

        m_prevArrow = nwo5::utils::setupNode(
            nwo5::utils::createButtonFrame("GJ_arrow_02_001.png", this, menu_selector(StrenumSettingButton::onPrevious)),

            SetNodeID{"previous-button"_spr},
            SetNodePositionY{SETTING_BUTTON_SIZE.height * (3.0f/4.0f)},
            SetNodeScaleWithSize{ARROW_SIZE},
            SetNodeParent{m_inputMenu}
        );

        nwo5::utils::setupNode(
            m_label,

            SetNodeAnchor{CENTER_ANCHOR},
            SetNodeScaleWithHeight{SETTING_BUTTON_SIZE.height / 2 - PADDING},
            LimitNodeScaleWithWidth{SETTING_BUTTON_SIZE.width - PADDING * 2},
          
            SetNodePosition{SETTING_BUTTON_SIZE.width / 2, SETTING_BUTTON_SIZE.height * (1.0f/4.0f)}
        );

        for (int i = 0; i < setting<T>()->enumOptions().size(); i++) {
            if (setting<T>()->enumOptions()[i] == setting<T>()->get()) {
                setOption(i, false);

                break;
            }
        }

        return true;
    }
    void StrenumSettingButton::setOption(int pOption, bool pSet) {
        const auto& str = setting<T>()->enumOptions()[pOption];

        m_currentLabel->setString(str.c_str());
        nwo5::utils::setupNode(
            m_currentLabel,

            SetNodeScaleWithHeight{SETTING_BUTTON_SIZE.height / 2 - PADDING},
            LimitNodeScaleWithWidth{SETTING_BUTTON_SIZE.width * (3.0f/4.0f) - PADDING * 2}
        );

        m_nextArrow->setPositionX(SETTING_BUTTON_SIZE.width / 2 + m_currentLabel->getScaledContentWidth() / 2 + ARROW_GAP);
        m_prevArrow->setPositionX(SETTING_BUTTON_SIZE.width / 2 - m_currentLabel->getScaledContentWidth() / 2 - ARROW_GAP);

        if (pSet) {
            setting<T>()->set(str);

            trySubmitReloadSettingChanged(setting<T>()->reloadType());
        }

        m_currentOption = pOption;
    }
    void StrenumSettingButton::onNext(CCObject* pSender) {
        setOption((m_currentOption + 1) % setting<T>()->enumOptions().size(), true);
    }
    void StrenumSettingButton::onPrevious(CCObject* pSender) {
        setOption(m_currentOption ? (m_currentOption - 1) % setting<T>()->enumOptions().size() : setting<T>()->enumOptions().size() - 1, true);
    }
    SE_SETUP_SETTING_BUTTON_CREATE(StrenumSettingButton)

    bool RGBSettingButton::init(GenericSetting* pSetting, SettingsPopup* pPopup) {
        if (!ColorSettingButtonBase::init(pSetting, pPopup)) {
            return false;
        }

        setupReloadIndicator(setting<T>()->reloadType());

        m_colorFill->setColor(setting<T>()->get());

        return true;
    }
    void RGBSettingButton::setupColorPicker() {
        auto popup = ColorPickPopup::create(setting<T>()->get());

        popup->setCallback([this] (const ccColor4B& pCol) {
            const auto col = to3B(pCol);

            m_colorFill->setColor(col);
            
            setting<T>()->set(col);

            trySubmitReloadSettingChanged(setting<T>()->reloadType());
        });
        popup->show();
    }
    SE_SETUP_SETTING_BUTTON_CREATE(RGBSettingButton)

    bool RGBASettingButton::init(GenericSetting* pSetting, SettingsPopup* pPopup) {
        if (!ColorSettingButtonBase::init(pSetting, pPopup)) {
            return false;
        }

        setupReloadIndicator(setting<T>()->reloadType());

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

            trySubmitReloadSettingChanged(setting<T>()->reloadType());
        });
        popup->show();
    }
    SE_SETUP_SETTING_BUTTON_CREATE(RGBASettingButton)

    SettingButtonBase* createSettingButton(GenericSetting* pSetting, SettingsPopup* pPopup) {
        switch (pSetting->type()) {
            case SettingType::Bool: return BoolSettingButton::create(pSetting, pPopup);
            case SettingType::Int: return IntSettingButton::create(pSetting, pPopup);
            case SettingType::Float: return FloatSettingButton::create(pSetting, pPopup);
            case SettingType::String: {
                if (static_cast<SillySetting<std::string>*>(pSetting)->isEnum()) {
                    return StrenumSettingButton::create(pSetting, pPopup);
                }
                else {
                    return StringSettingButton::create(pSetting, pPopup);
                }
            }
            case SettingType::RGB: return RGBSettingButton::create(pSetting, pPopup);
            case SettingType::RGBA: return RGBASettingButton::create(pSetting, pPopup);
            default: return nullptr;
        }
    }
}