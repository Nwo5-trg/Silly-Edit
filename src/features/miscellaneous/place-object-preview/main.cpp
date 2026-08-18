// my justifications for making the obj preview an actual object instead of just creating a gameobject and managing it myself
// is so it properly previews all the effects (i.e. alpha and shaders) and thats js the easiest way to do it

#include <utils/include.hpp>
#include "shared.hpp"

using namespace geode::prelude;

namespace PlaceObjectPreview {
    void EditorUI::updatePreviewObject() {
        auto fields = m_fields.self();

        if (!PlaceObjectPreview::enabled()) {
            return Shared::removePreviewObject();
        }
        if (Shared::shouldHidePreviewObject()) {
            return Shared::removePreviewObject();
        }
        if (m_isPaused || m_editorLayer->m_playbackMode != PlaybackMode::Not) {
            return Shared::removePreviewObject();
        }
        if (!m_selectedObjectIndex || m_selectedMode != 2) {
            return Shared::removePreviewObject();
        }
        if (m_selectedObjectIndex == 1329 && m_editorLayer->m_coinCount.value() >= 3) {
            return Shared::removePreviewObject();
        }
        if (CCDirector::get()->getRunningScene()->getChildByType<FLAlertLayer*>(0)) {
            return Shared::removePreviewObject();
        }

        auto& obj = fields->previewObject;

        if (!obj || obj->m_objectID != m_selectedObjectIndex || (fields->wasSelectedObject && !m_selectedObject)) {
            // cuz of the other check
            Shared::removePreviewObject();

            Shared::shouldApplyCustomPlacedObjectOptions() = true;

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
            }

            Shared::shouldApplyCustomPlacedObjectOptions() = false;
        }

        fields->wasSelectedObject = false;

        editor::object::move(obj, this->getGridSnappedPos(m_editorLayer->m_objectLayer->convertToNodeSpace(cocos::getMousePos())));
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

                orange->setPositionOverride(obj->getRealPosition() + ccp(-10.0f, 100.0f));
            }
        }

        if (editor::trigger::is(obj)) {
            static_cast<EffectGameObject*>(obj)->m_isSpawnTriggered = true;
        }
    }



    // fixes obj count
    void EditorUI::onPause(CCObject* sender) {
        Shared::removePreviewObject();

        GD::EditorUI::onPause(sender);
    }
    // fixes coin limit
    bool EditorUI::onCreate() {
        Shared::removePreviewObject();

        return GD::EditorUI::onCreate();
    }
    // fixes getcycledobject
    bool EditorUI::canSelectObject(GameObject* object) {
        if (PlaceObjectPreview::enabled() && object == m_fields->previewObject) {
            return false;
        }
        return GD::EditorUI::canSelectObject(object);
    }
    // not technically necessary i dont think but just incase
    void EditorUI::selectObject(GameObject* object, bool ignoreFilter) {
        if (!PlaceObjectPreview::enabled() || object != m_fields->previewObject) {
            GD::EditorUI::selectObject(object, ignoreFilter);
        }
    }

    // to make rotation not look jittery
    void EditorUI::transformObjectCall(EditCommand command) {
        GD::EditorUI::transformObjectCall(command);

        this->updatePreviewObject();
    }





    void LevelEditorLayer::onModify(auto& pSelf) {
        (void)pSelf.setHookPriorityAfterPost("LevelEditorLayer::addSpecial", nwo5::utils::TINKER_EDIT_ID);
    }



    // some actual bullshit for tinker
    void LevelEditorLayer::addSpecial(GameObject* object) {
        if (!PlaceObjectPreview::enabled() || object->m_objectID != 31 || !nwo5::utils::isTinkerLoaded() || editor::notLoaded(editor::LoadedType::UI)) {
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

    // startpos stuff
    void LevelEditorLayer::onPlaytest() {
        Shared::removePreviewObject();

        GD::LevelEditorLayer::onPlaytest();
    }

    // *should* fix better edit auto save
    gd::string LevelEditorLayer::getLevelString() {
        Shared::removePreviewObject();
        
        return GD::LevelEditorLayer::getLevelString();
    }

    // lets you actually place an object over the preview
    bool LevelEditorLayer::typeExistsAtPosition(int objectID, CCPoint position, bool flipX, bool flipY, float rotation) {
        if (!PlaceObjectPreview::enabled() || editor::notLoaded(editor::LoadedType::UI)) {
            return GD::LevelEditorLayer::typeExistsAtPosition(objectID, position, flipX, flipY, rotation);
        }

        // Searching For An Object That Doesn't Exist
        if (auto obj = editor::ui<PlaceObjectPreview::EditorUI>()->m_fields->previewObject) {
            this->removeObjectFromSection(obj); // could js remove it but funny

            const auto ret = GD::LevelEditorLayer::typeExistsAtPosition(objectID, position, flipX, flipY, rotation);

            this->addToSection(obj);

            return ret;
        }
        else {
            return GD::LevelEditorLayer::typeExistsAtPosition(objectID, position, flipX, flipY, rotation);
        }
    }

    // this is better than a setopacity hook oki
    void LevelEditorLayer::updateVisibility(float dt) {
        if (!PlaceObjectPreview::enabled() || editor::notLoaded(editor::LoadedType::UI)) {
            return GD::LevelEditorLayer::updateVisibility(dt);
        }

        GD::LevelEditorLayer::updateVisibility(dt);

        if (auto obj = editor::ui<PlaceObjectPreview::EditorUI>()->m_fields->previewObject) {
            obj->setOpacity(PlaceObjectPreview::opacity * (obj->getOpacity() / 255.0f));

            if (obj->m_objectID == 747) {
                if (auto orange = static_cast<TeleportPortalObject*>(obj)->m_orangePortal) {
                    orange->setOpacity(PlaceObjectPreview::opacity * (orange->getOpacity() / 255.0f));
                }
            }
        }
    }





    void Feature::onUpdate() {
        if (auto self = editor::ui<PlaceObjectPreview::EditorUI>()) {
            self->updatePreviewObject();
        }
    }
}