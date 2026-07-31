#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <internal/utils/utils.hpp>
#include "settings.hpp"

using namespace geode::prelude;

class $modify(EditorUI) {
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
        

        return true;
    }

    void selectObject(GameObject* object, bool ignoreFilter) {
        EditorUI::selectObject(object, ignoreFilter);

        if (Settings::Miscellaneous::fixObjectLabel.get()) {
            updateObjectInfoLabel();
        }
    } 
};