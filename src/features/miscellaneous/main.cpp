#include <Geode/modify/EditorUI.hpp>
#include <internal/utils/utils.hpp>
#include "settings.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

class $modify(MiscellaneousEditorUI, EditorUI) {
    struct Fields {
        CCMenuItemToggler* hideUIToggle = nullptr;
    };

    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) {
            return false;
        }

        nwo5::utils::setupKeybind(this, "miscellaneous-toggle-ignore-damage", [this] (const Keybind&, bool pDown, bool, double) {
            if (Settings::Miscellaneous::ignoreDamageKeybind.get() && pDown) {
                GameManager::get()->toggleGameVariable(GameVar::IgnoreDamage);

                this->m_editorLayer->m_ignoreDamage = GameManager::get()->getGameVariable(GameVar::IgnoreDamage);

                if (auto p1 = this->m_editorLayer->m_player1) {
                    p1->m_ignoreDamage = this->m_editorLayer->m_ignoreDamage;
                }
                if (auto p2 = this->m_editorLayer->m_player2) {
                    p2->m_ignoreDamage = this->m_editorLayer->m_ignoreDamage;
                }

                auto alert = TextAlertPopup::create(
                    fmt::format("Ignore Damage: {}", GameManager::get()->getGameVariable(GameVar::IgnoreDamage) ? "On" : "Off"),
                    0.6f, 0.6f, 100, "chatFont.fnt"
                );
                alert->setLabelColor({0, 255, 0});
                alert->setAlertPosition({0.f, 1.f}, {20.f, -20.f});
                this->addChild(alert, 100, 100);
            }
        });
        nwo5::utils::setupKeybind(this, "miscellaneous-make-object-invisible", [this] (const Keybind&, bool pDown, bool, double) {
            if (Settings::Miscellaneous::makeObjectInvisibleKeybind.get() && pDown && editor::selection::count()) {
                if (Settings::Miscellaneous::invisibleWithGroup.get()) {
                    bool hasGroup = false;

                    for (auto obj : editor::selection::getExt()) {
                        hasGroup = editor::object::hasGroup(obj, Settings::Miscellaneous::invisibleWithGroup.get());

                        if (!hasGroup) {
                            break;
                        }
                    }

                    if (hasGroup) {
                        editor::object::removeGroup(editor::selection::get(), Settings::Miscellaneous::invisibleWithGroup.get());
                    }
                    else {
                        editor::object::addGroup(editor::selection::get(), Settings::Miscellaneous::invisibleWithGroup.get());
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

        auto fields = m_fields.self();
        
        if (Settings::Miscellaneous::hideUIButton.get()) {
            if (auto undoMenu = this->getChildByID("undo-menu")) {
                auto off = CircleButtonSprite::createWithSprite(
                    "eye-on.png"_spr, 1.0f, CircleBaseColor::Green, CircleBaseSize::Tiny
                );

                auto on = CircleButtonSprite::createWithSprite(
                    "eye-off.png"_spr, 1.0f, CircleBaseColor::Gray, CircleBaseSize::Tiny
                );
                on->setOpacity(105);
                static_cast<CCSprite*>(on->getTopNode())->setOpacity(105);

                fields->hideUIToggle = ui::node(Setup(ui::toggler(
                    off, on, this, menu_selector(MiscellaneousEditorUI::onHideUI)
                ))
                    .id("hide-ui-toggle"_spr)
                    .parent(undoMenu)
                );
            }
        }

        return true;
    }

    void onHideUI(CCObject* pSender) {
        this->showUI(!nwo5::utils::isToggled(pSender));
    }

    void showUI(bool show) {
        EditorUI::showUI(show);

        if (auto toggler = m_fields->hideUIToggle) {
            // toggler->toggle(!show);
            toggler->setVisible(!editor::isPlaytesting());
        }
    }
};