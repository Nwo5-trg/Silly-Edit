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
        const auto offset = this->offsetForKey(id);
        const auto pos = pObj->getRealPosition() - offset;
        const auto gridSize = Settings::SelectionUtils::gridSize.get() * (editor::object::size(pObj) / 30.0f);

        return CCPoint{
            std::round(pos.x / gridSize) * gridSize,
            std::round(pos.y / gridSize) * gridSize
        } + offset;
    }

    void snapSelection(GameObject* pSnapObj) {
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

    static CCRect fuckWhyIsTheLinkerBeingStupidIGenuinelyHaveNoIdeaButICantUseItFromSillyAPISoFuckYou(const CCRect& pA, const CCRect& pB) {
        const CCPoint a{std::max(pA.getMinX(), pB.getMinX()), std::max(pA.getMinY(), pB.getMinY())};
        const CCPoint b{std::min(pA.getMaxX(), pB.getMaxX()), std::min(pA.getMaxY(), pB.getMaxY())};

        if (b.x > a.x && b.y > a.y) {
            return CCRect{a, b - a};
        }

        return CCRectZero;
    }

    bool ccTouchBegan(CCTouch* touch, CCEvent* event) {
        if (!EditorUI::ccTouchBegan(touch, event)) {
            return false;
        }

        // disable snap and do our own thing :3 (idk if snap obj would b valid rn and i dont feel like testing so i wont check for it)
        if (Settings::SelectionUtils::enabled.get() && GameManager::sharedState()->getGameVariable(GameVar::EnableSnap) && !editor::selection::empty()) {
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

    void ccTouchEnded(CCTouch* touch, CCEvent* event) {
        auto fields = m_fields.self();

        const auto draggingCamera  = m_isDraggingCamera;
        const auto swipeActive = m_swipeActive;
        const auto swipeSelected = m_swipeSelected;

        const auto continueSwipe = m_continueSwipe;
        auto obj = this->getSnapObject();

        EditorUI::ccTouchEnded(touch, event);

        // very hacky solution to avoid having to reimpl the entire function (holy fuck robtop ur logic is so fucking bad)
        if (!m_snapObjectExists || !m_continuousSnap || !m_snapObject) {
            if (m_selectedMode == 3 && m_touchID == -1 && Settings::SelectionUtils::alwaysSingleSelect.get() && editor::selection::count() > 1) {
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

    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) {
            return false;
        }

        Shared::addUpdateFunc([this] {
            if (editor::selection::empty() || !Settings::SelectionUtils::enabled.get()) {
                return;
            }

            const bool shouldntColorObjects = m_colorOverlay || m_hsvOverlay;

            const auto selectionCol = Settings::SelectionUtils::chroma.get() 
                ? nwo5::utils::getChroma<ccColor3B>(Shared::ChromaNode::Default) 
                : Settings::SelectionUtils::selectedObjectColor.get();
            const auto snapCol = Settings::SelectionUtils::chroma.get() 
                ? nwo5::utils::getChroma<ccColor3B>(Shared::ChromaNode::SelectionUtilsInvert) 
                : Settings::SelectionUtils::snapObjectColor.get();

            auto objs = editor::selection::getExt();

            // touches happen before schedulers so this works :3c
            if (!shouldntColorObjects) {
                for (auto obj : objs) {
                    obj->selectObject(selectionCol);
                }
            }

            if (const auto obj = getSnapObject(); obj && m_continueSwipe && m_fields->prollySnapping) {
                obj->selectObject(selectionCol);

                if (!Settings::SelectionUtils::snapIndicator.get()) {
                    return;
                }

                const auto pos = this->getSnappedPos(obj);
                const auto scale = obj->getScaledContentSize() / 2.0f;
                const auto theta = kmDegreesToRadians(-obj->getRotation());

                const CCPoint v[] = {
                    CCPoint{-scale.width, -scale.height}.rotateByAngle(CCPointZero, theta) + pos,
                    CCPoint{-scale.width, scale.height}.rotateByAngle(CCPointZero, theta) + pos,
                    CCPoint{scale.width, scale.height}.rotateByAngle(CCPointZero, theta) + pos,
                    CCPoint{scale.width, -scale.height}.rotateByAngle(CCPointZero, theta) + pos
                };

                Shared::getGridDraw()->drawPolygon(
                    v, 4, nwo5::utils::setOpacity(color_cast<ccColor4F>(snapCol), Settings::SelectionUtils::snapIndicatorFill.get()),
                    Settings::SelectionUtils::snapIndicatorThickness.get(), color_cast<ccColor4F>(snapCol)
                );
            }
        });

        nwo5::utils::setupKeybind(this, "free-snap-snap-selection", [this] (const Keybind&, bool pDown, bool pRepeat, double) {
            if (Settings::SelectionUtils::enabled.get() && pDown && !pRepeat && !editor::selection::empty()) {
                this->snapSelection(editor::selection::getFirst());
            }
        });

        return true;
    }
};