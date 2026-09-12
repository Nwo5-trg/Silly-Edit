#include <utils/include.hpp>
#include "popup.hpp"

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

        m_label = ui::label(m_setting->name(), Font::Default)
            .id("label"_spr)
            .anchor(Anchor::Left)
            .alignment(Label::Alignment::Left)
            .pos(INPUT_SIZE.width + LABEL_PADDING / 2, SIZE.height / 2)
            .parent(this);

        m_inputMenu = ui::menu(true)
            .id("input-menu"_spr)
            .size(INPUT_SIZE)
            .pos(INPUT_SIZE / 2)
            .parent(this);

        m_helpButton = ui::buttonFrame(
            ui::frame::HELP, this, menu_selector(SettingButtonBase::onHelp)
        )
            .id("help-button"_spr)
            .scaleToFit(HELP_BUTTON_SIZE)
            .hide();

        m_resetButton = ui::circleButtonFrame(
            ui::frame::RELOAD, CircleBaseColor::Green, this, menu_selector(SettingButtonBase::onReset)
        )
            .id("reset-button"_spr)
            .scaleToFit(HELP_BUTTON_SIZE)
            .userObject("nwo5.silly-api/tooltip", TooltipInfo::create("Reset setting"))
            .hide();

        m_reloadIndicator = ui::buttonFrame(
            ui::frame::UPDATE, this, nullptr
        )
            .id("reload-button"_spr)
            .scaleToFit(HELP_BUTTON_SIZE)
            .hide();

        m_helpMenu = ui::menu(ui::row()
            .alignment(AxisAlignment::End)
            .gap(HELP_GAP)
            .autoScale(false)
            .grow()
            .reverse()
            .ignoreInvisible()
        )
            .id("help-menu"_spr)
            .height(HELP_BUTTON_SIZE)
            .anchor(Anchor::Right)
            .pos(SIZE.width + HELP_BUTTON_SIZE / 2, SIZE.height)
            .children(
                m_helpButton,
                m_resetButton,
                m_reloadIndicator
            )
            .parent(this);

        return true;
    }
    void SettingButtonBase::onHelp(CCObject*) {
        FLAlertLayer::create(
            "Help",
            m_setting->description(),
            "OK"
        )->show();
    }
    void SettingButtonBase::onReset(CCObject*) {
        this->resetSetting();
    }
    auto SettingButtonBase::getSetting() const {
        return m_setting;
    }

    bool NumberSettingButtonBase::init(GenericSetting* pSetting) {
        if (!SettingButtonBase::init(pSetting)) {
            return false;
        }

        m_input = ui::input(
            INPUT_SIZE.width - INPUT_PADDING, INPUT_SIZE.height * (2.0f / 3.0f) - INPUT_PADDING, std::nullopt
        )
            .id("input"_spr)
            .pos(CCPointZero)
            .parent(m_inputMenu);

        m_label->setFitBox({LABEL_SIZE.width - LABEL_PADDING, LABEL_SIZE.height - LABEL_PADDING}, 0.5f);

        return true;
    }

    bool ColorSettingButtonBase::init(GenericSetting* pSetting) {
        if (!SettingButtonBase::init(pSetting)) {
            return false;
        }

        m_colorFill = ui::spr("color-button-fill.png"_spr);

        ui::button(
            m_colorFill, this, menu_selector(ColorSettingButtonBase::onColorPick)
        )
            .id("color_button"_spr)
            .scaleToFit(INPUT_SIZE.width * (4.0f/5.0f) - INPUT_PADDING)
            .pos(CCPointZero)
            .parent(m_inputMenu);

        m_colorFill->addChildAtPosition(ui::spr("color-button-frame.png"_spr), Anchor::Center);

        m_label->setFitBox({LABEL_SIZE.width - LABEL_PADDING, LABEL_SIZE.height - LABEL_PADDING}, 0.5f);

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

        this->setup<T>();

        m_toggler = ui::togglerBase(
            this, menu_selector(BoolSettingButton::onToggle)
        )
            .id("toggle"_spr)
            .scaleToFit(INPUT_SIZE.width * (4.0f/5.0f) - INPUT_PADDING)
            .pos(CCPointZero)
            .parent(m_inputMenu)
            .toggle(setting->get());
        
        m_label->setFitBox({LABEL_SIZE.width - LABEL_PADDING, LABEL_SIZE.height - LABEL_PADDING}, 0.5f);

        return true;
    }
    void BoolSettingButton::resetSetting() {
        auto setting = this->setting<T>();

        if (setting->get() == setting->getDefault()) {
            return;
        }

        m_toggler->activate();
    }
    void BoolSettingButton::onToggle(cocos2d::CCObject* pSender) {
        auto setting = this->setting<T>();

        this->set<T>(misc::isToggled(pSender));
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

        this->setup<T>();

        std::string placeholder;
        
        if (setting->hasMin()) {
            placeholder.append(fmt::format("{}<=", setting->min()));
        }
        placeholder.append(fmt::format("({})", misc::numToString(setting->getDefault())));
        if (setting->hasMax()) {
            placeholder.append(fmt::format("<={}", setting->max()));
        }

        Setup(m_input)
            .placeholder(placeholder)
            .filter(CommonFilter::Int)
            .string(misc::numToString(setting->get()))
            .callback([this] (const std::string& pStr) {
                auto setting = this->setting<T>();

                if (pStr.empty()) {
                    this->set<T>(setting->getDefault());
                }
                else {
                    this->set<T>(std::clamp(utils::numFromString<T>(pStr).unwrapOrDefault(), setting->min(), setting->max()));
                    
                    if (pStr != "0") {
                        this->m_input->setString(misc::numToString(setting->get()));
                    }
                }

                if (setting->reloadRequired()) {
                    SettingWithReloadChanged().send(setting->key(), setting->reloadType());
                }
            });

        if (const auto labelSize = cocos::getLabelSize(placeholder, Font::Default).width, inputSize = ui::w(m_input); labelSize > inputSize) {
            m_input->getInputNode()->setLabelPlaceholderScale(inputSize / labelSize);
        }

        return true;
    }
    void IntSettingButton::resetSetting() {
        auto setting = this->setting<T>();

        if (setting->get() == setting->getDefault()) {
            return;
        }

        m_input->setString(misc::numToString(setting->getDefault()), true);
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

        this->setup<T>();

        std::string placeholder;
        
        if (setting->hasMin()) {
            placeholder.append(fmt::format("{}<=", setting->min()));
        }
        placeholder.append(fmt::format("({})", misc::numToString(setting->getDefault())));
        if (setting->hasMax()) {
            placeholder.append(fmt::format("<={}", setting->max()));
        }

        Setup(m_input)
            .placeholder(placeholder)
            .filter(CommonFilter::Float)
            .string(misc::numToString(setting->get()))
            .callback([this] (const std::string& pStr) {
                auto setting = this->setting<T>();

                if (pStr.empty()) {
                    this->set<T>(setting->getDefault());
                }
                else {
                    this->set<T>(std::clamp(utils::numFromString<T>(pStr).unwrapOrDefault(), setting->min(), setting->max()));

                    if (!pStr.ends_with('.') && !pStr.ends_with('-') && pStr != "0") {
                        this->m_input->setString(misc::numToString(setting->get()));
                    }
                }

                if (setting->reloadRequired()) {
                    SettingWithReloadChanged().send(setting->key(), setting->reloadType());
                }
            });
        
        if (const auto labelSize = cocos::getLabelSize(placeholder, Font::Default).width, inputSize = ui::w(m_input); labelSize > inputSize) {
            m_input->getInputNode()->setLabelPlaceholderScale(inputSize / labelSize);
        }

        return true;
    }
    void FloatSettingButton::resetSetting() {
        auto setting = this->setting<T>();

        if (setting->get() == setting->getDefault()) {
            return;
        }

        m_input->setString(misc::numToString(setting->getDefault()), true);
    }
    SE_SETUP_SETTING_BUTTON_CREATE(FloatSettingButton)

    bool OpacitySettingButton::init(GenericSetting* pSetting) {
        if (!SettingButtonBase::init(pSetting)) {
            return false;
        }

        auto setting = this->setting<T>();

        if (!setting->validOnPlatform()) {
            return false;
        }

        this->setup<T>();

        m_inputMenu->setPosition(CCPointZero);

        m_slider = Setup(SliderNode::create(nullptr, true))
            .id("input"_spr)
            .pos(SIZE.width / 2, SIZE.height * (3.0f/4.0f) - SLIDER_OFFSET)
            .scaleWidthToFit(SIZE.width * (3.0f/4.0f) - PADDING)
            .callback([this] (auto, auto) {
                auto setting = this->setting<T>();

                this->set<T>(this->m_slider->getPercent() * 255);

                this->updateVisuals();

                if (setting->reloadRequired()) {
                    SettingWithReloadChanged().send(setting->key(), setting->reloadType());
                }
            })
            .callbackSelect([this] (auto, auto) {
                if (auto popup = static_cast<SettingsPopup*>(CCDirector::get()->getRunningScene()->getChildByID("settings-popup"_spr))) {
                    popup->toggleSettingsDrag(false);
                }
            })
            .callbackUnselect([this] (auto, auto) {
                if (auto popup = static_cast<SettingsPopup*>(CCDirector::get()->getRunningScene()->getChildByID("settings-popup"_spr))) {
                    popup->toggleSettingsDrag(true);
                }
            })
            .parent(m_inputMenu);

        m_slider->setPercent(setting->get() / 255.0f);

        Setup(m_label)
            .anchor(Anchor::Center)
            .scaleHeightToFit(SIZE.height / 2 - PADDING / 2)
            .limitScaleWidthToFit(SIZE.width - PADDING)
            .pos(SIZE.width / 2, SIZE.height * (1.0f/4.0f));

        return true;
    }
    void OpacitySettingButton::resetSetting() {
        auto setting = this->setting<T>();

        if (setting->get() == setting->getDefault()) {
            return;
        }

        this->set<T>(setting->getDefault());

        m_slider->setPercent(setting->get() / 255.0f);
        this->updateVisuals();
    }
    void OpacitySettingButton::updateVisuals() {
        auto setting = this->setting<T>();

        m_slider->getBar()->setOpacity(setting->get());
        m_slider->getGroove()->setOpacity(setting->get() ? setting->get() : 255);
        m_slider->getGroove()->setColor(setting->get() ? *Col::White : Col::DarkGray);

        Setup(this->m_label)
            .text(fmt::format("{} ({})", setting->name(), setting->get()))
            .scaleHeightToFit(SIZE.height / 2 - PADDING / 2)
            .limitScaleWidthToFit(SIZE.width - PADDING);
    }
    SE_SETUP_SETTING_BUTTON_CREATE(OpacitySettingButton)

    bool StringSettingButton::init(GenericSetting* pSetting) {
        if (!SettingButtonBase::init(pSetting)) {
            return false;
        }

        auto setting = this->setting<T>();

        if (!setting->validOnPlatform()) {
            return false;
        }

        this->setup<T>();

        m_inputMenu->setPosition(CCPointZero);

        m_input = ui::input(
            SIZE.width - PADDING, SIZE.height / 2 - PADDING / 2, setting->getDefault()
        )
            .id("input"_spr)
            .pos(SIZE.width / 2, SIZE.height * (3.0f/4.0f))
            .parent(m_inputMenu)
            .callback([this] (const std::string& pStr) {
                auto setting = this->setting<T>();

                if (pStr.empty()) {
                    this->set<T>(setting->getDefault());
                }
                else if (pStr == "\\0") {
                    this->set<T>("");
                }
                else {
                    this->set<T>(pStr);
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
    void StringSettingButton::resetSetting() {
        auto setting = this->setting<T>();

        if (setting->get() == setting->getDefault()) {
            return;
        }

        m_input->setString(setting->getDefault(), true);
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

        this->setup<T>();

        m_inputMenu->setPosition(CCPointZero);

        m_currentLabel = ui::label()
            .id("current-label"_spr)
            .pos(SIZE.width / 2, SIZE.height * (3.0f/4.0f))
            .parent(m_inputMenu);

        m_nextArrow = ui::buttonFrame(
            "GJ_arrow_02_001.png", this, menu_selector(StrenumSettingButton::onNext)
        )
            .id("next-button"_spr)
            .pos(SIZE.width - ARROW_SIZE / 2 - PADDING / 2, SIZE.height * (3.0f/4.0f))
            .scaleToFit(ARROW_SIZE)
            .parent(m_inputMenu)
            .flipX();
        
        m_prevArrow = ui::buttonFrame(
            "GJ_arrow_02_001.png", this, menu_selector(StrenumSettingButton::onPrevious)
        )
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
    void StrenumSettingButton::resetSetting() {
        auto setting = this->setting<T>();

        if (setting->get() == setting->getDefault()) {
            return;
        }

        this->setOption(
            std::ranges::find(setting->enumOptions(), setting->getDefault()) - setting->enumOptions().begin(), true
        );
    }
    void StrenumSettingButton::setOption(int pOption, bool pSet) {
        auto setting = this->setting<T>();

        const auto& str = setting->enumOptions()[pOption];

        Setup(m_currentLabel)
            .string(str)
            .scaleHeightToFit(SIZE.height * (2.0f/5.0f) - PADDING / 2)
            .limitScaleWidthToFit(SIZE.width * (3.0f/4.0f) - PADDING);

        if (pSet) {
            this->set<T>(str);

            if (setting->reloadRequired()) {
                SettingWithReloadChanged().send(setting->key(), setting->reloadType());
            }
        }

        m_currentOption = pOption;
    }
    void StrenumSettingButton::onNext(CCObject*) {
        this->setOption((m_currentOption + 1) % this->setting<T>()->enumOptions().size(), true);
    }
    void StrenumSettingButton::onPrevious(CCObject*) {
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

        this->setup<T>();

        m_colorFill->setColor(setting->get());

        return true;
    }
    void RGBSettingButton::resetSetting() {
        auto setting = this->setting<T>();

        if (setting->get() == setting->getDefault()) {
            return;
        }

        this->set<T>(setting->getDefault());

        m_colorFill->setColor(setting->get());

        if (setting->reloadRequired()) {
            SettingWithReloadChanged().send(setting->key(), setting->reloadType());
        }
    }
    void RGBSettingButton::setupColorPicker() {
        auto popup = ColorPickPopup::create(this->setting<T>()->get());

        popup->setCallback([this] (const ccColor4B& pCol) {
            const auto col = to3B(pCol);

            m_colorFill->setColor(col);
        
            auto setting = this->setting<T>();

            this->set<T>(col);

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

        this->setup<T>();

        m_colorFill->setColor(color_cast<ccColor3B>(setting->get()));
        m_colorFill->setOpacity(setting->get().a);

        return true;
    }
    void RGBASettingButton::resetSetting() {
        auto setting = this->setting<T>();

        if (setting->get() == setting->getDefault()) {
            return;
        }

        this->set<T>(setting->getDefault());

        m_colorFill->setColor(color_cast<ccColor3B>(setting->get()));
        m_colorFill->setOpacity(setting->get().a);

        if (setting->reloadRequired()) {
            SettingWithReloadChanged().send(setting->key(), setting->reloadType());
        }
    }
    void RGBASettingButton::setupColorPicker() {
        auto popup = ColorPickPopup::create(this->setting<T>()->get());

        popup->setCallback([this] (const ccColor4B& pCol) {
            m_colorFill->setColor(to3B(pCol));
            m_colorFill->setOpacity(pCol.a);

            auto setting = this->setting<T>();

            this->set<T>(pCol);

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
            case SettingType::Int: {
                if (auto setting = static_cast<SillySetting<int>*>(pSetting); setting->min() == 0 && setting->max() == 255) {
                    return OpacitySettingButton::create(pSetting);
                }
                else {
                    return IntSettingButton::create(pSetting);
                }
            }
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