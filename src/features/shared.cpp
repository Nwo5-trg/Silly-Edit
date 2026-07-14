#include <Geode/modify/LevelEditorLayer.hpp>
#include "features/shared.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

class $modify(SharedLevelEditorLayer, LevelEditorLayer) {
    struct Fields {
        SillyDrawNode* gridDraw = nullptr;
        CCLayer* gridLayer = nullptr;
        SillyDrawNode* overlayDraw = nullptr;
        CCLayer* overlayLayer = nullptr;

        CCLayer* hiddenLayer = nullptr;

        std::vector<geode::Function<void()>> drawFuncs;
    };

    CCLayer* createLayer(std::string_view pID, int pZ) {
        return Setup(CCLayer::create())
            .id(pID)
            .pos(CCPointZero)
            .order(pZ)
            .parent(m_objectLayer);
    }
    SillyDrawNode* createDrawNode(std::string_view pID, CCNode* pParent) {
        return Setup(SillyDrawNode::create())
            .id(pID)
            .pos(CCPointZero)
            .parent(m_objectLayer);
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
    SillyDrawNode* getGridDraw() {
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
    SillyDrawNode* getOverlayDraw() {
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