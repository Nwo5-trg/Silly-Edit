#ifdef GEODE_IS_DESKTOP

// my justifications for making the obj preview an actual object instead of just creating a gameobject and managing it myself
// is so it properly previews all the effects (i.e. alpha and shaders) and thats js the easiest way to do it
// rip editor collab compat good luck alk

#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <features/default-object-options/include.hpp>
#include <alphalaneous.editortab_api/include/EditorTabAPI.hpp>
#include <Geode/modify/GameObject.hpp>
#include <internal/utils/utils.hpp>
#include <features/shared.hpp>
#include "settings.hpp"

using namespace geode::prelude;

class $modify(PlaceObjectPreviewEditorUI, EditorUI) {
    struct Fields {
        GameObject* previewObject = nullptr;
        // to reset duplicated values
        bool wasSelectedObject = false;
    };

    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) {
            return false;
        }

        Shared::addUpdateFunc(SE_UPDATE_FUNC(updatePreviewObject));
        
        return true;
    }

    void removePreviewObject() {
        if (auto& obj = m_fields->previewObject) {
            editor::object::remove(obj);

            obj = nullptr;
        }
    }

    void updatePreviewObject() {
        auto fields = m_fields.self();

        auto& obj = fields->previewObject;
        
        bool shouldShow = true;

        if (!Settings::Miscellaneous::placeObjectPreview.get()) {
            shouldShow = false;
        }
        else if (m_isPaused || m_editorLayer->m_playbackMode != PlaybackMode::Not) {
            shouldShow = false;
        }
        else if (!m_selectedObjectIndex || m_selectedMode != 2) {
            shouldShow = false;
        }
        else if (m_selectedObjectIndex == 1329 && m_editorLayer->m_coinCount.value() >= 3) {
            shouldShow = false;
        }
        else if (alpha::editor_tabs::getCurrentTab().unwrapOrDefault() == "custom") {
            shouldShow = false;
        }
        else if (CCDirector::get()->getRunningScene()->getChildByType<FLAlertLayer*>(0)) {
            shouldShow = false;
        }

        if (!shouldShow) {
            return removePreviewObject();
        }

        if (!obj || obj->m_objectID != m_selectedObjectIndex || (fields->wasSelectedObject && !m_selectedObject)) {
            // cuz of the other check
            removePreviewObject();

            DefaultObjectOptions::shouldApplyObjectOptions() = true;

            obj = createObject(m_selectedObjectIndex, CCPointZero);

            DefaultObjectOptions::shouldApplyObjectOptions() = false;
        }

        fields->wasSelectedObject = false;

        // rly not a big deal to do every update since duplicatevalues doesnt do anything heavy
        if (m_selectedObject && m_selectedObject->m_objectID == m_selectedObjectIndex) {
            removeOffset(obj);
            obj->duplicateValues(m_selectedObject);
            applyOffset(obj);

            editor::object::rotate(obj, 0.0f, obj, false, AUTO_CENTER, false);
            editor::object::rotate(obj, m_selectedObject->getObjectRotation(), obj, false, getGridSnappedPos(obj->getRealPosition()), true);

            fields->wasSelectedObject = true;
        }

        const auto basePos = getGridSnappedPos(
            m_editorLayer->m_objectLayer->convertToNodeSpace(cocos::getMousePos())
        );
        const auto pos = basePos + offsetForKey(obj->m_objectID);
        
        editor::object::move(obj, pos);

        obj->m_editorLayer = editor::currentLayer();

        // i hate teleportals i hate teleportals i hate teleportals i hate teleportals i hate teleportals i hate teleportals i hate teleportals
        if (obj->m_objectID == 747) {
            if (auto orange = static_cast<TeleportPortalObject*>(obj)->m_orangePortal) {
                orange->m_editorLayer = editor::currentLayer();

                orange->setPositionOverride(pos + ccp(-10.0f, 100.0f));
            }
        }
    }

    // fixes obj count
    void onPause(CCObject* sender) {
        removePreviewObject();
        EditorUI::onPause(sender);
    }
    // fixes coin limit
    bool onCreate() {
        removePreviewObject();
        return EditorUI::onCreate();
    }
    // fixes getcycledobject
    bool canSelectObject(GameObject* object) {
        if (Settings::Miscellaneous::placeObjectPreview.get() && object == m_fields->previewObject) {
            return false;
        }
        return EditorUI::canSelectObject(object);
    }
    // not technically necessary i dont think but just incase
    void selectObject(GameObject* object, bool ignoreFilter) {
        if (!Settings::Miscellaneous::placeObjectPreview.get() || object != m_fields->previewObject) {
            EditorUI::selectObject(object, ignoreFilter);
        }
    }
};

class $modify(LevelEditorLayer) {
    // *should* fix better edit auto save
    gd::string getLevelString() {
        if (auto ui = editor::ui<PlaceObjectPreviewEditorUI>()) {
            ui->removePreviewObject();
        }
        
        return LevelEditorLayer::getLevelString();
    }
    // lets you actually place an object over the preview
    bool typeExistsAtPosition(int objectID, CCPoint position, bool flipX, bool flipY, float rotation) {
        // Searching For An Object That Doesn't Exist
        if (auto obj = editor::ui<PlaceObjectPreviewEditorUI>()->m_fields->previewObject) {
            removeObjectFromSection(obj); // could js remove it but id like to avoid recreating the obj if possible

            const auto ret = LevelEditorLayer::typeExistsAtPosition(objectID, position, flipX, flipY, rotation);

            addToSection(obj);

            return ret;
        }
        else {
            return LevelEditorLayer::typeExistsAtPosition(objectID, position, flipX, flipY, rotation);
        }
    }

    void updateVisibility(float dt) {
        LevelEditorLayer::updateVisibility(dt);

        if (auto obj = editor::ui<PlaceObjectPreviewEditorUI>()->m_fields->previewObject) {
            obj->setOpacity(Settings::Miscellaneous::placeObjectPreviewOpacity.get() * (obj->getOpacity() / 255.0f));

            if (obj->m_objectID == 747) {
                if (auto orange = static_cast<TeleportPortalObject*>(obj)->m_orangePortal) {
                    orange->setOpacity(Settings::Miscellaneous::placeObjectPreviewOpacity.get() * (orange->getOpacity() / 255.0f));
                }
            }
        }
    }
};

#endif