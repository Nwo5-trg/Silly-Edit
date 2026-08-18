#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;

namespace SelectionUtils {
    GameObject* EditorUI::getSnapObject() {
        return m_snapObjectExists ? m_snapObject : nullptr;
    }

    CCPoint EditorUI::getSnappedPos(GameObject* pObj) {
        const auto id = editor::object::id(pObj);
        const auto offset = this->offsetForKey(id);
        const auto pos = pObj->getRealPosition() - offset;
        const auto gridSize = SelectionUtils::gridSize * (editor::object::size(pObj) / 30.0f);

        return CCPoint{
            std::round(pos.x / gridSize) * gridSize,
            std::round(pos.y / gridSize) * gridSize
        } + offset;
    }

    void EditorUI::snapSelection(GameObject* pSnapObj) {
        if (!pSnapObj) {
            return;
        }

        editor::object::move(
            editor::selection::get(), 
            this->getSnappedPos(pSnapObj), 
            false, pSnapObj->getRealPosition()
        );
        
        editor::update();
    }



    bool EditorUI::ccTouchBegan(CCTouch* touch, CCEvent* event) {
        if (!GD::EditorUI::ccTouchBegan(touch, event)) {
            return false;
        }

        // disable snap and do our own thing :3 (idk if snap obj would b valid rn and i dont feel like testing so i wont check for it)
        if (SelectionUtils::enabled() && GameManager::sharedState()->getGameVariable(GameVar::EnableSnap) && !editor::selection::empty()) {
            GameManager::sharedState()->setGameVariable(GameVar::EnableSnap, false);
            m_fields->prollySnapping = true;

            // i dont exactly remember y i need this and cant just use a member variable but im trusting
            // past me for all the logic - current me doesnt wanna figure that stuff out
            m_fields->correctLastTouchPos = m_editorLayer->m_objectLayer->convertTouchToNodeSpace(touch);
        }
        
        return true;
    }

    void EditorUI::ccTouchMoved(CCTouch* touch, CCEvent* event) {
        GD::EditorUI::ccTouchMoved(touch, event);

        m_fields->correctLastTouchPos = m_editorLayer->m_objectLayer->convertTouchToNodeSpace(touch);
    }

    void EditorUI::ccTouchEnded(CCTouch* touch, CCEvent* event) {
        auto fields = m_fields.self();

        const auto draggingCamera  = m_isDraggingCamera;
        const auto swipeActive = m_swipeActive;
        const auto swipeSelected = m_swipeSelected;

        const auto continueSwipe = m_continueSwipe;
        auto obj = this->getSnapObject();

        GD::EditorUI::ccTouchEnded(touch, event);

        // very hacky solution to avoid having to reimpl the entire function (holy fuck robtop ur logic is so fucking bad)
        if (!m_snapObjectExists || !m_continuousSnap || !m_snapObject) {
            if (m_selectedMode == 3 && m_touchID == -1 && SelectionUtils::alwaysSingleSelect && editor::selection::count() > 1) {
                const auto world = getTouchPoint(touch, event);

                if (!swipeActive && !draggingCamera && (!swipeSelected || m_swipeStart.getDistance(world) < 20.0f)) {
                    auto objs = m_editorLayer->objectsAtPosition(m_editorLayer->m_objectLayer->convertToNodeSpace(world));

                    if (objs->count()) {
                        editor::selection::set(static_cast<GameObject*>(objs->firstObject()), true, true, true, true);
                        
                        editor::update();
                    }
                }
            }
        }

        if (fields->prollySnapping) {
            GameManager::sharedState()->setGameVariable(GameVar::EnableSnap, true);

            if (continueSwipe && obj) {
                this->snapSelection(obj);
            }

            fields->prollySnapping = false;
        }
    }





    void Feature::onEditor() {
        auto self = editor::ui<SelectionUtils::EditorUI>();

        nwo5::utils::setupKeybind(self, "free-snap-snap-selection", [self] (const Keybind&, bool pDown, bool pRepeat, double) {
            if (SelectionUtils::enabled() && pDown && !pRepeat && !editor::selection::empty()) {
                self->snapSelection(editor::selection::getFirst());
            }
        });
    }

    void Feature::onUpdate() {
        auto self = editor::ui<SelectionUtils::EditorUI>();

        if (editor::selection::empty() || !SelectionUtils::enabled()) {
            return;
        }

        const bool shouldntColorObjects = self->m_colorOverlay || self->m_hsvOverlay;

        const auto selectionCol = SelectionUtils::chroma 
            ? nwo5::utils::getChroma<ccColor3B>(Shared::ChromaNode::Default) 
            : SelectionUtils::selectedObjectColor;
        const auto snapCol = SelectionUtils::chroma 
            ? nwo5::utils::getChroma<ccColor3B>(Shared::ChromaNode::SelectionUtilsInvert) 
            : SelectionUtils::snapObjectColor;

        auto objs = editor::selection::getExt();

        // touches happen before schedulers so this works :3c
        if (!shouldntColorObjects) {
            for (auto obj : objs) {
                obj->selectObject(selectionCol);
            }
        }

        if (const auto obj = self->getSnapObject(); obj && self->m_continueSwipe && self->m_fields->prollySnapping) {
            obj->selectObject(selectionCol);

            if (!SelectionUtils::snapIndicator) {
                return;
            }

            const auto pos = self->getSnappedPos(obj);
            const auto scale = obj->getScaledContentSize() / 2.0f;
            const auto theta = kmDegreesToRadians(-obj->getRotation());

            const CCPoint v[] = {
                CCPoint{-scale.width, -scale.height}.rotateByAngle(CCPointZero, theta) + pos,
                CCPoint{-scale.width, scale.height}.rotateByAngle(CCPointZero, theta) + pos,
                CCPoint{scale.width, scale.height}.rotateByAngle(CCPointZero, theta) + pos,
                CCPoint{scale.width, -scale.height}.rotateByAngle(CCPointZero, theta) + pos
            };

            Shared::getGridDraw()->drawPolygon(
                v, 4, nwo5::utils::setOpacity(color_cast<ccColor4F>(snapCol), SelectionUtils::snapIndicatorFill.get()),
                SelectionUtils::snapIndicatorThickness, color_cast<ccColor4F>(snapCol)
            );
        }
    }
}