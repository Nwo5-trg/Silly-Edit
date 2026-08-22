#include <utils/include.hpp>
#include "setting-button.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

namespace Settings {
    bool SettingButtonBase::init(GenericSetting* pSetting) {
        if (!CCNode::init()) {
            return false;
        }

        m_setting = pSetting;

        Setup(this)
            .id("{}-setting"_spr, pSetting->key())
            .size(SIZE);

        m_label = Setup(ui::label(m_setting->name()))
            .id("label"_spr)
            .anchor(Anchor::Right)
            .scaleWidthToFit(LABEL_SIZE.width - LABEL_PADDING)
            .limitScaleHeightToFit(LABEL_SIZE.height - LABEL_PADDING)
            .pos(
                SIZE.width - LABEL_PADDING / 2,
                SIZE.height / 2
            )
            .parent(this);

        m_inputMenu = Setup(ui::menu(true))
            .id("input-menu"_spr)
            .size(INPUT_SIZE)
            .pos(INPUT_SIZE / 2)
            .parent(this);

        m_helpButton = Setup(ui::buttonFrame(
            "GJ_infoIcon_001.png", this, menu_selector(SettingButtonBase::onHelp)
        ))
            .id("help-button"_spr)
            .scaleToFit(HELP_BUTTON_SIZE);

        m_reloadIndicator = Setup(ui::buttonFrame(
            "edit_ccwBtn_001.png", this, nullptr
        ))
            .id("reload-button"_spr)
            .scaleToFit(HELP_BUTTON_SIZE);

        m_helpMenu = Setup(ui::menu(ui::row(AxisAlignment::End, HELP_GAP)
            .autoScale(false)
            .grow(true)
            .reverse()
        ))
            .id("help-menu"_spr)
            .height(HELP_BUTTON_SIZE)
            .anchor(Anchor::Right)
            .pos(SIZE.width + HELP_BUTTON_SIZE / 2, SIZE.height)
            .children(
                m_helpButton,
                m_reloadIndicator
            )
            .parent(this);

        return true;
    }
    void SettingButtonBase::setupReloadIndicator(SettingReload pReload) {
        // idek it wasnt working now it is and im not sure this is a reason - it shoudlnt be the reason - but im scared
        const auto shouldShowHelp = m_setting->hasDescription();
        m_helpButton->setVisible(shouldShowHelp);

        if (shouldShowHelp) {
            const auto str = m_setting->description();
            m_helpButton->setUserObject("nwo5.silly-api/tooltip", TooltipInfo::create(string::toUpper(str.substr(0, 1)) + str.substr(1)));
        }

        if (pReload == SettingReload::None) {
            m_reloadIndicator->setVisible(false);

            return m_helpMenu->updateLayout();
        }

        m_helpMenu->updateLayout();

        switch (pReload) {
            case SettingReload::Editor: {
                Setup(m_reloadIndicator)
                    .color(ccORANGE)
                    .callback([] (auto*) {
                        Notification::create("editor reload is required to apply setting !", NotificationIcon::Info)->show();
                    })
                    .userObject("nwo5.silly-api/tooltip", TooltipInfo::create("Editor reload required"));
            break; }
            case SettingReload::Pause: {
                Setup(m_reloadIndicator)
                    .color(ccRED)
                    .callback([] (auto*) {
                        Notification::create("pause menu reload is required to apply setting !", NotificationIcon::Info)->show();
                    })
                    .userObject("nwo5.silly-api/tooltip", TooltipInfo::create("Pause reload required"));
            break; }
            case SettingReload::Popup: {
                Setup(m_reloadIndicator)
                    .color(ccBLUE)
                    .callback([] (auto*) {
                        Notification::create("settings popup reload is required to apply setting !", NotificationIcon::Info)->show();
                    })
                    .userObject("nwo5.silly-api/tooltip", TooltipInfo::create("Popup reload required"));
            break; }
            case SettingReload::Game: {
                Setup(m_reloadIndicator)
                    .color(ccGRAY)
                    .callback([] (auto*) {
                        Notification::create("game reload is required to apply setting !", NotificationIcon::Info)->show();
                    })
                    .userObject("nwo5.silly-api/tooltip", TooltipInfo::create("Game reload required"));
            break; }
            default: return;
        }
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

        m_input = Setup(ui::input(
            INPUT_SIZE.width - INPUT_PADDING, INPUT_SIZE.height * (2.0f / 3.0f) - INPUT_PADDING, std::nullopt
        ))
            .id("input"_spr)
            .pos(CCPointZero)
            .parent(m_inputMenu);

        return true;
    }

    bool ColorSettingButtonBase::init(GenericSetting* pSetting) {
        if (!SettingButtonBase::init(pSetting)) {
            return false;
        }

        m_colorFill = CCSprite::create("color-button-fill.png"_spr);

        Setup(ui::button(
            m_colorFill, this, menu_selector(ColorSettingButtonBase::onColorPick)
        ))
            .id("color_button"_spr)
            .scaleToFit(INPUT_SIZE.width - INPUT_PADDING)
            .pos(CCPointZero)
            .parent(m_inputMenu);

        m_colorFill->addChildAtPosition(CCSprite::create("color-button-frame.png"_spr), Anchor::Center);

        return true;
    }
    void ColorSettingButtonBase::onColorPick(CCObject* pSender) {
        this->setupColorPicker();
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

        auto setting = this->setting<T>();

        if (!setting->validOnPlatform()) {
            return false;
        }

        this->setupReloadIndicator(setting->reloadType());

        Setup(ui::togglerFrame(
            "GJ_checkOff_001.png", "GJ_checkOn_001.png", this, menu_selector(BoolSettingButton::onToggle)
        ))
            .id("toggle"_spr)
            .scaleToFit(INPUT_SIZE.width * (3.0f/4.0f) - INPUT_PADDING)
            .pos(CCPointZero)
            .parent(m_inputMenu)
            .toggle(setting->get());

        return true;
    }
    void BoolSettingButton::onToggle(cocos2d::CCObject* pSender) {
        auto setting = this->setting<T>();

        setting->set(nwo5::utils::isToggled(pSender));
        if (setting->reloadRequired()) {
            SettingWithReloadChanged().send(setting->key(), setting->reloadType());
        }
    }
    SE_SETUP_SETTING_BUTTON_CREATE(BoolSettingButton)

    bool IntSettingButton::init(GenericSetting* pSetting) {
        if (!NumberSettingButtonBase::init(pSetting)) {
            return false;
        }

        auto setting = this->setting<T>();

        if (!setting->validOnPlatform()) {
            return false;
        }

        this->setupReloadIndicator(setting->reloadType());

        Setup(m_input)
            .placeholder(nwo5::utils::numToString(setting->getDefault()))
            .filter(CommonFilter::Int)
            .string(nwo5::utils::numToString(setting->get()))
            .callback([this] (const std::string& pStr) {
                auto setting = this->setting<T>();

                if (pStr.empty()) {
                    setting->set(setting->getDefault());
                }
                else {
                    setting->set(std::clamp(utils::numFromString<T>(pStr).unwrapOrDefault(), setting->min(), setting->max()));
                    this->m_input->setString(nwo5::utils::numToString(setting->get()));
                }

                if (setting->reloadRequired()) {
                    SettingWithReloadChanged().send(setting->key(), setting->reloadType());
                }
            });

        return true;
    }
    SE_SETUP_SETTING_BUTTON_CREATE(IntSettingButton)

    bool FloatSettingButton::init(GenericSetting* pSetting) {
        if (!NumberSettingButtonBase::init(pSetting)) {
            return false;
        }

        auto setting = this->setting<T>();

        if (!setting->validOnPlatform()) {
            return false;
        }

        this->setupReloadIndicator(setting->reloadType());

        Setup(m_input)
            .placeholder(nwo5::utils::numToString(setting->getDefault()))
            .filter(CommonFilter::Float)
            .string(nwo5::utils::numToString(setting->get()))
            .callback([this] (const std::string& pStr) {
                auto setting = this->setting<T>();

                if (pStr.empty()) {
                    setting->set(setting->getDefault());
                }
                else {
                    setting->set(std::clamp(utils::numFromString<T>(pStr).unwrapOrDefault(), setting->min(), setting->max()));

                    if (!pStr.ends_with('.') && !pStr.ends_with('-')) {
                        this->m_input->setString(nwo5::utils::numToString(setting->get()));
                    }
                }

                if (setting->reloadRequired()) {
                    SettingWithReloadChanged().send(setting->key(), setting->reloadType());
                }
            });

        return true;
    }
    SE_SETUP_SETTING_BUTTON_CREATE(FloatSettingButton)

    bool StringSettingButton::init(GenericSetting* pSetting) {
        if (!SettingButtonBase::init(pSetting)) {
            return false;
        }

        auto setting = this->setting<T>();

        if (!setting->validOnPlatform()) {
            return false;
        }

        this->setupReloadIndicator(setting->reloadType());

        m_inputMenu->setPosition(CCPointZero);

        Setup(ui::input(
            SIZE.width - PADDING, SIZE.height / 2 - PADDING / 2, setting->getDefault()
        ))
            .id("input"_spr)
            .pos(SIZE.width / 2, SIZE.height * (3.0f/4.0f))
            .parent(m_inputMenu)
            .callback([this] (const std::string& pStr) {
                auto setting = this->setting<T>();

                if (pStr.empty()) {
                    setting->set(setting->getDefault());
                }
                else if (pStr == "\\0") {
                    setting->set("");
                }
                else {
                    setting->set(pStr);
                }

                if (setting->reloadRequired()) {
                    SettingWithReloadChanged().send(setting->key(), setting->reloadType());
                }
            })
            .filter(CommonFilter::Any)
            .string(this->setting<T>()->get());

        Setup(m_label)
            .anchor(Anchor::Center)
            .scaleHeightToFit(SIZE.height / 2 - PADDING / 2)
            .limitScaleWidthToFit(SIZE.width - PADDING)
            .pos(SIZE.width / 2, SIZE.height * (1.0f/4.0f));

        return true;
    }
    SE_SETUP_SETTING_BUTTON_CREATE(StringSettingButton)

    bool StrenumSettingButton::init(GenericSetting* pSetting) {
        if (!SettingButtonBase::init(pSetting)) {
            return false;
        }

        auto setting = this->setting<T>();

        if (!setting->validOnPlatform()) {
            return false;
        }

        this->setupReloadIndicator(setting->reloadType());

        m_inputMenu->setPosition(CCPointZero);

        m_currentLabel = Setup(ui::label())
            .id("current-label"_spr)
            .pos(SIZE.width / 2, SIZE.height * (3.0f/4.0f))
            .parent(m_inputMenu);

        m_nextArrow = Setup(ui::buttonFrame(
            "GJ_arrow_02_001.png", this, menu_selector(StrenumSettingButton::onNext)
        ))
            .id("next-button"_spr)
            .pos(SIZE.width - ARROW_SIZE / 2 - PADDING / 2, SIZE.height * (3.0f/4.0f))
            .scaleToFit(ARROW_SIZE)
            .parent(m_inputMenu)
            .flipX();
        
        m_prevArrow = Setup(ui::buttonFrame(
            "GJ_arrow_02_001.png", this, menu_selector(StrenumSettingButton::onPrevious)
        ))
            .id("previous-button"_spr)
            .pos(0.0f + ARROW_SIZE / 2 + PADDING / 2, SIZE.height * (3.0f/4.0f))
            .scaleToFit(ARROW_SIZE)
            .parent(m_inputMenu);

        Setup(m_label)
            .anchor(Anchor::Center)
            .scaleHeightToFit(SIZE.height / 2 - PADDING / 2)
            .limitScaleWidthToFit(SIZE.width - PADDING)
            .pos(SIZE.width / 2, SIZE.height * (1.0f/4.0f));

        for (int i = 0; i < setting->enumOptions().size(); i++) {
            if (setting->enumOptions()[i] == setting->get()) {
                this->setOption(i, false);

                break;
            }
        }

        this->setOption(
            std::ranges::find(setting->enumOptions(), setting->get()) - setting->enumOptions().begin(), false
        );

        return true;
    }
    void StrenumSettingButton::setOption(int pOption, bool pSet) {
        auto setting = this->setting<T>();

        const auto& str = setting->enumOptions()[pOption];

        Setup(m_currentLabel)
            .string(str)
            .scaleHeightToFit(SIZE.height * (2.0f/5.0f) - PADDING / 2)
            .limitScaleWidthToFit(SIZE.width * (3.0f/4.0f) - PADDING);

        // m_nextArrow->setPositionX(SIZE.width / 2 + m_currentLabel->getScaledContentWidth() / 2 + ARROW_GAP);
        // m_prevArrow->setPositionX(SIZE.width / 2 - m_currentLabel->getScaledContentWidth() / 2 - ARROW_GAP);

        if (pSet) {
            setting->set(str);

            if (setting->reloadRequired()) {
                SettingWithReloadChanged().send(setting->key(), setting->reloadType());
            }
        }

        m_currentOption = pOption;
    }
    void StrenumSettingButton::onNext(CCObject* pSender) {
        this->setOption((m_currentOption + 1) % this->setting<T>()->enumOptions().size(), true);
    }
    void StrenumSettingButton::onPrevious(CCObject* pSender) {
        this->setOption(m_currentOption ? (m_currentOption - 1) % this->setting<T>()->enumOptions().size() : this->setting<T>()->enumOptions().size() - 1, true);
    }
    SE_SETUP_SETTING_BUTTON_CREATE(StrenumSettingButton)

    bool RGBSettingButton::init(GenericSetting* pSetting) {
        if (!ColorSettingButtonBase::init(pSetting)) {
            return false;
        }

        auto setting = this->setting<T>();

        if (!setting->validOnPlatform()) {
            return false;
        }

        this->setupReloadIndicator(setting->reloadType());

        m_colorFill->setColor(setting->get());

        return true;
    }
    void RGBSettingButton::setupColorPicker() {
        auto popup = ColorPickPopup::create(this->setting<T>()->get());

        popup->setCallback([this] (const ccColor4B& pCol) {
            const auto col = to3B(pCol);

            m_colorFill->setColor(col);
        
            auto setting = this->setting<T>();

            setting->set(col);

            if (setting->reloadRequired()) {
                SettingWithReloadChanged().send(setting->key(), setting->reloadType());
            }
        });

        popup->show();
    }
    SE_SETUP_SETTING_BUTTON_CREATE(RGBSettingButton)

    bool RGBASettingButton::init(GenericSetting* pSetting) {
        if (!ColorSettingButtonBase::init(pSetting)) {
            return false;
        }

        auto setting = this->setting<T>();

        if (!setting->validOnPlatform()) {
            return false;
        }

        this->setupReloadIndicator(setting->reloadType());

        m_colorFill->setColor(color_cast<ccColor3B>(setting->get()));
        m_colorFill->setOpacity(setting->get().a);

        return true;
    }
    void RGBASettingButton::setupColorPicker() {
        auto popup = ColorPickPopup::create(this->setting<T>()->get());

        popup->setCallback([this] (const ccColor4B& pCol) {
            m_colorFill->setColor(to3B(pCol));
            m_colorFill->setOpacity(pCol.a);

            auto setting = this->setting<T>();

            setting->set(pCol);

            if (setting->reloadRequired()) {
                SettingWithReloadChanged().send(setting->key(), setting->reloadType());
            }
        });
        popup->show();
    }
    SE_SETUP_SETTING_BUTTON_CREATE(RGBASettingButton)

    SettingButtonBase* createSettingButton(GenericSetting* pSetting) {
        switch (pSetting->type()) {
            case SettingType::Bool: return BoolSettingButton::create(pSetting);
            case SettingType::Int: return IntSettingButton::create(pSetting);
            case SettingType::Float: return FloatSettingButton::create(pSetting);
            case SettingType::String: {
                if (static_cast<SillySetting<std::string>*>(pSetting)->isEnum()) {
                    return StrenumSettingButton::create(pSetting);
                }
                else {
                    return StringSettingButton::create(pSetting);
                }
            }
            case SettingType::RGB: return RGBSettingButton::create(pSetting);
            case SettingType::RGBA: return RGBASettingButton::create(pSetting);
            default: return nullptr;
        }
    }
}