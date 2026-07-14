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

    CCPoint getSnappedPos(GameObject* pObj) {
        const auto id = editor::object::id(pObj);
        const auto offset = offsetForKey(id);
        const auto pos = pObj->getRealPosition() - offset;
        const auto gridSize = Settings::FreeSnap::gridSize.get() * (editor::object::size(pObj) / 30.0f);

        return CCPoint{
            std::round(pos.x / gridSize) * gridSize,
            std::round(pos.y / gridSize) * gridSize
        } + offset;
    }

    void snapSelection(GameObject* pSnapObj) {
        editor::object::move(
            editor::selection::get(), 
            getSnappedPos(pSnapObj), 
            false, pSnapObj->getRealPosition()
        );
        
        editor::update();
    }

    bool ccTouchBegan(CCTouch* touch, CCEvent* event) {
        if (!EditorUI::ccTouchBegan(touch, event)) {
            return false;
        }

        // disable snap and do our own thing :3 (idk if snap obj would b valid rn and i dont feel like testing so i wont check for it)
        if (Settings::FreeSnap::enabled.get() && GameManager::sharedState()->getGameVariable(GameVar::EnableSnap) && !editor::selection::empty()) {
            GameManager::sharedState()->setGameVariable(GameVar::EnableSnap, false);
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

        GameManager::sharedState()->setGameVariable(GameVar::EnableSnap, true);

        if (continueSwipe && obj) {
            snapSelection(obj);
        }

        m_fields->prollySnapping = false;
    }

    void updateSelect() {    
        if (editor::selection::empty() || !Settings::FreeSnap::enabled.get()) {
            return;
        }

        const bool shouldntColorObjects = m_colorOverlay || m_hsvOverlay;

        const auto selectionCol = Settings::FreeSnap::chroma.get() 
            ? nwo5::utils::getChroma<ccColor3B>(Shared::ChromaNode::FreeSnap) 
            : Settings::FreeSnap::selectedObjectColor.get();
        const auto snapCol = Settings::FreeSnap::chroma.get() 
            ? nwo5::utils::getChroma<ccColor3B>(Shared::ChromaNode::FreeSnapInvert) 
            : Settings::FreeSnap::snapObjectColor.get();

        // touches happen before schedulers so this works :3c
        for (auto obj : editor::selection::getExt()) {
            if (!shouldntColorObjects) {
                obj->selectObject(selectionCol);
            }
        }

        if (const auto obj = getSnapObject(); obj && m_continueSwipe && m_fields->prollySnapping) {
            if (!shouldntColorObjects) {
                obj->selectObject(selectionCol);
            }

            if (!Settings::FreeSnap::snapIndicator.get()) {
                return;
            }

            const auto pos = getSnappedPos(obj);
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

        nwo5::utils::setupKeybind(this, "free-snap-snap-selection", [this] (const Keybind&, bool pDown, bool pRepeat, double) {
            if (Settings::FreeSnap::enabled.get() && pDown && !pRepeat && !editor::selection::empty()) {
                snapSelection(editor::selection::getFirst());
            }
        });

        return true;
    }
};