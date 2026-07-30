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
                m_editorLayer->m_ignoreDamage = GameManager::get()->getGameVariable(GameVar::IgnoreDamage);
            }
        });
        nwo5::utils::setupKeybind(this, "miscellaneous-make-object-invisible", [this] (const Keybind&, bool pDown, bool, double) {
            log::error("rawr");
            if (Settings::Miscellaneous::makeObjectInvisibleKeybind.get() && pDown && editor::selection::count()) {
                if (Settings::Miscellaneous::invisibleWithGroup.get()) {
                    bool hasGroup = false;

                    for (auto obj : editor::selection::getExt()) {
                        hasGroup = editor::object::hasGroup(obj, Settings::Miscellaneous::invisibleWithGroup.get());

                        if (!hasGroup) {
                            break;
                        }
                    }

                    log::error("has group {}", hasGroup);
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

                    log::error("is hide {}", isHide);
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