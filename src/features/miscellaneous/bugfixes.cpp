#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <internal/utils/utils.hpp>
#include "settings.hpp"

using namespace geode::prelude;

class $modify(EditorUI) {
    void clickOnPosition(CCPoint position) {
        EditorUI::clickOnPosition(position);

        if (Settings::Miscellaneous::minorBugfixes.get()) {
            updateObjectInfoLabel();
        }
    }
};

class $modify(LevelEditorLayer) {
};