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

    void update(bool pUpdateControls) {
        auto editor = ui();

        editor->updateButtons();
        editor->updateObjectInfoLabel();
        editor->updateGridNodeSize();
        editor->tryUpdateTimeMarkers();
        
        if (pUpdateControls) {
            if (editor->m_rotationControl && editor->m_rotationControl->isVisible()) {
                editor->deactivateRotationControl();
                editor->activateRotationControl(nullptr);
            }
            if (editor->m_scaleControl && editor->m_scaleControl->isVisible()) {
                editor->deactivateScaleControl();
                editor->activateScaleControl(nullptr);
            }
            if (editor->m_transformControl && editor->m_transformControl->isVisible()) {
                editor->m_transformControl->refreshControl();
            }
        }
    }

    float zoom() {
        return ui()->m_editorZoom;
    }
    cocos2d::CCPoint center() {
        return CCPoint{
            (CCDirector::get()->getWinSize() / 2) - CCPoint{
                layer()->m_objectLayer->getPositionX(), 
                layer()->m_objectLayer->getPositionY() - ui()->m_toolbarHeight / 2
            }
         } / zoom();
    }

    namespace Impl {
        void createUndoObject(UndoCommand pCommand, bool pAddToList) {
            ui()->createUndoObject(pCommand, !pAddToList);
        }
    }
}