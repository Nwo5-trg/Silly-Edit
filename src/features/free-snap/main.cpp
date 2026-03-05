#include <Geode/modify/EditorUI.hpp>
#include <internal/utils/utils.hpp>
#include <features/shared.hpp>
#include "settings.hpp"

using namespace geode::prelude;

class $modify(EditorUI) {
    struct Fields {
        bool prollySnapping = false;
        CCPoint correctLastTouchPos;
    };

    GameObject* getSnapObject() {
        return m_snapObjectExists ? m_snapObject : nullptr;
    }

    void snapSelection(GameObject* pSnapObj) {
        Editor::Object::move(
            Editor::Selection::get(), 
            Editor::Object::snappedPos(pSnapObj, Settings::FreeSnap::gridSize.get()), 
            false, pSnapObj->getRealPosition()
        );
        
        Editor::update();
    }

    bool ccTouchBegan(CCTouch* touch, CCEvent* event) {
        if (!EditorUI::ccTouchBegan(touch, event)) {
            return false;
        }

        // disable snap and do our own thing :3 (idk if snap obj would b valid rn and i dont feel like testing so i wont check for it)
        if (Settings::FreeSnap::enabled.get() && GameManager::sharedState()->getGameVariable("0008") && !Editor::Selection::empty()) {
            GameManager::sharedState()->setGameVariable("0008", false);
            m_fields->prollySnapping = true;

            // i dont exactly remember y i need this and cant just use a member variable but im trusting
            // past me for all the logic - current me doesnt wanna figure that stuff out
            m_fields->correctLastTouchPos = m_editorLayer->m_objectLayer->convertTouchToNodeSpace(touch);
        }
        
        return true;
    }

    void ccTouchMoved(CCTouch* touch, CCEvent* event) {
        EditorUI::ccTouchMoved(touch, event);

        m_fields->correctLastTouchPos = m_editorLayer->m_objectLayer->convertTouchToNodeSpace(touch);
    }
    void ccTouchCancelled(CCTouch* touch, CCEvent* event) {
        EditorUI::ccTouchCancelled(touch, event);
        
        m_fields->prollySnapping = false;
    }

    void ccTouchEnded(CCTouch* touch, CCEvent* event) {
        if (!m_fields->prollySnapping) {
            EditorUI::ccTouchEnded(touch, event);
            return;
        }

        // i have no earthly idea how i figured this out or what this does but im not gonna question it
        const bool continueSwipe = m_continueSwipe;
        auto obj = getSnapObject();

        EditorUI::ccTouchEnded(touch, event);

        GameManager::sharedState()->setGameVariable("0008", true);

        if (continueSwipe && obj) {
            snapSelection(obj);
        }

        m_fields->prollySnapping = false;
    }

    void updateSelect() {    
        if (Editor::Selection::empty()) {
            return;
        }

        const bool shouldntColorObjects = m_colorOverlay || m_hsvOverlay;

        const auto selectionCol = Settings::FreeSnap::chroma.get() 
            ? Shared::getChroma<ccColor3B>(Shared::ChromaNode::FreeSnap) 
            : Settings::FreeSnap::selectedObjectColor.get();
        const auto snapCol = Settings::FreeSnap::chroma.get() 
            ? Shared::getChroma<ccColor3B>(Shared::ChromaNode::FreeSnapInvert) 
            : Settings::FreeSnap::snapObjectColor.get();

        // touches happen before schedulers so this works :3c
        for (auto obj : Editor::Selection::getExt()) {
            if (shouldntColorObjects) {
                obj->updateObjectEditorColor();
            }
            else {
                obj->selectObject(selectionCol);
            }
        }

        if (const auto obj = getSnapObject(); obj && m_continueSwipe && m_fields->prollySnapping) {
            if (shouldntColorObjects) {
                obj->updateObjectEditorColor();
            }
            else {
                obj->selectObject(snapCol);
            }

            if (!Settings::FreeSnap::snapIndicator.get()) {
                return;
            }

            const auto pos = Editor::Object::snappedPos(obj, Settings::FreeSnap::gridSize.get());
            const auto scale = obj->getScaledContentSize() / 2.0f;
            const auto theta = kmDegreesToRadians(-obj->getRotation());

            CCPoint v[] = {
                CCPoint{-scale.width, -scale.height}.rotateByAngle(CCPointZero, theta) + pos,
                CCPoint{-scale.width, scale.height}.rotateByAngle(CCPointZero, theta) + pos,
                CCPoint{scale.width, scale.height}.rotateByAngle(CCPointZero, theta) + pos,
                CCPoint{scale.width, -scale.height}.rotateByAngle(CCPointZero, theta) + pos
            };

            const auto col = ccc4FFromccc3B(snapCol);

            Shared::getGridDraw()->drawPolygon(
                v, 4, {col.r, col.g, col.b, Settings::FreeSnap::snapIndicatorFill.get()},
                Settings::FreeSnap::snapIndicatorThickness.get(), col
            );
        }
    }

    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) {
            return false;
        }

        Shared::addUpdateFunc(SE_UPDATE_FUNC(updateSelect));

        Utils::setupKeybind(this, "free-snap-snap-selection", [this] (const Keybind&, bool pDown, bool pRepeat, double) {
            if (Settings::FreeSnap::enabled.get() && pDown && !pRepeat && !Editor::Selection::empty()) {
                snapSelection(Editor::Selection::getFirst());
            }
        });

        return true;
    }
};