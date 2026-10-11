#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include "feature-manager.hpp"

using namespace geode::prelude;

class $modify(EditorUI) {
    static void onModify(auto& pSelf) {
        (void)pSelf.setHookPriorityBeforePost("EditorUI::init", "nwo5.ui-scaling");
    }

    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) {
            return false;
        }

        sillyedit::features::FeatureManager::get()->onEditorInit();
        
        return true;
    }
};

class $modify(LevelEditorLayer) {
    void updateEditor(float dt) {
        LevelEditorLayer::updateEditor(dt);

        sillyedit::features::FeatureManager::get()->onEditorUpdate();
    }
};