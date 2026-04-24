#include "settings.hpp"
#include "include.hpp"
#include <internal/utils/utils.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>

using namespace geode::prelude;
using namespace nwo5::syntax;

namespace BetterLayers {
    bool EditLayerPopup::init(LayerSettings* pSettings) {
        if (!Popup::init({WIDTH, HEIGHT})) {
            return false;
        }

        m_settings = pSettings;

        m_layer = editor::currentLayer();

        auto opacityLabel = nwo5::utils::setupNode(
            CCLabelBMFont::create("Opacity:", "bigFont.fnt"),

            SetNodeID{"opacity-label"_spr},
            SetNodeScaleWithHeight{LABEL_HEIGHT},
            SetNodePositionY{HEIGHT / 2},
            SetNodeParent{m_mainLayer}
        );
        m_opacityInput = nwo5::utils::setupNode(
            nwo5::utils::createTextInput(INPUT_SIZE, "255", [this] (const std::string& pStr) {
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
            }),

            SetNodeID{"opacity-input"_spr},
            SetNodePositionY{HEIGHT / 2},
            SetNodeParent{m_mainLayer}
        );

        if (m_settings->getLayerOpacity(m_layer).has_value()) {
            m_opacityInput->setString(nwo5::utils::numToString(m_settings->getLayerOpacity(m_layer).value()));
        }
        m_opacityInput->setCommonFilter(CommonFilter::Uint);
        m_opacityInput->setMaxCharCount(3);

        opacityLabel->setPositionX(WIDTH / 2 - m_opacityInput->getScaledContentWidth() / 2 - PADDING / 2);
        m_opacityInput->setPositionX(WIDTH / 2 + opacityLabel->getScaledContentWidth() / 2 + PADDING / 2);

        auto hideLayerToggle = nwo5::utils::setupNode(
            CCMenuItemToggler::createWithStandardSprites(this, menu_selector(EditLayerPopup::onToggleHidden), 1.0f),

            SetNodeID{"hide-layer-toggle"_spr},
            SetNodeScaleWithSize{BUTTON_SIZE},
            SetNodePosition{WIDTH - PADDING - BUTTON_SIZE / 2, HEIGHT / 2},
            SetNodeParent{m_buttonMenu}
        );

        hideLayerToggle->toggle(m_settings->isLayerHidden(m_layer));

        auto focusLayerToggle = nwo5::utils::setupNode(
            CCMenuItemToggler::create(
                CCSprite::createWithSpriteFrameName("GJ_starsIcon_gray_001.png"),
                CCSprite::createWithSpriteFrameName("GJ_starsIcon_001.png"),
                this, menu_selector(EditLayerPopup::onToggleFocused)
            ),

            SetNodeID{"focus-layer-toggle"_spr},
            SetNodeScaleWithSize{BUTTON_SIZE},
            SetNodePosition{PADDING + BUTTON_SIZE / 2, HEIGHT / 2},
            SetNodeParent{m_buttonMenu}
        );

        focusLayerToggle->toggle(m_settings->getFocusedLayer().has_value() && m_settings->getFocusedLayer().value() == m_layer);

        return true;
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

        auto defaultOpacityLabel = nwo5::utils::setupNode(
            CCLabelBMFont::create("Default Opacity:", "bigFont.fnt"),

            SetNodeID{"default-opacity-label"_spr},
            SetNodeScaleWithHeight{LABEL_HEIGHT}
        );
        m_defaultOpacityInput = nwo5::utils::setupNode(
            nwo5::utils::createTextInput(INPUT_SIZE, nwo5::utils::numToString(Settings::BetterLayers::layerOpacity.getDefault()), [this] (const std::string& pStr) {
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
            }),

            SetNodeID{"default-opacity-input"_spr}
        );

        if (m_settings->getDefaultOpacity().has_value()) {
            m_defaultOpacityInput->setString(nwo5::utils::numToString(m_settings->getDefaultOpacity().value()));
        }
        m_defaultOpacityInput->setCommonFilter(CommonFilter::Uint);
        m_defaultOpacityInput->setMaxCharCount(3);

        auto focusedLayerLabel = nwo5::utils::setupNode(
            CCLabelBMFont::create("Focused Layer:", "bigFont.fnt"),

            SetNodeID{"focused-layer-label"_spr},
            SetNodeScaleWithHeight{LABEL_HEIGHT}
        );
        m_focusedLayerInput = nwo5::utils::setupNode(
            nwo5::utils::createTextInput(INPUT_SIZE, nwo5::utils::numToString(Settings::BetterLayers::layerOpacity.getDefault()), [this] (const std::string& pStr) {
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
            }),

            SetNodeID{"focused-layer-input"_spr}
        );

        if (m_settings->getFocusedLayer().has_value()) {
            m_focusedLayerInput->setString(nwo5::utils::numToString(m_settings->getFocusedLayer().value()));
        }
        m_focusedLayerInput->setCommonFilter(CommonFilter::Uint);
        m_focusedLayerInput->setMaxCharCount(4);

        auto unfocusLayerButton = nwo5::utils::setupNode(
            nwo5::utils::createButtonFrame("GJ_trashBtn_001.png", this, menu_selector(EditAllLayersPopup::onUnfocusLayer))
        );

        auto menu = nwo5::utils::setupNode(
            CCMenu::create(),

            SetNodeID{"menu"_spr},
            SetNodePosition{WIDTH / 2, HEIGHT / 2},
            SetNodeWidth{
                std::max(defaultOpacityLabel->getScaledContentWidth(), focusedLayerLabel->getScaledContentWidth()) + GAP + INPUT_SIZE + GAP + BUTTON_SIZE
            },
            SetNodeChildren{
                defaultOpacityLabel,
                m_defaultOpacityInput,
                focusedLayerLabel,
                m_focusedLayerInput,
                unfocusLayerButton
            },
            SetNodeParent{m_mainLayer}
        );

        menu->setLayout(RowLayout::create()
            ->setGap(GAP)
            ->setGrowCrossAxis(true)
            ->setAxisAlignment(AxisAlignment::Start)
            ->setCrossAxisAlignment(AxisAlignment::End)
            ->setAutoScale(false)
        );

        nwo5::utils::setupNode(
            CCLabelBMFont::create("layer opacity is from 0-255, 0.75 = 191, 0.5 = 127, 0.25 = 63", "chatFont.fnt"),

            SetNodeID{"label-for-the-children-because-i-dont-wanna-make-the-inputs-convert-from-float-cuz-im-lazy-this-also-pads-out-space-in-the-popup-tho-uwu"_spr},
            SetNodeScaleWithHeight{10.0f},
            SetNodePosition{WIDTH / 2, GAP},
            SetNodeParent{m_mainLayer}
        );

        return true;
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