#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

namespace BetterScale {
    void GJScaleControl::customScale(float pScale, ObjectScaleType pType) {
        const auto num = BetterScale::allowNegative ? pScale : std::abs(pScale);

        switch (pType) {
            case ObjectScaleType::XY: {
                object::scale(selection::get(), num, true, editor::AUTO_CENTER, !m_scaleLocked);

                m_sliderXY->setValue(valueFromScale(num));
            break; }
            case ObjectScaleType::X: {
                object::scaleX(selection::get(), num, true, editor::AUTO_CENTER, !m_scaleLocked);

                m_sliderX->setValue(valueFromScale(num));
            break; }
            case ObjectScaleType::Y: {
                object::scaleY(selection::get(), num, true, editor::AUTO_CENTER, !m_scaleLocked);

                m_sliderY->setValue(valueFromScale(num)); 
            break; }
        }

        editor::update(false);
    }

    void GJScaleControl::updateShortcuts() {
        auto fields = m_fields.self();

        fields->shortcuts.clear();

        fields->shortcutsMenu->removeAllChildren();
        fields->shortcutsXMenu->removeAllChildren();
        fields->shortcutsYMenu->removeAllChildren();

        if (BetterScale::shortcutsString.get().empty()) {
            return;
        }

        const auto split = string::split(BetterScale::shortcutsString.get(), ",");

        for (auto menu : {fields->shortcutsMenu, fields->shortcutsXMenu, fields->shortcutsYMenu}) {
            for (int i = 0; i < split.size(); i++) {
                ui::circleButton(
                    ui::label(split[i], Font::Default), CircleBaseColor::Green, 
                    this, menu_selector(BetterScale::GJScaleControl::onScaleShortcut)
                )
                    .tag(i)
                    .scaleToFit(SHORTCUT_SIZE)
                    .parent(menu);
            }
        }

        for (const auto& str : split) {
            fields->shortcuts.push_back(utils::numFromString<float>(str).unwrapOr(1.0f));
        }
    }

    void GJScaleControl::updateInputValues() {
        auto fields = m_fields.self();

        CCSize max{std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest()};

        for (auto obj : selection::getExt()) {
            max.width = std::max(max.width, obj->m_scaleX);
            max.height = std::max(max.height, obj->m_scaleY);
        }

        fields->scaleInput->setString(misc::numToString(std::max(max.width, max.height)));
        fields->scaleXInput->setString(misc::numToString(max.width));
        fields->scaleYInput->setString(misc::numToString(max.height));
    }

    void GJScaleControl::updateCustomNodes() {
        auto fields = m_fields.self();

        const auto scaleVisible = m_scaleLabel->isVisible();

        for (auto node : fields->scaleNodes) {
            node->setVisible(scaleVisible);
        }
        for (auto node : fields->scaleXYNodes) {
            node->setVisible(!scaleVisible);
        }
        
        if (BetterScale::shortcutsString.get().empty()) {
            fields->extrasMenu->setPositionY(scaleVisible ? DEFAULT_LOCK_HEIGHT : DEFAULT_LOCK_XY_HEIGHT);

            fields->scaleYInput->setPositionY(DEFAULT_LABEL_Y_HEIGHT);
            fields->newScaleYLabel->setPositionY(DEFAULT_LABEL_Y_HEIGHT);
            m_sliderY->setPositionY(DEFAULT_SLIDER_Y_HEIGHT);
        }
        else {
            fields->extrasMenu->setPositionY(
                scaleVisible ? (DEFAULT_LOCK_HEIGHT + SHORTCUT_SPACE) : (DEFAULT_LOCK_XY_HEIGHT + SHORTCUT_SPACE * 2)
            );

            fields->scaleYInput->setPositionY(DEFAULT_LABEL_Y_HEIGHT + SHORTCUT_SPACE);
            fields->newScaleYLabel->setPositionY(DEFAULT_LABEL_Y_HEIGHT + SHORTCUT_SPACE);
            m_sliderY->setPositionY(DEFAULT_SLIDER_Y_HEIGHT + SHORTCUT_SPACE);
        }
    }

    void GJScaleControl::onScaleShortcut(CCObject* pSender) {
        this->customScale(
            m_fields->shortcuts[pSender->getTag()], 
            static_cast<ObjectScaleType>(static_cast<CCNode*>(pSender)->getParent()->getTag())
        );

        this->updateInputValues();
    }

    void GJScaleControl::onSwitchMode(CCObject* pSender) {
        editor::activateScaleControl(!m_scaleLabel->isVisible());
    }



    bool GJScaleControl::init() {
        if (!GD::GJScaleControl::init()) {
            return false;
        }

        if (!BetterScale::enabled()) {
            return true;
        }

        auto fields = m_fields.self();

        m_scaleLabel->setOpacity(0);

        fields->newScaleLabel = ui::label("Scale: ", Font::Default)
            .id("new_scale-label"_spr)
            .scale(LABEL_SCALE)
            .pos(-INPUT_SIZE.width / 2, ui::y(m_scaleLabel))
            .parent(this);
        fields->scaleNodes.push_back(fields->newScaleLabel);

        fields->scaleInput = ui::input(INPUT_SIZE.width, INPUT_SIZE.height, "1")
            .id("scale-input"_spr)
            .pos(fields->newScaleLabel->getScaledContentWidth() / 2, ui::y(m_scaleLabel))
            .callback([this] (const std::string& pStr) {
                if (!pStr.empty() && !pStr.ends_with('.') && pStr != "0") {
                    const auto num = utils::numFromString<float>(pStr).unwrapOrDefault();

                    this->customScale(num, ObjectScaleType::XY);

                    this->updateInputValues();
                }
            })
            .parent(this);
        fields->scaleNodes.push_back(fields->scaleInput);

        fields->shortcutsMenu = ui::menu(ui::row()
            .alignment(AxisAlignment::Center)
            .gap(SHORTCUT_GAP)
            .autoScale(false)
            .grow()
        )
            .id("shortcuts-menu"_spr)
            .tag(static_cast<int>(ObjectScaleType::XY))
            .height(SHORTCUT_SIZE)
            .pos(0.0f, ui::y(fields->newScaleLabel) + SHORTCUT_SPACE)
            .parent(this);

        fields->scaleNodes.push_back(fields->shortcutsMenu);

        m_scaleXLabel->setOpacity(0);

        fields->newScaleXLabel = ui::label("ScaleX: ", Font::Default)
            .id("new-scale-x-label"_spr)
            .scale(LABEL_SCALE)
            .pos(-INPUT_SIZE.width / 2, ui::y(m_scaleXLabel))
            .parent(this);
        fields->scaleXYNodes.push_back(fields->newScaleXLabel);

        fields->scaleXInput = ui::input(INPUT_SIZE.width, INPUT_SIZE.height, "1")
            .id("scale-x-input"_spr)
            .pos(fields->newScaleXLabel->getScaledContentWidth() / 2, ui::y(m_scaleXLabel))
            .callback([this] (const std::string& pStr) {
                if (!pStr.empty() && !pStr.ends_with('.') && pStr != "0") {
                    const auto num = utils::numFromString<float>(pStr).unwrapOrDefault();
                    
                    this->customScale(num, ObjectScaleType::X);

                    this->updateInputValues();
                }
            })
            .parent(this);
        fields->scaleXYNodes.push_back(fields->scaleXInput);

        fields->shortcutsXMenu = ui::menu(ui::row()
            .alignment(AxisAlignment::Center)
            .gap(SHORTCUT_GAP)
            .autoScale(false)
            .grow()
        )
            .id("shortcuts-x-menu"_spr)
            .tag(static_cast<int>(ObjectScaleType::X))
            .height(SHORTCUT_SIZE)
            .pos(0.0f, ui::y(fields->newScaleXLabel) + SHORTCUT_SPACE)
            .parent(this);
        fields->scaleXYNodes.push_back(fields->shortcutsXMenu);

        m_scaleYLabel->setOpacity(0);

        fields->newScaleYLabel = ui::label("ScaleY: ", Font::Default)
            .id("new-scale-y-label"_spr)
            .scale(LABEL_SCALE)
            .pos(-INPUT_SIZE.width / 2, ui::y(m_scaleYLabel))
            .parent(this);
        fields->scaleXYNodes.push_back(fields->newScaleYLabel);

        fields->scaleYInput = ui::input(INPUT_SIZE.width, INPUT_SIZE.height, "1")
            .id("scale-y-input"_spr)
            .pos(fields->newScaleYLabel->getScaledContentWidth() / 2, ui::y(m_scaleYLabel))
            .callback([this] (const std::string& pStr) {
                if (!pStr.empty() && !pStr.ends_with('.') && pStr != "0") {
                    const auto num = utils::numFromString<float>(pStr).unwrapOrDefault();
                    
                    this->customScale(num, ObjectScaleType::Y);

                    this->updateInputValues();
                }
            })
            .parent(this);
        fields->scaleXYNodes.push_back(fields->scaleYInput);

        fields->shortcutsYMenu = ui::menu(ui::row()
            .alignment(AxisAlignment::Center)
            .gap(SHORTCUT_GAP)
            .autoScale(false)
            .grow()
        )
            .id("shortcuts-y-menu"_spr)
            .tag(static_cast<int>(ObjectScaleType::Y))
            .height(SHORTCUT_SIZE)
            .pos(0.0f, DEFAULT_LABEL_Y_HEIGHT + SHORTCUT_SPACE * 2)
            .parent(this);
        fields->scaleXYNodes.push_back(fields->shortcutsYMenu);

        m_scaleLockButton->getParent()->setVisible(false);

        fields->extrasMenu = ui::menu(ui::row()
            .alignment(AxisAlignment::Center)
            .gap(SHORTCUT_GAP)
            .autoScale(false)
            .grow()
        )
            .id("extras-menu"_spr)
            .height(EXTRAS_BUTTON_SIZE)
            .posX(0.0f)
            .parent(this);

        ui::toggler(
            BetterScale::newLockTexture 
                ? CircleButtonSprite::createWithSprite("unlocked-icon.png"_spr, 1.0f, CircleBaseColor::Gray)
                : CCSprite::createWithSpriteFrameName("warpLockOffBtn_001.png"),
            BetterScale::newLockTexture 
                ? CircleButtonSprite::createWithSprite("locked-icon.png"_spr, 1.0f, CircleBaseColor::Blue)
                : CCSprite::createWithSpriteFrameName("warpLockOnBtn_001.png"),
            this, menu_selector(BetterScale::GJScaleControl::onToggleLockScale)
        )
            .id("lock-button"_spr)
            .scaleToFit(EXTRAS_BUTTON_SIZE)
            .parent(fields->extrasMenu);

        if (BetterScale::switchModeButton) {
            ui::circleButtonFrame(
                "GJ_sortIcon_001.png", CircleBaseColor::Pink, this, menu_selector(BetterScale::GJScaleControl::onSwitchMode)
            )
                .id("switch-mode-button"_spr)
                .scaleToFit(EXTRAS_BUTTON_SIZE)
                .parent(fields->extrasMenu);
        }

        this->updateShortcuts();

        fields->betterScaleLoaded = true;

        // laziest solution works so fuck you
        for (auto node : getChildrenExt()) {
            if (const auto id = node->getID().view(); id.contains(TINKER_EDIT_ID) || id.contains(BETTER_EDIT_ID)) {
                // not making invisible cuz that would just be reset so close enough
                node->setScale(0.0f);

                if (auto input = typeinfo_cast<TextInput*>(node)) {
                    input->setEnabled(false);
                }
            }
        }

        return true;
    }

    void GJScaleControl::ccTouchMoved(CCTouch* touch, CCEvent* event) {
        GD::GJScaleControl::ccTouchMoved(touch, event);

        if (m_fields->betterScaleLoaded) {
            this->updateInputValues();
        }
    }





    void EditorUI::activateScaleControl(CCObject* sender) {
        if (!BetterScale::enabled()) {
            return GD::EditorUI::activateScaleControl(sender);
        }

        GD::EditorUI::activateScaleControl(sender);

        if (auto control = reinterpret_cast<BetterScale::GJScaleControl*>(m_scaleControl); control && control->m_fields->betterScaleLoaded) {
            Setup(control)
                .scale(
                    BetterScale::lockControlSize 
                        ? BetterScale::controlSize 
                        : ((1 / editor::zoom()) * BetterScale::controlSize) 
                )
                .pos(selection::center() + CCPoint{0.0f, BetterScale::controlOffset});

            control->updateCustomNodes();
            control->updateInputValues();
        }
    }

    void EditorUI::updateScaleControl() {
        if (!BetterScale::enabled()) {
            return GD::EditorUI::updateScaleControl();
        }

        GD::EditorUI::updateScaleControl();

        if (auto control = reinterpret_cast<BetterScale::GJScaleControl*>(m_scaleControl); control && control->m_fields->betterScaleLoaded) {
            control->updateCustomNodes();
            control->updateInputValues();
        }
    }





    void Feature::onEditor() {
        auto self = editor::ui<BetterScale::EditorUI>();

        feature.registerKeybind<"activate-scale-control">([self] (bool pDown, bool) {
            if (pDown && !selection::empty()) {
                if (self->m_scaleControl && self->m_scaleControl->isVisible() && self->m_scaleControl->m_scaleLabel->isVisible()) {
                    self->deactivateScaleControl();
                }
                else {
                    editor::activateScaleControl(false);
                }
            }
        });
        feature.registerKeybind<"activate-scale-xy-control">([self] (bool pDown, bool) {
            if (pDown && !selection::empty()) {
                if (self->m_scaleControl && self->m_scaleControl->isVisible() && !self->m_scaleControl->m_scaleLabel->isVisible()) {
                    self->deactivateScaleControl();
                }
                else {
                    editor::activateScaleControl(true);
                }
            }
        });
    }
}