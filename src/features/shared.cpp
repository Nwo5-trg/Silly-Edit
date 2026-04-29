#include <Geode/modify/LevelEditorLayer.hpp>
#include <internal/utils/utils.hpp>
#include "features/shared.hpp"

using namespace geode::prelude;
using namespace nwo5::utils::setup;

class $modify(SharedLevelEditorLayer, LevelEditorLayer) {
    struct Fields {
        CCDrawNode* gridDraw = nullptr;
        CCLayer* gridLayer = nullptr;
        CCDrawNode* overlayDraw = nullptr;
        CCLayer* overlayLayer = nullptr;

        CCLayer* hiddenLayer = nullptr;

        std::vector<geode::Function<void()>> drawFuncs;
    };

    CCLayer* createLayer(std::string_view pID, int pZ) {
        return nwo5::utils::setupNode(
            CCLayer::create(),

            SetNodeID{pID},
            SetNodePosition{CCPointZero},
            SetNodeOrder{pZ},
            SetNodeParent{m_objectLayer}
        );
    }
    CCDrawNode* createDrawNode(std::string_view pID, CCNode* pParent) {
        auto drawNode = nwo5::utils::setupNode(
            CCDrawNode::create(),

            SetNodeID{pID},
            SetNodePosition{CCPointZero},
            SetNodeParent{pParent}
        );

        drawNode->setBlendFunc({GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA});
        drawNode->m_bUseArea = false;
        
        return drawNode;
    }

    bool init(GJGameLevel* level, bool noUI) {
        if (!LevelEditorLayer::init(level, noUI)) {
            return false;
        }

        m_fields->gridLayer = createLayer("grid-layer"_spr, m_drawGridLayer->getZOrder() + 1);
        m_fields->gridDraw = createDrawNode("grid-draw"_spr, m_fields->gridLayer);
        m_fields->overlayLayer = createLayer("overlay-layer"_spr, m_editorUI->m_scaleControl->getZOrder() - 1);
        m_fields->overlayDraw = createDrawNode("overlay-draw"_spr, m_fields->overlayLayer);
        (m_fields->hiddenLayer = createLayer("hidden-layer"_spr, 0))->setVisible(false);
    
        return true;
    }

    void updateEditor(float dt) {
        LevelEditorLayer::updateEditor(dt);

        auto& fields = m_fields;

        if (!fields->gridDraw) {
            return;
        }

        fields->gridDraw->clear();
        fields->overlayDraw->clear();

        for (auto& func : m_fields->drawFuncs) {
            func();
        }
    }
};

namespace Shared {
    CCDrawNode* getGridDraw() {
        if (auto layer = editor::layer<SharedLevelEditorLayer*>()) {
            return layer->m_fields->gridDraw;
        }

        return nullptr;
    }
    CCLayer* getGridLayer() {
        if (auto layer = editor::layer<SharedLevelEditorLayer*>()) {
            return layer->m_fields->gridLayer;
        }
        
        return nullptr;
    }
    CCDrawNode* getOverlayDraw() {
        if (auto layer = editor::layer<SharedLevelEditorLayer*>()) {
            return layer->m_fields->overlayDraw;
        }

        return nullptr;
    }
    CCLayer* getOverlayLayer() {
        if (auto layer = editor::layer<SharedLevelEditorLayer*>()) {
            return layer->m_fields->overlayLayer;
        }
        
        return nullptr;
    }
    CCLayer* getHiddenLayer() {
        if (auto layer = editor::layer<SharedLevelEditorLayer*>()) {
            return layer->m_fields->hiddenLayer;
        }

        return nullptr;
    }

    void addUpdateFunc(geode::Function<void()> pFunc) {
        if (auto layer = editor::layer<SharedLevelEditorLayer*>()) {
            layer->m_fields->drawFuncs.push_back(std::move(pFunc));
        }
    }
}