#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;

namespace SillyKeybinds {
    void Feature::onEditor() {
        auto self = editor::ui();

        feature.registerKeybind<"toggle-ignore-damage">([self] (bool pDown, bool) {
            if (!pDown) {
                return;
            }

            GameManager::get()->toggleGameVariable(GameVar::IgnoreDamage);

            self->m_editorLayer->m_ignoreDamage = GameManager::get()->getGameVariable(GameVar::IgnoreDamage);

            if (auto p1 = self->m_editorLayer->m_player1) {
                p1->m_ignoreDamage = self->m_editorLayer->m_ignoreDamage;
            }
            if (auto p2 = self->m_editorLayer->m_player2) {
                p2->m_ignoreDamage = self->m_editorLayer->m_ignoreDamage;
            }

            auto alert = TextAlertPopup::create(
                fmt::format("Ignore Damage: {}", GameManager::get()->getGameVariable(GameVar::IgnoreDamage) ? "On" : "Off"),
                0.6f, 0.6f, 100, "chatFont.fnt"
            );
            alert->setLabelColor({0, 255, 0});
            alert->setAlertPosition({0.f, 1.f}, {20.f, -20.f});
            self->addChild(alert, 100, 100);
        });

        feature.registerKeybind<"make-object-invisible">([self] (bool pDown, bool) {
            if (!pDown || selection::empty()) {
                return;
            }

            if (sillyedit::settings::invisibleWithGroup) {
                bool hasGroup = false;

                for (auto obj : selection::getExt()) {
                    hasGroup = object::hasGroup(obj, sillyedit::settings::invisibleWithGroup);

                    if (!hasGroup) {
                        break;
                    }
                }

                if (hasGroup) {
                    object::removeGroup(selection::get(), sillyedit::settings::invisibleWithGroup);
                }
                else {
                    object::addGroup(selection::get(), sillyedit::settings::invisibleWithGroup);
                }
            }
            else {
                bool isHide = false;

                for (auto obj : selection::getExt()) {
                    isHide = obj->m_isHide;

                    if (!isHide) {
                        break;
                    }
                }

                for (auto obj : selection::getExt()) {
                    obj->m_isHide = !isHide;
                }
            }
        });

        feature.registerKeybind<"restart-playtest">([self] (bool pDown, bool pRepeat) {
            if (!pDown || pRepeat || !editor::isPlaytesting()) {
                return;
            }

            editor::ui()->m_playtestStopBtn->activate();
            editor::ui()->m_playtestBtn->activate();
        });

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
        
        feature.registerKeybind<"zoom-in">([self] (bool pDown, bool) {
            if (pDown) {
                self->updateZoom(editor::zoom() / 0.9f);
            }
        });
        feature.registerKeybind<"zoom-out">([self] (bool pDown, bool) {
            if (pDown) {
                self->updateZoom(editor::zoom() * 0.9f);
            }
        });

        feature.registerKeybind<"quarter-move-up">([self] (bool pDown, bool) {
            if (!pDown || selection::empty()) {
                return;
            }

            auto objs = editor::ui()->m_selectedObjects;

            if (auto selectedObj = editor::ui()->m_selectedObject) {
                objs = CCArray::createWithObject(selectedObj);
            }

            object::moveBy(objs, {0.0f, 7.5f});
            editor::update();
        });
        feature.registerKeybind<"quarter-move-left">([self] (bool pDown, bool) {
            if (!pDown || selection::empty()) {
                return;
            }

            auto objs = editor::ui()->m_selectedObjects;

            if (auto selectedObj = editor::ui()->m_selectedObject) {
                objs = CCArray::createWithObject(selectedObj);
            }

            object::moveBy(objs, {-7.5f, 0.0f});
            editor::update();
        });
        feature.registerKeybind<"quarter-move-down">([self] (bool pDown, bool) {
            if (!pDown || selection::empty()) {
                return;
            }

            auto objs = editor::ui()->m_selectedObjects;

            if (auto selectedObj = editor::ui()->m_selectedObject) {
                objs = CCArray::createWithObject(selectedObj);
            }

            object::moveBy(objs, {0.0f, -7.5f});
            editor::update();
        });
        feature.registerKeybind<"quarter-move-right">([self] (bool pDown, bool) {
            if (!pDown || selection::empty()) {
                return;
            }

            auto objs = editor::ui()->m_selectedObjects;

            if (auto selectedObj = editor::ui()->m_selectedObject) {
                objs = CCArray::createWithObject(selectedObj);
            }

            object::moveBy(objs, {7.5f, 0.0f});
            editor::update();
        });
    }
}