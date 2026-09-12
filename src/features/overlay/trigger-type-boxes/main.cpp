#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

namespace TriggerTypeBoxes {
    void LevelEditorLayer::drawTriggerTypeBoxes(GameObject* pObj) {
        if (pObj->m_classType != GameObjectClassType::Effect || !pObj->isVisible() || pObj->m_isDisabled || pObj->m_greenDebugDraw) {
            return;
        }

        const auto opacity = pObj->getOpacity();

        if (!opacity) {
            return;
        }

        auto trigger = static_cast<EffectGameObject*>(pObj);

        auto drawnode = TriggerTypeBoxes::drawInfront ? sillyedit::utils::getOverlayDraw() : sillyedit::utils::getGridDraw();

        if (trigger->m_isTouchTriggered && TriggerTypeBoxes::touchTriggerIndicators) {
            const auto pos = ccAdd(
                trigger->getRealPosition(),
                sillyedit::utils::triggerHasBodyOffset(trigger->m_objectID) && TriggerTypeBoxes::offsetTouchTrigger 
                    ? sillyedit::utils::TRIGGER_BODY_OFFSET 
                    : CCPointZero
            );
            const auto col = misc::setOpacity(
                TriggerTypeBoxes::chroma 
                    ? sillyedit::utils::getChroma<ccColor4B>() 
                    : color_cast<ccColor4B>(TriggerTypeBoxes::touchTriggerColor.get()),
                opacity
            );

            if (trigger->m_isMultiTriggered && TriggerTypeBoxes::multiTriggerIndicators) {
                drawnode->drawRect(
                    ccSub(pos, TriggerTypeBoxes::touchTriggerSize / 2 + TriggerTypeBoxes::multiTriggerDistance / 2),
                    ccAdd(pos, TriggerTypeBoxes::touchTriggerSize / 2 + TriggerTypeBoxes::multiTriggerDistance / 2), Col::Clear,
                    TriggerTypeBoxes::thickness / (TriggerTypeBoxes::scaleWithZoom ? editor::zoom() : 1.0f),
                    color_cast<ccColor4F>(misc::setOpacity(
                        col, sillyedit::utils::modifyOpacity(col.a, TriggerTypeBoxes::multiTriggerOpacity.get())
                    ))
                );
            }

            drawnode->drawRect(
                ccSub(pos, TriggerTypeBoxes::touchTriggerSize / 2), ccAdd(pos, TriggerTypeBoxes::touchTriggerSize / 2),
                color_cast<ccColor4F>(misc::setOpacity(
                    col, sillyedit::utils::modifyOpacity(col.a, TriggerTypeBoxes::touchTriggerFill.get())
                )),
                TriggerTypeBoxes::thickness / (TriggerTypeBoxes::scaleWithZoom ? editor::zoom() : 1.0f), color_cast<ccColor4F>(col)
            );

            return;
        }

        if (trigger->m_isSpawnTriggered && TriggerTypeBoxes::spawnTriggerIndicators) {
            const auto pos = ccAdd(
                trigger->getRealPosition(),
                sillyedit::utils::triggerHasBodyOffset(trigger->m_objectID)
                    ? sillyedit::utils::TRIGGER_BODY_OFFSET
                    : CCPointZero
            );
            const auto col = misc::setOpacity(
                TriggerTypeBoxes::chroma 
                    ? sillyedit::utils::getChroma<ccColor4B>() 
                    : color_cast<ccColor4B>(TriggerTypeBoxes::spawnTriggerColor.get()),
                opacity
            );

            if (trigger->m_isMultiTriggered && TriggerTypeBoxes::multiTriggerIndicators) {
                drawnode->drawCircle(
                    pos, TriggerTypeBoxes::spawnTriggerSize / 2 + TriggerTypeBoxes::multiTriggerDistance / 2, Col::Clear, 32,
                    TriggerTypeBoxes::thickness / (TriggerTypeBoxes::scaleWithZoom ? editor::zoom() : 1.0f),
                    color_cast<ccColor4F>(misc::setOpacity(
                        col, sillyedit::utils::modifyOpacity(col.a, TriggerTypeBoxes::multiTriggerOpacity.get())
                    ))
                );
            }

            drawnode->drawCircle(
                pos, TriggerTypeBoxes::spawnTriggerSize / 2,
                color_cast<ccColor4F>(misc::setOpacity(
                    col, sillyedit::utils::modifyOpacity(col.a, TriggerTypeBoxes::spawnTriggerFill.get())
                )),
                32, TriggerTypeBoxes::thickness / (TriggerTypeBoxes::scaleWithZoom ? editor::zoom() : 1.0f), color_cast<ccColor4F>(col)
            );

            return;
        }
    }



    void LevelEditorLayer::updateDebugDraw() {
        if (!TriggerTypeBoxes::enabled()) {
            return GD::LevelEditorLayer::updateDebugDraw();
        }

        const auto ret = m_drawTriggerBoxes;
        m_drawTriggerBoxes = false;

        GD::LevelEditorLayer::updateDebugDraw();

        m_drawTriggerBoxes = ret;

        object::forEachInSection([this] (GameObject* pObj) {
            this->drawTriggerTypeBoxes(pObj);
        });
    }
}