#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/EditorUI.hpp>
#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

namespace Sillyedit {
    class $modify(SharedEditorUI, EditorUI) {
        struct Fields {
            PlaybackMode playbackModeRet = PlaybackMode::Not;
            bool blockScrolling = false;
        };

        static void onModify(auto& pSelf) {
            (void)pSelf.setHookPriorityBeforePre("EditorUI::scrollWheel", TINKER_EDIT_ID);
        }

        bool init(LevelEditorLayer* editorLayer) {
            if (!EditorUI::init(editorLayer)) {
                return false;
            }

            // evil hack to get around tinkers scrolling reimpl (its either this or i stop propgating the event entirely ok :sob:)
            this->addEventListener(ScrollWheelEvent(), [this] (double, double) {
                if (shouldBlockScrolling()) {
                    m_fields->playbackModeRet = this->m_editorLayer->m_playbackMode;
                    this->m_editorLayer->m_playbackMode = PlaybackMode::Playing;
                }
            }, Priority::Normal - 1);
            this->addEventListener(ScrollWheelEvent(), [this] (double, double) {
                if (shouldBlockScrolling()) {
                    this->m_editorLayer->m_playbackMode = m_fields->playbackModeRet;
                }
            }, Priority::Normal + 1);

            return true;
        }

        bool onCreate() {
            // to not break custom objects
            shouldApplyCustomPlacedObjectOptions() = m_selectedObjectIndex >= 1;

            auto ret = EditorUI::onCreate();

            shouldApplyCustomPlacedObjectOptions() = false;

            return ret;
        }

        void scrollWheel(float y, float x) {
            if (shouldBlockScrolling()) {
                return;
            }

            EditorUI::scrollWheel(y, x);
        }
    };

    class $modify(SharedLevelEditorLayer, LevelEditorLayer) {
        struct Fields {
            std::unordered_map<DrawNode, SillyDrawNode*> gridDraw;
            CCLayer* gridLayer = nullptr;
            std::unordered_map<DrawNode, SillyDrawNode*> overlayDraw;
            CCLayer* overlayLayer = nullptr;

            CCLayer* hiddenLayer = nullptr;
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

            m_fields->gridLayer = this->createLayer("grid-layer"_spr, m_drawGridLayer->getZOrder() + 1);
            m_fields->overlayLayer = this->createLayer("overlay-layer"_spr, m_editorUI->m_scaleControl->getZOrder() - 1);

            (m_fields->hiddenLayer = this->createLayer("hidden-layer"_spr, 0))->setVisible(false);

            m_fields->gridDraw[DrawNode::Default] = this->createDrawNode("grid-draw"_spr, m_fields->gridLayer);
            m_fields->overlayDraw[DrawNode::Default] = this->createDrawNode("overlay-draw"_spr, m_fields->overlayLayer);
        
            return true;
        }

        void updateEditor(float dt) {
            auto fields = m_fields.self();

            if (fields->gridLayer && fields->overlayLayer) {
                for (auto [_, draw] : fields->gridDraw) {
                    draw->clear();
                }
                for (auto [_, draw] : fields->overlayDraw) {
                    draw->clear();
                }
            }

            shouldBlockScrolling() = false;

            LevelEditorLayer::updateEditor(dt);
        }
    };

    SillyDrawNode* getGridDraw(DrawNode pNode) {
        if (auto layer = editor::layer<SharedLevelEditorLayer*>()) {
            return layer->m_fields->gridDraw[pNode];
        }

        return nullptr;
    }
    CCLayer* getGridLayer() {
        if (auto layer = editor::layer<SharedLevelEditorLayer*>()) {
            return layer->m_fields->gridLayer;
        }
        
        return nullptr;
    }
    SillyDrawNode* getOverlayDraw(DrawNode pNode) {
        if (auto layer = editor::layer<SharedLevelEditorLayer*>()) {
            return layer->m_fields->overlayDraw[pNode];
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
}