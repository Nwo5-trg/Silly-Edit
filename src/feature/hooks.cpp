#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <nwo5.ui-scaling/include/include.hpp>
#include <utils/include.hpp>
#include "feature-manager.hpp"

using namespace geode::prelude;

class $modify(EditorUI) {
    static void onModify(auto& pSelf) {
        (void)pSelf.setHookPriorityAfterPost("EditorUI::init", nwo5::utils::TINKER_EDIT_ID);
    }

    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) {
            return false;
        }

        for (auto [_, feature] : Features::FeatureManager::get()->getFeatures()) {
            feature->onEditor();
        }

        this->addEventListener(nwo5::uiscaling::EditorUIScaleChanged(), [] (float pScale) {
            for (auto [_, feature] : Features::FeatureManager::get()->getFeatures()) {
                feature->onUIUpdated(pScale);
            }
        });
        
        return true;
    }
};

class $modify(LevelEditorLayer) {
    void updateEditor(float dt) {
        LevelEditorLayer::updateEditor(dt);

        for (auto [_, feature] : Features::FeatureManager::get()->getFeatures()) {
            feature->onUpdate();
        }
    }
};