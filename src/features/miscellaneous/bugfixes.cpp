#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <internal/utils/utils.hpp>
#include "settings.hpp"

using namespace geode::prelude;

class $modify(EditorUI) {
    void selectObject(GameObject* object, bool ignoreFilter) {
        EditorUI::selectObject(object, ignoreFilter);

        updateObjectInfoLabel();
    }
};