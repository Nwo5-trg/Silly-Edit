// my justifications for making the obj preview an actual object instead of just creating a gameobject and managing it myself
// is so it properly previews all the effects (i.e. alpha and shaders) and thats js the easiest way to do it

#include "shared.hpp"

using namespace geode::prelude;

static constexpr int PREVIEW_OBJECT_TAG = 8373767689;

namespace PlaceObjectPreview {
    void EditorUI::updatePreviewObject() {
        auto fields = m_fields.self();

        if (!PlaceObjectPreview::enabled()) {
            return sillyedit::shared::removePreviewObject();
        }
        if (sillyedit::shared::shouldHidePreviewObject()) {
            return sillyedit::shared::removePreviewObject();
        }
        if (m_isPaused || m_editorLayer->m_playbackMode != PlaybackMode::Not) {
            return sillyedit::shared::removePreviewObject();
        }
        if (!m_selectedObjectIndex || m_selectedMode != 2) {
            return sillyedit::shared::removePreviewObject();
        }
        if (m_selectedObjectIndex == 1329 && m_editorLayer->m_coinCount.value() >= 3) {
            return sillyedit::shared::removePreviewObject();
        }
        if (CCDirector::get()->getRunningScene()->getChildByType<FLAlertLayer*>(0)) {
            return sillyedit::shared::removePreviewObject();
        }

        auto& obj = fields->previewObject;

        if (!obj || obj->m_objectID != m_selectedObjectIndex || (fields->wasSelectedObject && !m_selectedObject)) {
            // cuz of the other check
            sillyedit::shared::removePreviewObject();

            sillyedit::utils::shouldApplyCustomPlacedObjectOptions() = true;

            // let me tell you about the story of the girl who wasted
            // 7 and a half FUCKING HOURS OF HER LIFE debugging this one STUPID FUCKING FUNCTION
            // so origionally this was the editorui createobject function
            // which i have no idea why i choose, ig cuz of the below pixelscale bs stuff from the decomp
            // but then in floodfill, shit just crashed sometimes when undoing, and i had NO FUCKING IDEA WHY
            // i tried everything in the book 5 times over and i literally couldnt solve it
            // then i found out it fixed itself if place object preview was disabled
            // so then i tried everything in the book again 15 times over and still fucking nothing
            // until my dumbass finally had a good idea of checking what fucking undo command was crashing
            // and apparently it *wasnt* UndoCommand::Paste, which is what i thought was causing the issue,
            // instead it was UndoCommand 2, so instead i tried for an hour to debug if an extra remove
            // undo call was happening, until i had the bright fucking idea of CHECKING WHAT THE UNDOCOMMAND
            // EVEN FUCKING WAS, IT WASNT FUCKING DELETE OR EVEN DELETEMULTI IT WAS FUCKING NEW
            // MY DUMBASS HEAD WAS STEADFAST ON THE BELIEF THAT IT WAS A DELETE UNDO COMMAND FOR NO FUCKING REASON
            // from there the fix was easy since theres only 1 place in that that undo command can even happen
            // fml
            obj = m_editorLayer->createObject(m_selectedObjectIndex, CCPointZero, true);

            if (obj) { // ty alpha for the decomp
                if (obj->m_pixelScaleX != 1.0 || obj->m_pixelScaleY != 1.0) {
                    obj->updateCustomScaleX(obj->m_pixelScaleX);
                    obj->updateCustomScaleY(obj->m_pixelScaleY);
                }

                obj->setTag(PREVIEW_OBJECT_TAG);

                if (trigger::is(obj)) {
                    static_cast<EffectGameObject*>(obj)->m_isSpawnTriggered = true;
                    // no clue what this does but it lets me fix trigger type boxes easily so
                    obj->m_greenDebugDraw = true;
                }
            }

            sillyedit::utils::shouldApplyCustomPlacedObjectOptions() = false;
        }

        fields->wasSelectedObject = false;

        object::move(obj, this->getGridSnappedPos(m_editorLayer->m_objectLayer->convertToNodeSpace(cocos::getMousePos())));
        this->applyOffset(obj);

        // rly not a big deal to do every update since duplicatevalues doesnt do anything heavy
        if (m_selectedObject && m_selectedObject->m_objectID == m_selectedObjectIndex) {
            obj->duplicateValues(m_selectedObject);

            fields->wasSelectedObject = true;
        }

        obj->m_editorLayer = editor::currentLayer();

        // i hate teleportals i hate teleportals i hate teleportals i hate teleportals i hate teleportals i hate teleportals i hate teleportals
        if (obj->m_objectID == 747) {
            if (auto orange = static_cast<TeleportPortalObject*>(obj)->m_orangePortal) {
                orange->m_editorLayer = editor::currentLayer();

                orange->setPositionOverride(obj->getRealPosition() + CCPoint{-10.0f, 100.0f});
            }
        }
    }



    // fixes obj count
    void EditorUI::onPause(CCObject* sender) {
        sillyedit::shared::removePreviewObject();

        GD::EditorUI::onPause(sender);
    }
    // fixes coin limit
    bool EditorUI::onCreate() {
        sillyedit::shared::removePreviewObject();

        return GD::EditorUI::onCreate();
    }
    // fixes getcycledobject
    bool EditorUI::canSelectObject(GameObject* object) {
        if (PlaceObjectPreview::enabled() && object->getTag() == PREVIEW_OBJECT_TAG && object == m_fields->previewObject) {
            return false;
        }
        return GD::EditorUI::canSelectObject(object);
    }
    // not technically necessary i dont think but just incase
    void EditorUI::selectObject(GameObject* object, bool ignoreFilter) {
        if (!PlaceObjectPreview::enabled() || object->getTag() != PREVIEW_OBJECT_TAG || object != m_fields->previewObject) {
            GD::EditorUI::selectObject(object, ignoreFilter);
        }
    }

    // to make rotation not look jittery
    void EditorUI::transformObjectCall(EditCommand command) {
        GD::EditorUI::transformObjectCall(command);

        this->updatePreviewObject();
    }



    // some actual bullshit for tinker
    void LevelEditorLayer::addSpecial(GameObject* object) {
        if (!PlaceObjectPreview::enabled() || object->m_objectID != 31 || !sillyedit::utils::isTinkerLoaded() || editor::notLoaded(editor::LoadedType::UI)) {
            return GD::LevelEditorLayer::addSpecial(object);
        }
        if (auto obj = editor::ui<PlaceObjectPreview::EditorUI>()->m_fields->previewObject; obj && object == obj) {
            obj->m_objectID = 0;
            GD::LevelEditorLayer::addSpecial(object);
            obj->m_objectID = 31;
        }
        else {
            GD::LevelEditorLayer::addSpecial(object);
        }
    }

    // *should* fix better edit auto save
    gd::string LevelEditorLayer::getLevelString() {
        sillyedit::shared::removePreviewObject();
        
        return GD::LevelEditorLayer::getLevelString();
    }

    // lets you actually place an object over the preview
    bool LevelEditorLayer::typeExistsAtPosition(int objectID, CCPoint position, bool flipX, bool flipY, float rotation) {
        if (!PlaceObjectPreview::enabled() || editor::notLoaded(editor::LoadedType::UI)) {
            return GD::LevelEditorLayer::typeExistsAtPosition(objectID, position, flipX, flipY, rotation);
        }

        // Searching For An Object That Doesn't Exist
        if (auto obj = editor::ui<PlaceObjectPreview::EditorUI>()->m_fields->previewObject) {
            const auto id = obj->m_objectID;
            obj->m_objectID = -1;

            const auto ret = GD::LevelEditorLayer::typeExistsAtPosition(objectID, position, flipX, flipY, rotation);

            obj->m_objectID = id;

            return ret;
        }
        else {
            return GD::LevelEditorLayer::typeExistsAtPosition(objectID, position, flipX, flipY, rotation);
        }
    }

    // make the function actually work
    GameObject* LevelEditorLayer::objectAtPosition(CCPoint position) {
        if (!PlaceObjectPreview::enabled() || editor::notLoaded(editor::LoadedType::UI)) {
            return GD::LevelEditorLayer::objectAtPosition(position);
        }

        if (auto obj = editor::ui<PlaceObjectPreview::EditorUI>()->m_fields->previewObject) {
            this->removeObjectFromSection(obj);

            const auto ret = GD::LevelEditorLayer::objectAtPosition(position);

            this->addToSection(obj);

            return ret;
        }
        else {
            return GD::LevelEditorLayer::objectAtPosition(position);
        }
    }

    // make the function actually work
    CCArray* LevelEditorLayer::objectsAtPosition(CCPoint position) {
        if (!PlaceObjectPreview::enabled() || editor::notLoaded(editor::LoadedType::UI)) {
            return GD::LevelEditorLayer::objectsAtPosition(position);
        }

        if (auto obj = editor::ui<PlaceObjectPreview::EditorUI>()->m_fields->previewObject) {
            this->removeObjectFromSection(obj);

            const auto ret = GD::LevelEditorLayer::objectsAtPosition(position);

            this->addToSection(obj);

            return ret;
        }
        else {
            return GD::LevelEditorLayer::objectsAtPosition(position);
        }
    }

    // this is better than a setopacity hook oki
    void LevelEditorLayer::updateVisibility(float dt) {
        if (!PlaceObjectPreview::enabled() || editor::notLoaded(editor::LoadedType::UI)) {
            return GD::LevelEditorLayer::updateVisibility(dt);
        }

        GD::LevelEditorLayer::updateVisibility(dt);

        if (auto obj = editor::ui<PlaceObjectPreview::EditorUI>()->m_fields->previewObject) {
            obj->setOpacity(sillyedit::utils::modifyOpacity(obj->getOpacity(), PlaceObjectPreview::opacity));

            // lol idk y it doesnt work normally honestly, prolly has to do with all the shenanigans i do to make this a fake asl object
            if (trigger::type(obj) != trigger::ObjectType::Normal) {
                updateObjectLabel(obj);
            }

            if (obj->m_objectID == 747) {
                if (auto orange = static_cast<TeleportPortalObject*>(obj)->m_orangePortal) {
                    orange->setOpacity(sillyedit::utils::modifyOpacity(obj->getOpacity(), PlaceObjectPreview::opacity));
                }
            }
        }
    }





    void Feature::onEditor() {
        auto self = editor::ui<PlaceObjectPreview::EditorUI>();

        self->addEventListener(OnPlaytestEvent(true), [] {
            sillyedit::shared::removePreviewObject();
        });
    }

    void Feature::onUpdate() {
        if (auto self = editor::ui<PlaceObjectPreview::EditorUI>()) {
            self->updatePreviewObject();
        }
    }
}