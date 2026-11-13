#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

namespace EasingPreview {
    bool SelectEasingPopup::init(GD::SetupTriggerPopup* popup, PreviewType type, int prop) {
        if (!Popup::init(SIZE)) {
            return false;
        }

        this->setTitle("Select Easing", Font::Gold);
        this->setID("select-easing-popup"_spr);

        auto easingButtonMenu = *ui::menu(ui::row()
            .alignment(AxisAlignment::Start)
            .gap(MENU_GAP)
            .crossAlignment(AxisAlignment::End)
            .autoScale(false)
            .grow(false)
        )
            .id("easing-button-menu"_spr)
            .size(MENU_SIZE)
            .layoutAnchor(Anchor::Bottom).layoutAnchorOffsetY(PADDING / 2)
            .parent(m_mainLayer);

        for (int i = 0; i < 19 + 1; i++) {
            const auto toggleButton = i == 19;

            const auto str = toggleButton ? "Toggle Graph" : std::string{GameToolbox::easeToText(i)};

            CCNode* button = nullptr;
            if (toggleButton) {
                if (type == PreviewType::None) {
                    return true;
                }

                button = ui::circleTogglerFrame(
                    "GJ_sTrendingIcon_001.png", CircleBaseColor::Green, "square_01_001.png", CircleBaseColor::Blue,
                    this, menu_selector(SelectEasingPopup::onGraphModeToggle), 1.0f, 0.75f
                )
                    .id("graph-mode-toggle"_spr)
                    .scaleToFit(GRAPH_MODE_TOGGLE_SIZE);

                if (EasingPreview::graphByDefault) {
                    static_cast<CCMenuItemToggler*>(button)->activate();
                }
            }
            else {
                const auto rate = prop == 30 ? popup->m_easingRate : popup->getValue(prop == 242 ? 243 : 249);
                
                button = Setup(EasingButton::create(enum_cast<EasingType>(i), type, rate, [this, type = i, prop = prop, popup = popup] (EasingType type) {
                    popup->valueChanged(prop, type);

                    if (prop == 30) {
                        popup->m_easingType = enum_cast<EasingType>(type);
                        
                        popup->updateEaseLabel();
                        popup->toggleEaseRateVisibility();
                    }
                    else {
                        popup->updateCustomEaseLabel(prop, type);

                        if (auto obj = popup->m_customEasingTags->objectForKey(prop)) {
                            popup->toggleCustomEaseRateVisibility(prop, obj->getTag());
                        }
                    }

                    this->onClose(nullptr);
                }))
                    .id("button"_spr);

                m_buttons.push_back(static_cast<EasingButton*>(button));
            }
            
            ui::menu(ui::column()
                .alignment(AxisAlignment::Center)
                .gap(MENU_GAP)
                .autoScale(false)
                .crossLineAlignment(AxisAlignment::Center)
                .reverse()
                .grow(false)
            )
                .id("{}-menu"_spr, string::toLower(string::replace(str, " ", "-")))
                .size(EasingButton::SIZE.width, EasingButton::SIZE.height + MENU_GAP + LABEL_HEIGHT)
                .children(
                    button,
                    Setup(ui::label(str, Font::Default))
                        .id("label"_spr)
                        .scaleHeightToFit(LABEL_HEIGHT)
                        .limitScaleWidthToFit(EasingButton::SIZE.width)
                )
                .parent(easingButtonMenu);
        }

        return true;
    }

    void SelectEasingPopup::onGraphModeToggle(CCObject* sender) {
        for (auto button : m_buttons) {
            button->setGraphMode(misc::isToggled(sender));
        }
    }

    SelectEasingPopup* SelectEasingPopup::create(GD::SetupTriggerPopup* popup, PreviewType type, int prop) {
        auto ret = new SelectEasingPopup;

        if (!ret->init(popup, type, prop)) {
            delete ret;

            return nullptr;
        }

        ret->autorelease();

        return ret;
    }

    void SelectEasingPopup::onClose(cocos2d::CCObject* sender) {
        Popup::onClose(sender);
    }
};