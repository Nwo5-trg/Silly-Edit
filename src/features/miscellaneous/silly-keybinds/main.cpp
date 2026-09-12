#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;

namespace SillyKeybinds {
    void Feature::onEditor() {
        auto self = editor::ui();

        feature.registerKeybind<"toggle-ignore-damage">([self] (bool pDown, bool) {
            if (pDown) {
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
            }
        });

        feature.registerKeybind<"make-object-invisible">([self] (bool pDown, bool) {
            if (pDown && selection::count()) {
                if (Settings::invisibleWithGroup) {
                    bool hasGroup = false;

                    for (auto obj : selection::getExt()) {
                        hasGroup = object::hasGroup(obj, Settings::invisibleWithGroup);

                        if (!hasGroup) {
                            break;
                        }
                    }

                    if (hasGroup) {
                        object::removeGroup(selection::get(), Settings::invisibleWithGroup);
                    }
                    else {
                        object::addGroup(selection::get(), Settings::invisibleWithGroup);
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
            }
        });
    }
}