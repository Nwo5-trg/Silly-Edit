#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

namespace EasingPreview {
    bool SelectEasingPopup::init(GD::SetupTriggerPopup* pPopup, PreviewType pType, int pProp) {
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

        for (size_t i = 0; i < 19 + 1; i++) {
            const auto toggleButton = i == 19;

            const auto str = toggleButton ? "Toggle Graph" : std::string{GameToolbox::easeToText(i)};

            CCNode* button = nullptr;
            if (toggleButton) {
                if (pType == PreviewType::None) {
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
                button = Setup(EasingButton::create(enum_cast<EasingType>(i), pType, pPopup->m_easingRate, [this, type = i, prop = pProp, popup = pPopup] (EasingType pType) {
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

    void SelectEasingPopup::onGraphModeToggle(CCObject* pSender) {
        for (auto button : m_buttons) {
            button->setGraphMode(misc::isToggled(pSender));
        }
    }

    SelectEasingPopup* SelectEasingPopup::create(GD::SetupTriggerPopup* pPopup, PreviewType pType, int pProp) {
        auto ret = new SelectEasingPopup;

        if (!ret->init(pPopup, pType, pProp)) {
            delete ret;

            return nullptr;
        }

        ret->autorelease();

        return ret;
    }

    void SelectEasingPopup::onClose(cocos2d::CCObject* pSender) {
        Popup::onClose(pSender);
    }
};