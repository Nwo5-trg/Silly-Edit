#include <utils/include.hpp>
#include "include.hpp"

using namespace nwo5::ui::prelude;
using namespace geode::prelude;

namespace EditorPositionBar {
    void EditorUI::showPositonText(int pIndex) {
        if (!EditorPositionBar::showGoToText) {
            return;
        }

        this->addChild(
            TextAlertPopup::create(fmt::format("Position {} loaded.", pIndex), 0.5f, 0.6f, 150, Font::Default), 100, 150
        );
    }
    void EditorUI::moveEditorToSavedPos(CCPoint pPos) {
        editor::layer()->m_objectLayer->setPosition(
            ui::winSize() / 2 + CCPoint{0.0f, m_toolbarHeight / 2} - pPos * editor::zoom()
        );
    }



    void EditorUI::keyDown(cocos2d::enumKeyCodes key, double timestamp) {
        if (!EditorPositionBar::enabled()) {
            GD::EditorUI::keyDown(key, timestamp);
        }

        if (key >= enumKeyCodes::KEY_Zero && key <= enumKeyCodes::KEY_Nine) {
            const auto num = key - enumKeyCodes::KEY_Zero;

            if (sillyedit::utils::modifierDown(EditorPositionBar::createPositionModifier)) {
                const auto objLayerPos = editor::layer()->m_objectLayer->getPosition();
                const auto pos = (ui::winSize() / 2 + CCPoint{0.0f, m_toolbarHeight / 2} - objLayerPos) / editor::zoom();

                m_editorLayer->m_savedPositions[num] = pos;

                this->addChild(
                    TextAlertPopup::create(fmt::format("Position {} saved.", num), 0.5f, 0.6f, 150, Font::Default), 100, 150
                );

                return;
            }

            if (sillyedit::utils::modifierDown(EditorPositionBar::goToPositionModifier) && m_editorLayer->m_savedPositions[num] != CCPointZero) {
                this->showPositonText(num);
                this->moveEditorToSavedPos(m_editorLayer->m_savedPositions[num]);

                return;
            }
        }

        // dont call the original gd one
        if (key >= enumKeyCodes::KEY_Zero && key <= enumKeyCodes::KEY_Nine) {
            if (CCKeyboardDispatcher::get()->getAltKeyPressed() || CCKeyboardDispatcher::get()->getControlKeyPressed()) {
                return;
            }
        }

        GD::EditorUI::keyDown(key, timestamp);
    }





    void Feature::onUpdate() {
        if (!EditorPositionBar::enabled()) {
            return;
        }
        
        auto self = editor::ui<EditorPositionBar::EditorUI>();
        auto fields = self->m_fields.self();

        if (!fields->positionBarMenu) {
            return;
        }

        const std::vector savedPositions{editor::layer()->m_savedPositions};

        if (!fields->lastSavedPositions.has_value() || fields->lastSavedPositions.value() != savedPositions) {
            if (!fields->lastSavedPositions.has_value()) {
                fields->lastSavedPositions = std::vector<CCPoint>{};

                fields->lastSavedPositions.value().resize(
                    savedPositions.size(), CCPoint{std::numeric_limits<float>::max(), std::numeric_limits<float>::max()}
                );
            }

            for (int i = 0; i < savedPositions.size(); i++) {
                const auto pos = savedPositions[i];
                auto button = fields->positionBarMenu->getChildByTag(i);

                if (pos == CCPointZero) {
                    if (button) {
                        button->removeMeAndCleanup();
                    }
                }
                else if (pos != fields->lastSavedPositions.value()[i]) {
                    Setup(EditorPositionButton::create(i, pos))
                        .id("editor-position-{}-button"_spr, i)
                        .order(i)
                        .parent(fields->positionBarMenu);
                }
            }

            fields->positionBarMenu->updateLayout();

            fields->lastSavedPositions = savedPositions;
        }

        const auto scale = self->m_positionSlider->getScale();
        Setup(fields->positionBarMenu)
            .scale(scale * EditorPositionBar::scale)
            .pos(
                ui::x(self->m_positionSlider), ui::y(self->m_positionSlider) - ui::sh(self->m_positionSlider) / 2 + (EditorPositionBar::offset * scale)
            ); 
    }

    void Feature::onToggled(bool pEnabled) {
        auto self = editor::ui<EditorPositionBar::EditorUI>();
        auto fields = self->m_fields.self();

        if (!pEnabled) {
            if (fields->positionBarMenu) {
                self->m_uiItems->removeObject(fields->positionBarMenu);
                fields->positionBarMenu->removeMeAndCleanup();
                fields->positionBarMenu = nullptr;
            }

            return;
        }
        
        fields->positionBarMenu = ui::menu(ui::row()
            .alignment(AxisAlignment::Center)
            .gap(BAR_GAP)
            .autoScale(false)
            .grow()
        )
            .id("position-bar-menu"_spr)
            .anchor(Anchor::Top)
            .parent(self)
            .addTo(self->m_uiItems);

        fields->lastSavedPositions = std::nullopt;
    }

    void Feature::onSettingChanged(std::string pName, GenericSetting*) {
        if (pName == "showBar") {
            // lol
            Feature::onToggled(EditorPositionBar::showBar);
        }
    }
}