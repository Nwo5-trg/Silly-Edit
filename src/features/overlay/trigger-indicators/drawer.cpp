// dont fucking null terminate the vectors to all or atleast find a way to almost never resize it mayb compare against size before doing so
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
            this->drawToRect(pStart, getObjectBounds(objs, true));

            return;
        }

        m_state.clusterResult.clear();
        clusterObjects(m_state.clusterResult, objs, pTargetingTrigger ? TriggerIndicators::maxTriggerClusterDistance : TriggerIndicators::maxObjectClusterDistance);

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
                this->drawToRect(pStart, getObjectBounds(cluster, true));
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

    cocos2d::CCPoint Drawer::inputExtraPosFor(GameObject* pObj) {
        return pObj->getRealPosition() + (sillyedit::utils::triggerHasBodyOffset(pObj->m_objectID) ? sillyedit::utils::TRIGGER_BODY_OFFSET : CCPointZero) - CCPoint{TriggerIndicators::extrasOffset * pObj->getScaleX(), 0.0f};
    }
    std::pair<cocos2d::CCPoint, cocos2d::CCPoint> Drawer::outputExtraPosFor(GameObject* pObj, bool pHasCenter) {
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

        const auto triggerPos = m_state.trigger->getRealPosition();
        const auto triggerSelected = m_state.trigger->m_isSelected;
        const auto maxDistanceSQ = TriggerIndicators::maxDistance * TriggerIndicators::maxDistance;

        const auto selectOverrideVal = TriggerIndicators::selectOverride.get();
        const auto onlySelectedVal = TriggerIndicators::onlyTriggers.get();
        const auto onlyTriggersVal = TriggerIndicators::onlyTriggers.get();
        const auto onlySpawnVal = TriggerIndicators::onlyTriggers.get();

        const auto size = objs->count();

        m_state.triggerTargets.clear();
        m_state.objectTargets.clear();

        for (auto obj : CCArrayExt<GameObject*>(objs)) {
            const auto selectState = triggerSelected || obj->m_isSelected;

            if (!selectState) {
                if (onlySelectedVal) {
                    continue;
                }

                // i dont think sillyedit::utils::pointdistancesqfast is actually being inlined here sooooo
                if (maxDistanceSQ) {
                    const float x = obj->m_positionX;
                    const float y = obj->m_positionY;

                    if ((triggerPos.x - x) * (triggerPos.x - x) + (triggerPos.y - y) * (triggerPos.y - y) > maxDistanceSQ) {
                        continue;
                    }
                }
            }

            const bool isTrigger = sillyedit::utils::isTriggerFast(obj);

            if (!selectState || !selectOverrideVal) {
                if (onlyTriggersVal && !isTrigger) {
                    continue;
                }

                if (isTrigger && onlySpawnVal && !static_cast<EffectGameObject*>(obj)->m_isSpawnTriggered) {
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

    void Drawer::clusterObjects(std::vector<std::vector<GameObject*>>& pOut, std::span<GameObject* const> pObjs, float pClusterSize) {
        static std::vector<GameObject*> queue;
        static std::unordered_map<GameObject*, int> map;
        
        queue.clear();
        map.clear();

        for (auto obj : pObjs) {
            if (map.contains(obj)) {
                continue;
            }

            const int currentClusterIndex = pOut.size();
            pOut.emplace_back();
            auto& currentCluster = pOut.back();

            queue.clear();
            queue.push_back(obj);
            map[obj] = currentClusterIndex;

            while (!queue.empty()) {
                auto currentObj = queue.back();
                const float x = currentObj->m_positionX;
                const float y = currentObj->m_positionY;
                queue.pop_back();
                currentCluster.push_back(currentObj);

                for (auto neighbour : pObjs) {
                    if (map.find(neighbour) != map.end()) {
                        continue;
                    }

                    if (std::abs(x - neighbour->m_positionX) <= pClusterSize && std::abs(y - neighbour->m_positionY) <= pClusterSize) {
                        map[neighbour] = currentClusterIndex;
                        queue.push_back(neighbour);
                    }
                }
            }
        }
    }
    CCRect Drawer::getObjectBounds(std::span<GameObject* const> pObjs, bool pAddSize) {
        CCPoint min = {std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
        CCPoint max = {std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest()};

        for (auto obj : pObjs) {
            const float x = obj->m_positionX;
            const float y = obj->m_positionY;
            const auto size = pAddSize ? ui::ssize(obj) / 2 : CCSizeZero;

            min.x = std::min(min.x, x - size.width);
            min.y = std::min(min.y, y - size.height);
            max.x = std::max(max.x, x + size.width);
            max.y = std::max(max.y, y + size.height);
        }

        return {min, max - min};
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

        for (auto obj : m_triggers) {
            if (!TriggerIndicators::noCulling && sillyedit::utils::pointDistanceSQFast(center.x, obj->m_positionX, center.y, obj->m_positionY) > cullDistanceSQ) {
                continue;
            }

            const auto id = obj->m_objectID;

            m_state.trigger = obj;
            
            auto target = trigger::target(obj);

            if (trigger::targetType(obj) != trigger::InputType::Group) {
                target = false;
            }
            else if (0 > target || target > editor::constants::MAX_GROUPS || m_groupBlacklist[target]) {
                target = false;
            }

            auto center = trigger::center(obj);

            if (trigger::centerType(obj) != trigger::InputType::Group) {
                center = false;
            }
            else if (0 > center || center > editor::constants::MAX_GROUPS || m_groupBlacklist[center]) {
                center = false;
            }

            if (!target && !center) {
                continue;
            }

            const auto [targetOutPos, centerOutPos] = this->outputExtraPosFor(obj, center);
            const CCSize scale{ui::sx(obj), ui::sy(obj)};
            const auto opacity = obj->getOpacity() / 255.0f;
            
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

            m_state.color.a = obj->getOpacity() / 255.0f;

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

            if (TriggerIndicators::alwaysDrawExtras) {
                this->drawOutputExtra(targetOutPos, scale, opacity);

                // obvious issue with this but no one will ever notice >:3 (and if they do ill just change it to a more obscure magic number lol)
                if (centerOutPos != CCPointZero) {
                    this->drawOutputExtra(centerOutPos, scale, opacity);
                }

                this->drawInputExtra(this->inputExtraPosFor(obj), scale, opacity);
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

    void Drawer::updateTriggerList() {
        m_triggers.clear();

        for (auto obj : CCArrayExt<GameObject*>(editor::objectArray())) {
            if (!sillyedit::utils::isTriggerFast(obj)) {
                continue;
            };

            const auto id = obj->m_objectID;

            if (id > editor::constants::OBJECT_IDS) {
                continue;
            }

            m_triggers.push_back(static_cast<EffectGameObject*>(obj));
        }
    }
}