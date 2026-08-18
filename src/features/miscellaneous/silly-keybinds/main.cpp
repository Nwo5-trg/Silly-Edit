#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;

namespace SillyKeybinds {
    void Feature::onEditor() {
        auto self = editor::ui();

        nwo5::utils::setupKeybind(self, "silly-keybinds-toggle-ignore-damage", [self] (const Keybind&, bool pDown, bool, double) {
            if (SillyKeybinds::enabled() && SillyKeybinds::ignoreDamageKeybind && pDown) {
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

        nwo5::utils::setupKeybind(self, "silly-keybinds-make-object-invisible", [self] (const Keybind&, bool pDown, bool, double) {
            if (SillyKeybinds::enabled() && SillyKeybinds::makeObjectInvisibleKeybind && pDown && editor::selection::count()) {
                if (SillyKeybinds::invisibleWithGroup) {
                    bool hasGroup = false;

                    for (auto obj : editor::selection::getExt()) {
                        hasGroup = editor::object::hasGroup(obj, SillyKeybinds::invisibleWithGroup);

                        if (!hasGroup) {
                            break;
                        }
                    }

                    if (hasGroup) {
                        editor::object::removeGroup(editor::selection::get(), SillyKeybinds::invisibleWithGroup);
                    }
                    else {
                        editor::object::addGroup(editor::selection::get(), SillyKeybinds::invisibleWithGroup);
                    }
                }
                else {
                    bool isHide = false;

                    for (auto obj : editor::selection::getExt()) {
                        isHide = obj->m_isHide;

                        if (!isHide) {
                            break;
                        }
                    }

                    for (auto obj : editor::selection::getExt()) {
                        obj->m_isHide = !isHide;
                    }
                }
            }
        });
    }
}