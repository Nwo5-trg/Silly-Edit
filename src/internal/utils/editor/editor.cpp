#include "editor.hpp"

using namespace geode::prelude;

namespace Editor {
    EditorUI* ui() {
        static const auto manager = GameManager::get();
        return manager->m_levelEditorLayer->m_editorUI;
    };
    LevelEditorLayer* layer() {
        static const auto manager = GameManager::get();
        return manager->m_levelEditorLayer;
    }

    bool loaded() {
        return layer();
    }

    void update(bool pUpdateControls, bool pOtherwiseDeactivateControls) {
        auto editor = ui();

        editor->updateButtons();
        editor->updateObjectInfoLabel();
        editor->updateGridNodeSize();
        editor->tryUpdateTimeMarkers();
        
        if (pUpdateControls) {
            if (auto control = editor->m_rotationControl; control && control->isVisible()) {
                activateRotationControl();
            }
            if (auto control = editor->m_scaleControl; control && control->isVisible()) {
                activateScaleControl(!control->m_scaleLabel->isVisible());
            }
            if (auto control = editor->m_transformControl; control && control->isVisible()) {
                activateTransformControl();
            }
        }
        else if (pOtherwiseDeactivateControls) {
            editor->deactivateRotationControl();
            editor->deactivateScaleControl();
            editor->deactivateTransformControl();
        }
    }

    float zoom() {
        return layer()->m_objectLayer->getScale();
    }
    cocos2d::CCPoint center() {
        return CCPoint{
            (CCDirector::get()->getWinSize() / 2) - CCPoint{
                layer()->m_objectLayer->getPositionX(), 
                layer()->m_objectLayer->getPositionY() - ui()->m_toolbarHeight / 2
            }
         } / zoom();
    }

    void activateRotationControl(bool pRefresh) {
        if (!ui()->m_rotationControl) {
            return;
        }

        if (pRefresh) {
            ui()->deactivateRotationControl();
        }
        else if (ui()->m_rotationControl->isVisible()) {
            return;
        }

        ui()->activateRotationControl(nullptr);
    }
    void activateScaleControl(bool pXY, bool pRefresh) {
        if (!ui()->m_scaleControl) {
            return;
        }

        if (pRefresh) {
            ui()->deactivateScaleControl();
        }
        else if (ui()->m_scaleControl->isVisible()) {
            return;
        }

        auto obj = CCNode::create();
        obj->setTag(pXY ? 30 : 29);

        ui()->activateScaleControl(obj);
    }
    void activateTransformControl(bool pRefresh) {
        if (!ui()->m_transformControl) {
            return;
        }

        if (pRefresh) {
            ui()->deactivateTransformControl();
        }
        else if (ui()->m_transformControl->isVisible()) {
            return;
        }

        ui()->activateTransformControl(nullptr);
    }

    int currentLayer() {
        return layer()->m_currentLayer;
    }
    bool layerSelectable(int pLayer, bool pIgnoreLocked) {
        if (pLayer > MAX_LAYERS || (!pIgnoreLocked && layerLocked(pLayer))) {
            return false;
        }

        const auto layer = currentLayer();

        return layer == -1 || layer == pLayer;
    }
    bool layerLocked(int pLayer) {
        return layer()->isLayerLocked(pLayer);
    }

    namespace Impl {
        void createUndoObject(UndoCommand pCommand) {
            ui()->createUndoObject(pCommand, false);
        }
    }
}