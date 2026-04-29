#include "settings.hpp"
#include "include.hpp"
#include <internal/utils/utils.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>

using namespace geode::prelude;
using namespace nwo5::ui::prelude;
using namespace nwo5::utils::setup;

namespace BetterLayers {
    bool EditLayerPopup::init(LayerSettings* pSettings) {
        if (!Popup::init({WIDTH, HEIGHT})) {
            return false;
        }

        m_settings = pSettings;

        m_layer = editor::currentLayer();

        auto opacityLabel = ui::node(
            Setup(ui::label("Opacity:", Font::Default))
                .id("opacity-label"_spr)
                .scaleHeightToFit(LABEL_HEIGHT)
        );
        m_opacityInput = ui::node(
            Setup(ui::input(INPUT_SIZE, "255"))
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
                })
        );
        if (m_settings->getLayerOpacity(m_layer).has_value()) {
            m_opacityInput->setString(nwo5::utils::numToString(m_settings->getLayerOpacity(m_layer).value()));
        }

        auto clearOpacityButton = ui::node(
            Setup(ui::buttonFrame("GJ_trashBtn_001.png", this, menu_selector(EditLayerPopup::onClearOpacity)))
                .id("clear-opacity-button"_spr)
        );

        auto opacityMenu = ui::node(
            Setup(ui::menu(ui::horizontalDistrbLayout(GAP)))
                .id("opacity-menu"_spr)
                .pos(WIDTH / 2, HEIGHT / 2)
                .children(
                    opacityLabel,
                    m_opacityInput,
                    clearOpacityButton
                )
                .parent(m_mainLayer)
        );

        auto hideLayerToggle = ui::node(
            Setup(ui::togglerBase(this, menu_selector(EditLayerPopup::onToggleHidden)))
                .id("hide-layer-toggle"_spr)
                .scaleToFit(BUTTON_SIZE)
                .pos(WIDTH - PADDING - BUTTON_SIZE / 2, HEIGHT / 2)
                .parent(m_buttonMenu)
        );
        hideLayerToggle->toggle(m_settings->isLayerHidden(m_layer));

        auto focusLayerToggle = ui::node(
            Setup(ui::togglerFrame("GJ_starsIcon_gray_001.png", "GJ_starsIcon_001.png", this, menu_selector(EditLayerPopup::onToggleFocused)))
                .id("focus-layer-toggle"_spr)
                .scaleToFit(BUTTON_SIZE)
                .pos(PADDING + BUTTON_SIZE / 2, HEIGHT / 2)
                .parent(m_buttonMenu)
        );
        focusLayerToggle->toggle(m_settings->getFocusedLayer().has_value() && m_settings->getFocusedLayer().value() == m_layer);

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
        if (!Popup::init({WIDTH, HEIGHT})) {
            return false;
        }

        setTitle("Edit Level Layer Settings");

        m_settings = pSettings;

        auto defaultOpacityLabel = ui::node(
            Setup(ui::label("Default Opacity:", Font::Default))
                .id("defualt-opacity-label"_spr)
                .scaleHeightToFit(LABEL_HEIGHT)
        );
        
        m_defaultOpacityInput = ui::node(
            Setup(ui::input(INPUT_SIZE, nwo5::utils::numToString(Settings::BetterLayers::layerOpacity.getDefault())))
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
                })
        );
        if (m_settings->getDefaultOpacity().has_value()) {
            m_defaultOpacityInput->setString(nwo5::utils::numToString(m_settings->getDefaultOpacity().value()));
        }

        auto clearDefaultOpacityButton = ui::node(
            Setup(ui::buttonFrame("GJ_trashBtn_001.png", this, menu_selector(EditAllLayersPopup::onClearDefaultOpacity)))
                .id("clear-default-opacity-button"_spr)
        );

        auto focusedLayerLabel = ui::node(
            Setup(ui::label("Focused Layer:", Font::Default))
                .id("focused-layer-label"_spr)
                .scaleHeightToFit(LABEL_HEIGHT)
        );

        m_focusedLayerInput = ui::node(
            Setup(ui::input(INPUT_SIZE, nwo5::utils::numToString(Settings::BetterLayers::layerOpacity.getDefault())))
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
                })
        );
        if (m_settings->getFocusedLayer().has_value()) {
            m_focusedLayerInput->setString(nwo5::utils::numToString(m_settings->getFocusedLayer().value()));
        }

        auto unfocusLayerButton = ui::node(
            Setup(ui::buttonFrame("GJ_trashBtn_001.png", this, menu_selector(EditAllLayersPopup::onUnfocusLayer)))
                .id("unfocus-layer-button"_spr)
        );

        auto menu = ui::node(
            Setup(ui::menu(RowLayout::create()
                ->setGap(GAP)
                ->setGrowCrossAxis(true)
                ->setAxisAlignment(AxisAlignment::Start)
                ->setCrossAxisAlignment(AxisAlignment::End)
                ->setAutoScale(false)
            ))
                .id("menu"_spr)
                .pos(WIDTH / 2, HEIGHT / 2)
                .width(WIDTH)
                .children(
                    defaultOpacityLabel,
                    m_defaultOpacityInput,
                    clearDefaultOpacityButton,
                    focusedLayerLabel,
                    m_focusedLayerInput,
                    unfocusLayerButton
                )
                .parent(m_mainLayer)
        );

        auto dumbLabel = ui::node(
            Setup(ui::label("layer opacity is from 0-255, 0.75 = 191, 0.5 = 127, 0.25 = 63", Font::Chat))
                .id("label-for-the-children-because-i-dont-wanna-make-the-inputs-convert-from-float-cuz-im-lazy-this-also-pads-out-space-in-the-popup-tho-uwu"_spr)
                .scaleHeightToFit(10.0f)
                .pos(WIDTH / 2, GAP)
                .parent(m_mainLayer)
        );

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