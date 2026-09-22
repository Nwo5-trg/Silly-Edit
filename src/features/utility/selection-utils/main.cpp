#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;

namespace SelectionUtils {
    GameObject* EditorUI::getSnapObject() {
        return m_snapObjectExists ? m_snapObject : nullptr;
    }

    CCPoint EditorUI::getSnappedPos(GameObject* pObj) {
        const auto id = object::id(pObj);
        const auto offset = this->offsetForKey(id);
        const auto pos = pObj->getRealPosition() - offset;
        const auto gridSize = SelectionUtils::gridSize * (object::size(pObj) / 30.0f);

        return CCPoint{
            std::round(pos.x / gridSize) * gridSize,
            std::round(pos.y / gridSize) * gridSize
        } + offset;
    }

    void EditorUI::snapSelection(GameObject* pSnapObj) {
        if (!pSnapObj) {
            return;
        }
        
        object::move(
            selection::get(), this->getSnappedPos(pSnapObj), false, pSnapObj->getRealPosition()
        );
        
        editor::update();
    }



    void EditorUI::draw() {
        if (!SelectionUtils::enabled()) {
            return GD::EditorUI::draw();
        }

        if (m_swipeActive) {
            const auto col = color_cast<ccColor4B>(
                SelectionUtils::chroma 
                    ? sillyedit::utils::getChroma<ccColor3B>() 
                    : SelectionUtils::selectedObjectColor
            );

            if (SelectionUtils::snapIndicatorFill) {
                ccDrawSolidRect(
                    m_swipeStart, m_swipeEnd, 
                    color_cast<ccColor4F>(misc::setOpacity(col, SelectionUtils::selectionRectFill.get()))
                );
            }
            
            if (SelectionUtils::selectionRectThickness.get() == 1.0f) {
                ccDrawColor4B(col);
                ccDrawRect(m_swipeStart, m_swipeEnd);
            }
            else {
                glLineWidth(SelectionUtils::selectionRectThickness);

                ccDrawColor4B(col);
                ccDrawRect(m_swipeStart, m_swipeEnd);

                glLineWidth(1.0f);
            }
        }

        const bool ret = m_swipeActive;
        m_swipeActive = false;
        GD::EditorUI::draw();
        m_swipeActive = ret;
    }

    bool EditorUI::ccTouchBegan(CCTouch* touch, CCEvent* event) {
        if (!GD::EditorUI::ccTouchBegan(touch, event)) {
            return false;
        }

        auto fields = m_fields.self();

        // disable snap and do our own thing :3 (idk if snap obj would b valid rn and i dont feel like testing so i wont check for it)
        if (SelectionUtils::enabled() && GameManager::sharedState()->getGameVariable(GameVar::EnableSnap) && !selection::empty()) {
            GameManager::sharedState()->setGameVariable(GameVar::EnableSnap, false);
            fields->prollySnapping = true;

            // i dont exactly remember y i need this and cant just use a member variable but im trusting
            // past me for all the logic - current me doesnt wanna figure that stuff out
            fields->correctLastTouchPos = m_editorLayer->m_objectLayer->convertTouchToNodeSpace(touch);
        }
        else {
            fields->prollySnapping = false;
        }
        
        return true;
    }

    void EditorUI::ccTouchMoved(CCTouch* touch, CCEvent* event) {
        GD::EditorUI::ccTouchMoved(touch, event);

        m_fields->correctLastTouchPos = m_editorLayer->m_objectLayer->convertTouchToNodeSpace(touch);
    }

    void EditorUI::ccTouchEnded(CCTouch* touch, CCEvent* event) {
        auto fields = m_fields.self();

        const auto draggingCamera = m_isDraggingCamera;
        const auto swipeActive = m_swipeActive;
        const auto swipeSelected = m_swipeSelected;
        const auto swipeStart = m_swipeStart;
        const auto continueSwipe = m_continueSwipe;
        const auto rotationtouchID = m_rotationTouchID;
        const auto scaleTouchID = m_scaleTouchID;
        const auto transformTouchID = m_transformTouchID;

        auto obj = this->getSnapObject();

        GD::EditorUI::ccTouchEnded(touch, event);

        // very hacky solution to avoid having to reimpl the entire function (holy fuck robtop ur logic is so fucking bad)
        if (fields->prollySnapping) {
            GameManager::sharedState()->setGameVariable(GameVar::EnableSnap, true);

            if (continueSwipe && obj) {
                this->snapSelection(obj);

                return;
            }

            fields->prollySnapping = false;
        }
        
        if (!m_snapObjectExists || !m_continuousSnap || !m_snapObject) {
            const auto trySingleSelect = m_selectedMode == 3 
                && m_touchID == -1 
                && rotationtouchID == -1 && scaleTouchID == -1 && transformTouchID == -1
                && SelectionUtils::alwaysSingleSelect && selection::count() > 1;
            const auto tryEmptyDeselect = m_selectedMode == 3 
                && rotationtouchID == -1 && scaleTouchID == -1 && transformTouchID == -1
                && SelectionUtils::clickEmptyToDeselect && !selection::empty();

            if ((trySingleSelect || tryEmptyDeselect) && !swipeActive && !draggingCamera) {
                const auto world = getTouchPoint(touch, event);

                if ((!swipeSelected || (m_swipeStart.getDistance(world) < 20.0f)) && !sillyedit::utils::findUITextInputs(world)) {
                    auto objs = m_editorLayer->objectsAtPosition(m_editorLayer->m_objectLayer->convertToNodeSpace(world));

                    if (!objs->count() && tryEmptyDeselect) {
                        selection::clear(true);

                        editor::update(false, true);
                    }
                    else if (objs->count() && trySingleSelect) {
                        selection::set(static_cast<GameObject*>(objs->firstObject()), true, true, true, true);
                                
                        editor::update();
                    }
                }
            }
        }
    }





    void Feature::onEditor() {
        auto self = editor::ui<SelectionUtils::EditorUI>();

        feature.registerKeybind<"snap-selection">([self] (bool pDown, bool pRepeat) {
            if (pDown && !pRepeat && !selection::empty()) {
                self->snapSelection(selection::getFirst());
            }
        });
    }

    void Feature::onUpdate() {
        auto self = editor::ui<SelectionUtils::EditorUI>();

        if (selection::empty() || !SelectionUtils::enabled()) {
            return;
        }

        const bool shouldntColorObjects = self->m_colorOverlay || self->m_hsvOverlay;

        const auto selectionCol = SelectionUtils::chroma 
            ? sillyedit::utils::getChroma<ccColor3B>(sillyedit::utils::ChromaNode::SelectionUtilsDefault) 
            : SelectionUtils::selectedObjectColor;
        const auto snapCol = SelectionUtils::chroma 
            ? sillyedit::utils::getChroma<ccColor3B>(sillyedit::utils::ChromaNode::SelectionUtilsInvert) 
            : SelectionUtils::snapObjectColor;

        // touches happen before schedulers so this works :3c
        if (!shouldntColorObjects) {
            for (auto obj : selection::getExt()) {
                obj->selectObject(selectionCol);
            }
        }

        if (const auto obj = self->getSnapObject(); obj && self->m_continueSwipe && self->m_fields->prollySnapping) {
            obj->selectObject(snapCol);

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

            sillyedit::utils::getGridDraw()->drawPolygon(
                v, 4, color_cast<ccColor4F>(misc::setOpacity(color_cast<ccColor4B>(snapCol), SelectionUtils::snapIndicatorFill.get())),
                SelectionUtils::snapIndicatorThickness, color_cast<ccColor4F>(snapCol)
            );
        }
    }
}