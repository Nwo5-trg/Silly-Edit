#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

namespace TriggerIndicators {
    void Drawer::drawTargets(CCPoint pStart, bool pTargetingTrigger) {
        const auto& objs = pTargetingTrigger ? m_state.triggerTargets : m_state.objectTargets;
        const auto shouldCluster = pTargetingTrigger ? TriggerIndicators::clusterTriggers.get() : TriggerIndicators::clusterObjects;
        const auto shouldFallback = objs.size() > (pTargetingTrigger ? TriggerIndicators::triggerClustersMaxThreshold : TriggerIndicators::objectClustersMaxThreshold);
        const auto shouldRectFallback = !(pTargetingTrigger ? TriggerIndicators::triggerClusterLineFallback : TriggerIndicators::objectClusterLineFallback);

        if (!shouldCluster || (shouldFallback && !shouldRectFallback)) {
            for (auto obj : objs) {
                const auto end = pTargetingTrigger ? this->inputExtraPosFor(obj) : obj->getRealPosition();

                this->drawLine(pStart, end);

                if (!TriggerIndicators::alwaysDrawExtras && pTargetingTrigger) {
                    this->drawInputExtra(end, {ui::sx(obj), ui::sy(obj)}, obj->getOpacity() / 255.0f);
                }
            }

            return;
        }
        else if (shouldFallback && shouldRectFallback) {
            this->drawToRect(pStart, object::bounds(objs, true));

            return;
        }

        m_state.clusterResult.clear();
        object::cluster(m_state.clusterResult, objs, pTargetingTrigger ? TriggerIndicators::maxTriggerClusterDistance : TriggerIndicators::maxObjectClusterDistance);

        for (const auto& cluster : m_state.clusterResult) {
            if (cluster.size() == 1) {
                const auto obj = cluster.front();
                const auto end = pTargetingTrigger ? this->inputExtraPosFor(obj) : obj->getRealPosition();

                this->drawLine(pStart, end);

                if (!TriggerIndicators::alwaysDrawExtras && pTargetingTrigger) {
                    this->drawInputExtra(end, {ui::sx(obj), ui::sy(obj)}, obj->getOpacity() / 255.0f);
                }
            }
            else {
                this->drawToRect(pStart, object::bounds(cluster, true));
            }
        }
    }

    void Drawer::drawToRect(cocos2d::CCPoint pStart, cocos2d::CCRect pRect) {
        pRect.origin = ccSub(pRect.origin, m_state.thickness);
        pRect.size = ccAdd(pRect.size, m_state.thickness * 2);

        this->drawLine(pStart, sillyedit::utils::getLineCut(pStart, pRect));

        if (m_state.isCenter && TriggerIndicators::dottedCenterLines) {
            sillyedit::utils::getGridDraw()->drawDashedLine(
                pRect.origin, {pRect.origin.x, pRect.getMaxY()}, m_state.thickness, m_state.color, TriggerIndicators::dottedSegmentSize, TriggerIndicators::dottedDotSize
            );
            sillyedit::utils::getGridDraw()->drawDashedLine(
                {pRect.origin.x, pRect.getMaxY()}, pRect.origin + pRect.size, m_state.thickness, m_state.color, TriggerIndicators::dottedSegmentSize, TriggerIndicators::dottedDotSize
            );
            sillyedit::utils::getGridDraw()->drawDashedLine(
                pRect.origin + pRect.size, {pRect.getMaxX(), pRect.origin.y}, m_state.thickness, m_state.color, TriggerIndicators::dottedSegmentSize, TriggerIndicators::dottedDotSize
            );
            sillyedit::utils::getGridDraw()->drawDashedLine(
                {pRect.getMaxX(), pRect.origin.y}, pRect.origin, m_state.thickness, m_state.color, TriggerIndicators::dottedSegmentSize, TriggerIndicators::dottedDotSize
            );
        }
        else {
            sillyedit::utils::getGridDraw()->drawRect(
                pRect, Col::Clear, m_state.thickness, m_state.color
            );
        }
    }
    void Drawer::drawLine(cocos2d::CCPoint pStart, cocos2d::CCPoint pEnd) {
        if (m_state.isCenter && TriggerIndicators::dottedCenterLines) {
            sillyedit::utils::getGridDraw()->drawDashedLine(
                pStart, pEnd, m_state.thickness, m_state.color, TriggerIndicators::dottedSegmentSize, TriggerIndicators::dottedDotSize
            );
        }
        else {
            if (TriggerIndicators::drawLines) {
                sillyedit::utils::getGridDraw()->drawLine(
                    pStart, pEnd, m_state.thickness, m_state.color
                );
            }
            else {
                sillyedit::utils::getGridDraw()->drawSegment(
                    pStart, pEnd, m_state.thickness, m_state.color
                );
            }
        }
    }

    void Drawer::drawInputExtra(CCPoint pPos, CCSize pScale, float pOpacity) {
        auto drawnode = TriggerIndicators::drawExtrasBehind ? sillyedit::utils::getGridDraw() : sillyedit::utils::getOverlayDraw();

        const auto thickness = TriggerIndicators::thickness * ccMin(pScale) * 0.5f;

        pScale *= 3.5f * TriggerIndicators::extrasScale;

        drawnode->drawEllipse(
            pPos, pScale, misc::setOpacity(color_cast<ccColor4F>(TriggerIndicators::extrasFillCol.get()), pOpacity), 16,
            thickness, misc::setOpacity(color_cast<ccColor4F>(TriggerIndicators::extrasOutlineCol.get()), pOpacity)
        );
    }
    void Drawer::drawOutputExtra(CCPoint pPos, CCSize pScale, float pOpacity) {
        // lol
        if (TriggerIndicators::circleOutputExtras) {
            return drawInputExtra(pPos, pScale, pOpacity);
        }

        auto drawnode = TriggerIndicators::drawExtrasBehind ? sillyedit::utils::getGridDraw() : sillyedit::utils::getOverlayDraw();

        const auto thickness = TriggerIndicators::thickness * ccMin(pScale) * 0.5f;

        pScale.width *= 3.0f * TriggerIndicators::extrasScale;
        pScale.height *= 4.0f * TriggerIndicators::extrasScale;

        drawnode->drawPolygon(
            std::array{CCPoint{pPos.x - pScale.width, pPos.y - pScale.height}, CCPoint{pPos.x - pScale.width, pPos.y + pScale.height}, CCPoint{pPos.x + pScale.width, pPos.y}},
            misc::setOpacity(color_cast<ccColor4F>(TriggerIndicators::extrasFillCol.get()), pOpacity), thickness,
            misc::setOpacity(color_cast<ccColor4F>(TriggerIndicators::extrasOutlineCol.get()), pOpacity)
        );
    }

    cocos2d::CCPoint Drawer::inputExtraPosFor(GameObject* pObj) const {
        return pObj->getRealPosition() + (sillyedit::utils::triggerHasBodyOffset(pObj->m_objectID) ? sillyedit::utils::TRIGGER_BODY_OFFSET : CCPointZero) - CCPoint{TriggerIndicators::extrasOffset * pObj->getScaleX(), 0.0f};
    }
    std::pair<cocos2d::CCPoint, cocos2d::CCPoint> Drawer::outputExtraPosFor(GameObject* pObj, bool pHasCenter) const {
        const auto pos = pObj->getRealPosition() + (sillyedit::utils::triggerHasBodyOffset(pObj->m_objectID) ? sillyedit::utils::TRIGGER_BODY_OFFSET : CCPointZero);
        const auto hw = TriggerIndicators::extrasOffset * pObj->getScaleX();

        if (pHasCenter) {
            return {{pos.x + hw, pos.y + 5.0f}, {pos.x + hw, pos.y - 5.0f}};
        }
        else {
            return {{pos.x + hw, pos.y}, CCPointZero};
        }
    }

    void Drawer::updateTargets(int pGroup) {
        auto objs = editor::objectsWithGroup(pGroup);

        m_state.objectTargets.clear();
        m_state.triggerTargets.clear();

        const auto triggerPos = m_state.trigger->getRealPosition();
        const auto maxDistanceSQ = TriggerIndicators::maxDistance * TriggerIndicators::maxDistance;

        for (auto obj : CCArrayExt<GameObject*>(objs)) {
            const auto pos = obj->getRealPosition();
            const auto selectState = m_state.trigger->m_isSelected || obj->m_isSelected;

            if (!selectState && (TriggerIndicators::onlySelected || (maxDistanceSQ && sillyedit::utils::pointDistanceSQFast(triggerPos.x, pos.x, triggerPos.y, pos.y) > maxDistanceSQ))) {
                continue;
            }

            const auto isTrigger = sillyedit::utils::isTriggerFast(obj);

            if (!selectState || !TriggerIndicators::selectOverride) {
                if (TriggerIndicators::onlyTriggers && !isTrigger) {
                    continue;
                }

                if (isTrigger && TriggerIndicators::onlySpawn && !static_cast<EffectGameObject*>(obj)->m_isSpawnTriggered) {
                    continue;
                }
            }

            if (isTrigger) {
                m_state.triggerTargets.push_back(obj);
            }
            else {
                m_state.objectTargets.push_back(obj);
            }
        }
    }
    
    void Drawer::draw() {
        const auto shouldSelectionCull = !selection::empty() && std::ranges::any_of(selection::getExt(), [] (GameObject* pObj) {
            return object::hasGroups(pObj);
        });
        const auto cullDistance = std::max(
            ccMax(editor::size(false) / 2) * (shouldSelectionCull ? TriggerIndicators::cullMultiplierSelectionMod.get() : TriggerIndicators::cullMultiplier.get()),
            TriggerIndicators::minimumCullingSize.get()
        );
        const auto cullDistanceSQ = cullDistance * cullDistance;
        const auto center = editor::center(false);

        m_state.thickness = TriggerIndicators::thickness / (TriggerIndicators::scaleWithZoom ? editor::zoom() : 1.0f);

        for (auto obj : CCArrayExt<GameObject*>(editor::objectArray())) {
            if (!sillyedit::utils::isTriggerFast(obj)) {
                continue;
            };

            const auto id = obj->m_objectID;

            if (id > editor::constants::OBJECT_IDS) {
                continue;
            }

            const auto pos = obj->getRealPosition();

            if (!TriggerIndicators::noCulling && sillyedit::utils::pointDistanceSQFast(center.x, pos.x, center.y, pos.y) > cullDistanceSQ) {
                continue;
            }

            m_state.trigger = static_cast<EffectGameObject*>(obj);
            
            auto target = trigger::target(m_state.trigger);

            if (trigger::targetType(m_state.trigger) != trigger::InputType::Group) {
                target = false;
            }
            else if (0 > target || target > editor::constants::MAX_GROUPS || m_groupBlacklist[target]) {
                target = false;
            }

            auto center = trigger::center(m_state.trigger);

            if (trigger::centerType(m_state.trigger) != trigger::InputType::Group) {
                center = false;
            }
            else if (0 > center || center > editor::constants::MAX_GROUPS || m_groupBlacklist[center]) {
                center = false;
            }

            if (!target && !center) {
                continue;
            }

            const auto [targetOutPos, centerOutPos] = this->outputExtraPosFor(m_state.trigger, center);
            const CCSize scale{ui::sx(m_state.trigger), ui::sy(m_state.trigger)};
            const auto opacity = m_state.trigger->getOpacity() / 255.0f;

            if (TriggerIndicators::alwaysDrawExtras) {
                this->drawOutputExtra(targetOutPos, scale, opacity);

                if (center) {
                    this->drawOutputExtra(centerOutPos, scale, opacity);
                }
            }
            
            if (m_triggerBlacklist[id]) {
                continue;
            }

            if (TriggerIndicators::chroma) {
                const auto base = TriggerIndicators::chromaByObject ? obj->m_uniqueID : id;

                m_state.color = sillyedit::utils::getChroma<ccColor4F>(sillyedit::utils::hashInt(base) % 360);

                if (id == trigger::TOGGLE_TRIGGER && trigger::activateGroup(obj)) {
                    m_state.color = sillyedit::utils::getChroma<ccColor4F>((sillyedit::utils::hashInt(base) + 180) % 360);
                }
            }
            else {
                m_state.color = color_cast<ccColor4F>(trigger::color(id));

                if (id == trigger::TOGGLE_TRIGGER && trigger::activateGroup(obj)) {
                    m_state.color = {0.0f, 1.0f, 0.5f, 1.0f};
                }
            }

            m_state.color.a = m_state.trigger->getOpacity() / 255.0f;

            if (target) {
                this->updateTargets(target);
                
                if (!m_state.objectTargets.empty() || !m_state.triggerTargets.empty()) {
                    m_state.isCenter = false;

                    if (!m_state.objectTargets.empty()) {
                        this->drawTargets(targetOutPos, false);
                    }
                    if (!m_state.triggerTargets.empty()) {
                        this->drawTargets(targetOutPos, true);
                    }

                    if (!TriggerIndicators::alwaysDrawExtras) {
                        this->drawOutputExtra(targetOutPos, scale, opacity);
                    }
                }
            }
            if (center) {
                this->updateTargets(center);

                if (!m_state.objectTargets.empty() || !m_state.triggerTargets.empty()) {
                    m_state.isCenter = true;

                    if (!m_state.objectTargets.empty()) {
                        this->drawTargets(centerOutPos, false);
                    }
                    if (!m_state.triggerTargets.empty()) {
                        this->drawTargets(centerOutPos, true);
                    }

                    if (!TriggerIndicators::alwaysDrawExtras) {
                        this->drawOutputExtra(centerOutPos, scale, opacity);
                    }
                }
            }
        }
    }

    void Drawer::updateBlacklist() {
        m_groupBlacklist.fill(false);

        for (auto group : string::splitView(TriggerIndicators::groupBlacklist.get(), ",")) {
            const auto res = utils::numFromString<int>(group);

            if (res.isErr()) {
                continue;
            }

            if (const auto val = res.unwrap(); 0 < val && val <= editor::constants::MAX_GROUPS) {
                m_groupBlacklist[val] = true;
            }
        }

        m_triggerBlacklist.fill(false);

        for (auto trigger : string::splitView(TriggerIndicators::triggerBlacklist.get(), ",")) {
            const auto res = utils::numFromString<int>(trigger);

            if (res.isErr()) {
                continue;
            }

            if (const auto val = res.unwrap(); 0 < val && val <= editor::constants::OBJECT_IDS) {
                m_triggerBlacklist[val] = true;
            }
        }
    }
}