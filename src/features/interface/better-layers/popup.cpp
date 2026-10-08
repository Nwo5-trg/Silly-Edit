#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

namespace BetterLayers {
    bool EditLayerPopup::init(LayerSettings* pSettings) {
        if (!Popup::init(WIDTH, HEIGHT)) {
            return false;
        }

        this->setTitle(fmt::format("Edit Layer ({}) Settings", editor::currentLayer()));

        m_settings = pSettings;

        m_layer = editor::currentLayer();

        m_opacityInput = ui::input(INPUT_SIZE, "255")
            .id("opacity-input"_spr)
            .filter(CommonFilter::Uint)
            .maxCharCount(3)
            .callback([this] (const std::string& pStr) {
                    if (pStr.empty()) {
                        this->m_settings->unsetLayerOpacity(m_layer);
                    }
                    else {
                        const auto num = std::clamp(utils::numFromString<int>(pStr).unwrapOrDefault(), 0, 255);

                        this->m_settings->setLayerOpacity(m_layer, num);

                        if (num == 255) {
                            this->m_opacityInput->setString("255");
                        }
                    }
                });

        if (m_settings->getLayerOpacity(m_layer).has_value()) {
            m_opacityInput->setString(misc::numToString(m_settings->getLayerOpacity(m_layer).value()));
        }

        Setup(ui::menu(ui::row()
            .alignment(AxisAlignment::Start)
            .gap(GAP)
            .autoScale(false)
            .grow()
        ))
            .id("opacity-menu"_spr)
            .pos(WIDTH / 2, HEIGHT / 2)
            .children(
                ui::label("Opacity:", Font::Default)
                    .id("opacity-label"_spr)
                    .scaleHeightToFit(LABEL_HEIGHT),
                m_opacityInput,
                ui::buttonFrame(ui::frame::TRASH_BUTTON, this, menu_selector(EditLayerPopup::onClearOpacity))
                    .id("clear-opacity-button"_spr)
            )
            .parent(m_mainLayer);

        ui::togglerFrame(
            "edit_ePHideBtn_001.png", "edit_ePShowBtn_001.png", this, menu_selector(EditLayerPopup::onToggleHidden)
        )
            .id("hide-layer-toggle"_spr)
            .scaleToFit(BUTTON_SIZE)
            .pos(WIDTH - PADDING - BUTTON_SIZE / 2, HEIGHT / 2)
            .toggle(m_settings->isLayerHidden(m_layer))
            .parent(m_buttonMenu);
        ui::togglerFrame(
            ui::frame::GRAY_STAR, ui::frame::STAR, this, menu_selector(EditLayerPopup::onToggleFocused)
        )
            .id("focus-layer-toggle"_spr)
            .scaleToFit(BUTTON_SIZE)
            .pos(PADDING + BUTTON_SIZE / 2, HEIGHT / 2)
            .toggle(m_settings->getFocusedLayer().has_value() && m_settings->getFocusedLayer().value() == m_layer)
            .parent(m_buttonMenu);

        ui::label(
            "layer opacity is from 0-255, 0.75 = 191, 0.5 = 127, 0.25 = 63", Font::Chat
        )
            .id("label-for-the-children-because-i-dont-wanna-make-the-inputs-convert-from-float-cuz-im-lazy-this-also-pads-out-space-in-the-popup-tho-uwu"_spr)
            .scaleHeightToFit(10.0f)
            .layoutAnchor(Anchor::Bottom).layoutAnchorOffsetY(GAP)
            .parent(m_mainLayer);

        return true;
    }

    void EditLayerPopup::onClearOpacity(cocos2d::CCObject*) {
        m_opacityInput->setString("", true);
    }
    void EditLayerPopup::onToggleHidden(cocos2d::CCObject*) {
        m_settings->setLayerHidden(m_layer, !m_settings->isLayerHidden(m_layer));
    }
    void EditLayerPopup::onToggleFocused(cocos2d::CCObject*) {
        if (m_settings->getFocusedLayer() == m_layer) {
            m_settings->unsetFocusedLayer();
        }
        else {
            m_settings->setFocusedLayer(m_layer);
        }
    }

    EditLayerPopup* EditLayerPopup::create(LayerSettings* pSettings) {
        auto ret = new EditLayerPopup;

        if (!ret->init(pSettings)) {
            delete ret;

            return nullptr;
        }

        ret->autorelease();

        return ret;
    }



    

    bool EditAllLayersPopup::init(LayerSettings* pSettings) {
        if (!Popup::init(WIDTH, HEIGHT)) {
            return false;
        }

        this->setTitle("Edit Level Layer Settings");

        m_settings = pSettings;

        m_defaultOpacityInput = ui::input(
            INPUT_SIZE, misc::numToString(BetterLayers::layerOpacity.getDefault())
        )
            .filter(CommonFilter::Uint)
            .maxCharCount(3)
            .id("default-opacity-input"_spr)
            .callback([this] (const std::string& pStr) {
                if (pStr.empty()) {
                    this->m_settings->unsetDefaultOpacity();
                }
                else {
                    const auto num = std::clamp(utils::numFromString<int>(pStr).unwrapOrDefault(), 0, 255);

                    this->m_settings->setDefaultOpacity(num);

                    if (num == 255) {
                        this->m_defaultOpacityInput->setString("255");
                    }
                }
            });

        if (m_settings->getDefaultOpacity().has_value()) {
            m_defaultOpacityInput->setString(misc::numToString(m_settings->getDefaultOpacity().value()));
        }

        m_focusedLayerInput = ui::input(INPUT_SIZE, "None")
            .id("focused-layer-input"_spr)
            .filter(CommonFilter::Uint)
            .maxCharCount(4)
            .callback([this] (const std::string& pStr) {
                    if (pStr.empty()) {
                        this->m_settings->unsetFocusedLayer();
                    }
                    else {
                        const auto num = std::clamp(utils::numFromString<int>(pStr).unwrapOrDefault(), 0, editor::constants::MAX_LAYERS);

                        this->m_settings->setFocusedLayer(num);

                        if (num == editor::constants::MAX_LAYERS) {
                            this->m_focusedLayerInput->setString("9999");
                        }
                    }
                });

        if (m_settings->getFocusedLayer().has_value()) {
            m_focusedLayerInput->setString(misc::numToString(m_settings->getFocusedLayer().value()));
        }

        Setup(ui::menu(ui::row()
            .alignment(AxisAlignment::Center)
            .gap(GAP)
            .crossAlignment(AxisAlignment::End)
            .autoScale(false)
            .grow(false)
        ))
            .id("menu"_spr)
            .pos(WIDTH / 2, HEIGHT / 2)
            .width(WIDTH)
            .children(
                ui::label("Default Opacity:", Font::Default)
                    .id("defualt-opacity-label"_spr)
                    .scaleHeightToFit(LABEL_HEIGHT),
                m_defaultOpacityInput,
                ui::buttonFrame(
                    ui::frame::TRASH_BUTTON, this, menu_selector(EditAllLayersPopup::onClearDefaultOpacity)
                )
                    .id("clear-default-opacity-button"_spr),
                ui::label("Focused Layer:", Font::Default)
                    .id("focused-layer-label"_spr)
                    .scaleHeightToFit(LABEL_HEIGHT),
                m_focusedLayerInput,
                ui::buttonFrame(ui::frame::TRASH_BUTTON, this, menu_selector(EditAllLayersPopup::onUnfocusLayer))
                    .id("unfocus-layer-button"_spr)
            )
            .parent(m_mainLayer);

        ui::label(
            "layer opacity is from 0-255, 0.75 = 191, 0.5 = 127, 0.25 = 63", Font::Chat
        )
            .id("label-for-the-children-because-i-dont-wanna-make-the-inputs-convert-from-float-cuz-im-lazy-this-also-pads-out-space-in-the-popup-tho-uwu"_spr)
            .scaleHeightToFit(10.0f)
            .layoutAnchor(Anchor::Bottom).layoutAnchorOffsetY(GAP)
            .parent(m_mainLayer);

        return true;
    }

    void EditAllLayersPopup::onClearDefaultOpacity(cocos2d::CCObject*) {
        m_defaultOpacityInput->setString("", true);
    }
    void EditAllLayersPopup::onUnfocusLayer(cocos2d::CCObject*) {
        m_focusedLayerInput->setString("", true);
    }

    EditAllLayersPopup* EditAllLayersPopup::create(LayerSettings* pSettings) {
        auto ret = new EditAllLayersPopup;

        if (!ret->init(pSettings)) {
            delete ret;

            return nullptr;
        }

        ret->autorelease();

        return ret;
    }
}