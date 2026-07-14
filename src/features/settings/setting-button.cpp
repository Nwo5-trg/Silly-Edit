#include <internal/utils/utils.hpp>
#include "setting-button.hpp"
#include "popup.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

namespace Settings {
    bool SettingButtonBase::init(GenericSetting* pSetting, SettingsPopup* pPopup) {
        if (!CCNode::init()) {
            return false;
        }

        m_popup = pPopup;

        m_setting = pSetting;

        Setup(this)
            .id("{}-setting"_spr, pSetting->key())
            .size(SETTING_BUTTON_SIZE);

        m_label = ui::node(Setup(ui::label(m_setting->name()))
            .id("label"_spr)
            .anchor(LEFT_CENTER_ANCHOR)
            .scaleWidthToFit(DEFAULT_SETTING_LABEL_SIZE.width * (5.0f/6.0f))
            .limitScaleHeightToFit(DEFAULT_SETTING_LABEL_SIZE.height * (2.0f/3.0f))
            .pos(
                DEFAULT_SETTING_INPUT_MENU_SIZE.width + (DEFAULT_SETTING_LABEL_SIZE.width - (DEFAULT_SETTING_LABEL_SIZE.width * (5.0f/6.0f))) / 2,
                DEFAULT_SETTING_LABEL_SIZE.height / 2
            )
            .parent(this)
        );

        m_inputMenu = ui::node(Setup(ui::menu(true))
            .id("input-menu"_spr)
            .size(DEFAULT_SETTING_INPUT_MENU_SIZE)
            .pos(DEFAULT_SETTING_INPUT_MENU_SIZE / 2)
            .parent(this)
        );

        m_helpButton = ui::node(Setup(ui::buttonFrame(
            "GJ_infoIcon_001.png", this, menu_selector(SettingButtonBase::onHelp)
        ))
            .id("help-button"_spr)
            .scaleToFit(HELP_BUTTON_SIZE)
        );
        m_reloadIndicator = ui::node(Setup(ui::buttonFrame(
            "edit_ccwBtn_001.png", this, nullptr
        ))
            .id("reload-button"_spr)
            .scaleToFit(HELP_BUTTON_SIZE)
        );

        m_helpMenu = ui::node(Setup(ui::menu(AxisLayout::create()
            ->setAutoScale(false)
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::Start)
            ->setGrowCrossAxis(false)
            ->setAutoGrowAxis(0.0f)
            ->setGap(HELP_GAP)
        ))
            .id("help-menu"_spr)
            .size(HELP_BUTTON_SIZE, HELP_BUTTON_SIZE)
            .anchor(RIGHT_CENTER_ANCHOR)
            .pos(SETTING_BUTTON_SIZE.width + HELP_BUTTON_SIZE / 2, SETTING_BUTTON_SIZE.height)
            .children(
                m_helpButton,
                m_reloadIndicator
            )
            .parent(this)
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

        m_input = ui::node(Setup(ui::input(
            DEFAULT_SETTING_INPUT_MENU_SIZE.width * (5.0f/6.0f), DEFAULT_SETTING_INPUT_MENU_SIZE.height / 2, std::nullopt
        ))
            .id("input"_spr)
            .pos(CCPointZero)
            .parent(m_inputMenu)
        );

        return true;
    }

    bool ColorSettingButtonBase::init(GenericSetting* pSetting, SettingsPopup* pPopup) {
        if (!SettingButtonBase::init(pSetting, pPopup)) {
            return false;
        }

        m_colorFill = CCSprite::create("color-button-fill.png"_spr);

        ui::node(Setup(ui::button(
            m_colorFill, this, menu_selector(ColorSettingButtonBase::onColorPick)
        ))
            .id("color_button"_spr)
            .scaleToFit(DEFAULT_SETTING_INPUT_MENU_SIZE.width * (2.0f/3.0f))
            .pos(CCPointZero)
            .parent(m_inputMenu)
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

        auto toggler = ui::node(Setup(ui::togglerFrame(
            "GJ_checkOff_001.png", "GJ_checkOn_001.png", this, menu_selector(BoolSettingButton::onToggle)
        ))
            .id("toggle"_spr)
            .scaleToFit(DEFAULT_SETTING_INPUT_MENU_SIZE.width * (2.0f/3.0f))
            .pos(CCPointZero)
            .parent(m_inputMenu)
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

        Setup(m_input)
            .placeholder(nwo5::utils::numToString(setting<T>()->getDefault()))
            .filter(CommonFilter::Int)
            .string(nwo5::utils::numToString(setting<T>()->get()))
            .callback([this] (const std::string& pStr) {
                if (pStr.empty()) {
                    setting<T>()->set(setting<T>()->getDefault());
                }
                else {
                    setting<T>()->set(std::clamp(utils::numFromString<T>(pStr).unwrapOrDefault(), setting<T>()->min(), setting<T>()->max()));
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

        Setup(m_input)
            .placeholder(nwo5::utils::numToString(setting<T>()->getDefault()))
            .filter(CommonFilter::Float)
            .string(nwo5::utils::numToString(setting<T>()->get()))
            .callback([this] (const std::string& pStr) {
                if (pStr.empty()) {
                    setting<T>()->set(setting<T>()->getDefault());
                }
                else {
                    setting<T>()->set(std::clamp(utils::numFromString<T>(pStr).unwrapOrDefault(), setting<T>()->min(), setting<T>()->max()));
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

        auto input = ui::node(Setup(ui::input(
            SETTING_BUTTON_SIZE.width - PADDING * 2, SETTING_BUTTON_SIZE.height / 2 - PADDING, setting<T>()->getDefault()
        ))
            .id("input"_spr)
            .pos(SETTING_BUTTON_SIZE.width / 2, SETTING_BUTTON_SIZE.height * (3.0f/4.0f))
            .parent(m_inputMenu)
            .callback([this] (const std::string& pStr) {
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
            })
            .filter(CommonFilter::Any)
            .string(setting<T>()->get())
        );

        Setup(m_label)
            .anchor(CENTER_ANCHOR)
            .scaleHeightToFit(SETTING_BUTTON_SIZE.height / 2 - PADDING)
            .limitScaleWidthToFit(SETTING_BUTTON_SIZE.width - PADDING * 2)
            .pos(SETTING_BUTTON_SIZE.width / 2, SETTING_BUTTON_SIZE.height * (1.0f/4.0f));

        return true;
    }
    SE_SETUP_SETTING_BUTTON_CREATE(StringSettingButton)

    bool StrenumSettingButton::init(GenericSetting* pSetting, SettingsPopup* pPopup) {
        if (!SettingButtonBase::init(pSetting, pPopup)) {
            return false;
        }

        setupReloadIndicator(setting<T>()->reloadType());

        m_inputMenu->setPosition(CCPointZero);

        m_currentLabel = ui::node(Setup(ui::label())
            .id("current-label"_spr)
            .pos(SETTING_BUTTON_SIZE.width / 2, SETTING_BUTTON_SIZE.height * (3.0f/4.0f))
            .parent(m_inputMenu)
        );

        m_nextArrow = ui::node(Setup(ui::buttonFrame(
            "GJ_arrow_02_001.png", this, menu_selector(StrenumSettingButton::onNext)
        ))
            .id("next-button"_spr)
            .posY(SETTING_BUTTON_SIZE.height * (3.0f/4.0f))
            .scaleToFit(ARROW_SIZE)
            .parent(m_inputMenu)
            .flipX()
        );
        
        m_prevArrow = ui::node(Setup(ui::buttonFrame(
            "GJ_arrow_02_001.png", this, menu_selector(StrenumSettingButton::onPrevious)
        ))
            .id("previous-button"_spr)
            .posY(SETTING_BUTTON_SIZE.height * (3.0f/4.0f))
            .scaleToFit(ARROW_SIZE)
            .parent(m_inputMenu)
        );

        Setup(m_label)
            .anchor(CENTER_ANCHOR)
            .scaleHeightToFit(SETTING_BUTTON_SIZE.height / 2 - PADDING)
            .limitScaleWidthToFit(SETTING_BUTTON_SIZE.width - PADDING * 2)
            .pos(SETTING_BUTTON_SIZE.width / 2, SETTING_BUTTON_SIZE.height * (1.0f/4.0f));

        for (int i = 0; i < setting<T>()->enumOptions().size(); i++) {
            if (setting<T>()->enumOptions()[i] == setting<T>()->get()) {
                setOption(i, false);

                break;
            }
        }

        setOption(std::ranges::find(setting<T>()->enumOptions(), setting<T>()->get()) - setting<T>()->enumOptions().begin(), false);

        return true;
    }
    void StrenumSettingButton::setOption(int pOption, bool pSet) {
        const auto& str = setting<T>()->enumOptions()[pOption];

        Setup(m_currentLabel)
            .scaleHeightToFit(SETTING_BUTTON_SIZE.height / 2 - PADDING)
            .limitScaleWidthToFit(SETTING_BUTTON_SIZE.width * (3.0f/4.0f) - PADDING * 2)
            .string(str);

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