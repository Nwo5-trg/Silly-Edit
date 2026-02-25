#include <Geode/modify/LevelEditorLayer.hpp>
#include <internal/utils/utils.hpp>
#include "features/shared.hpp"
#include "settings/settings.hpp"

using namespace geode::prelude;
using namespace Utils::Aliases;

class $modify(SharedLevelEditorLayer, LevelEditorLayer) {
    struct Fields {
        CCDrawNode* gridDraw = nullptr;
        CCLayer* gridLayer = nullptr;
        CCDrawNode* overlayDraw = nullptr;
        CCLayer* overlayLayer = nullptr;

        CCLayer* hiddenLayer = nullptr;

        std::vector<CCNodeRGBA*> chromaNodes;

        std::vector<std::move_only_function<void()>> drawFuncs;
    };

    CCLayer* createLayer(std::string_view pID, int pZ) {
        return Utils::setupNode(
            CCLayer::create(),

            SetNodeID{pID},
            SetNodePosition{CCPointZero},
            SetNodeOrder{pZ},
            SetNodeParent{m_objectLayer}
        );
    }
    CCDrawNode* createDrawNode(std::string_view pID, CCNode* pParent) {
        auto drawNode = Utils::setupNode(
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

        constexpr std::array<ccColor3B, 6> gayArray{{
            {255, 128, 128}, {255, 255, 128}, {128, 255, 128},
            {128, 255, 255}, {128, 128, 255}, {255, 128, 255}
        }};

        for (int i = 0; i < 6; i++) {
            auto gayNode = CCNodeRGBA::create();
            
            m_fields->hiddenLayer->addChild(gayNode);
            m_fields->chromaNodes.push_back(gayNode);

            gayNode->runAction(CCRepeatForever::create(
                CCSequence::create(
                    CCTintTo::create(
                        Settings::General::sayoDeviceSensitivity, gayArray[(i + 0) % 6].r,
                        gayArray[(i + 0) % 6].g, gayArray[(i + 0) % 6].b
                    ),
                    CCTintTo::create(
                        Settings::General::sayoDeviceSensitivity, gayArray[(i + 1) % 6].r,
                        gayArray[(i + 1) % 6].g, gayArray[(i + 1) % 6].b
                    ),
                    CCTintTo::create(
                        Settings::General::sayoDeviceSensitivity, gayArray[(i + 2) % 6].r,
                        gayArray[(i + 2) % 6].g, gayArray[(i + 2) % 6].b
                    ),
                    CCTintTo::create(
                        Settings::General::sayoDeviceSensitivity, gayArray[(i + 3) % 6].r,
                        gayArray[(i + 3) % 6].g, gayArray[(i + 3) % 6].b
                    ),
                    CCTintTo::create(
                        Settings::General::sayoDeviceSensitivity, gayArray[(i + 4) % 6].r,
                        gayArray[(i + 4) % 6].g, gayArray[(i + 4) % 6].b
                    ),
                    CCTintTo::create(
                        Settings::General::sayoDeviceSensitivity, gayArray[(i + 5) % 6].r,
                        gayArray[(i + 5) % 6].g, gayArray[(i + 5) % 6].b
                    ), nullptr
                )
            ));
        }
    
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
        if (auto layer = reinterpret_cast<SharedLevelEditorLayer*>(Editor::layer())) {
            return layer->m_fields->gridDraw;
        }

        return nullptr;
    }
    CCLayer* getGridLayer() {
        if (auto layer = reinterpret_cast<SharedLevelEditorLayer*>(Editor::layer())) {
            return layer->m_fields->gridLayer;
        }
        
        return nullptr;
    }
    CCDrawNode* getOverlayDraw() {
        if (auto layer = reinterpret_cast<SharedLevelEditorLayer*>(Editor::layer())) {
            return layer->m_fields->overlayDraw;
        }

        return nullptr;
    }
    CCLayer* getOverlayLayer() {
        if (auto layer = reinterpret_cast<SharedLevelEditorLayer*>(Editor::layer())) {
            return layer->m_fields->overlayLayer;
        }
        
        return nullptr;
    }
    CCLayer* getHiddenLayer() {
        if (auto layer = reinterpret_cast<SharedLevelEditorLayer*>(Editor::layer())) {
            return layer->m_fields->hiddenLayer;
        }

        return nullptr;
    }

    ccColor3B getChroma3B(int pNode) {
        if (auto layer = reinterpret_cast<SharedLevelEditorLayer*>(Editor::layer())) {
            return layer->m_fields->chromaNodes[pNode]->getColor();
        }

        return {};
    }

    void addUpdateFunc(std::move_only_function<void()> pFunc) {
        if (auto layer = reinterpret_cast<SharedLevelEditorLayer*>(Editor::layer())) {
            layer->m_fields->drawFuncs.push_back(std::move(pFunc));
        }
    }
}