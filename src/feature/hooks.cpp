#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <nwo5.ui-scaling/include/compat.hpp>
#include <utils/include.hpp>
#include "feature-manager.hpp"

using namespace geode::prelude;
using namespace nwo5::uiscaling::prelude;

class $modify(EditorUI) {
    static void onModify(auto& pSelf) {
        (void)pSelf.setHookPriorityBeforePost("EditorUI::init", "nwo5.ui-scaling");
    }

    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) {
            return false;
        }

        for (auto [_, feature] : Features::FeatureManager::get()->getFeatures()) {
            feature->onEditor();
        }

        for (auto [_, feature] : Features::FeatureManager::get()->getFeatures()) {
            feature->onToggled(feature->enabled());
        }

        this->addEventListener(uiscaling::compat::EditorUI::Changed(), [] (float pScale, auto) {
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