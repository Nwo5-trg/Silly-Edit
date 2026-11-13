// dont fucking null terminate the vectors to all or atleast find a way to almost never resize it mayb compare against size before doing so
#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

namespace TriggerIndicators {
    void Drawer::drawTargets(CCPoint start, bool targetingTrigger) {
        const auto& objs = targetingTrigger ? m_state.triggerTargets : m_state.objectTargets;
        const auto shouldCluster = targetingTrigger ? TriggerIndicators::clusterTriggers.get() : TriggerIndicators::clusterObjects;
        const auto shouldFallback = objs.size() > (targetingTrigger ? TriggerIndicators::triggerClustersMaxThreshold : TriggerIndicators::objectClustersMaxThreshold);
        const auto shouldRectFallback = !(targetingTrigger ? TriggerIndicators::triggerClusterLineFallback : TriggerIndicators::objectClusterLineFallback);

        if (!shouldCluster || (shouldFallback && !shouldRectFallback)) {
            for (auto obj : objs) {

                const auto end = targetingTrigger ? this->inputExtraPosFor(obj) : obj->getRealPosition();

                this->drawLine(start, end);

                if (!TriggerIndicators::alwaysDrawExtras && targetingTrigger) {
                    this->drawInputExtra(end, {ui::sx(obj), ui::sy(obj)}, obj->getOpacity() / 255.0f);
                }
            }

            return;
        }
        else if (shouldFallback && shouldRectFallback) {
            this->drawToRect(start, getObjectBounds(objs, true));

            return;
        }

        m_state.clusterResult.clear();
        clusterObjects(m_state.clusterResult, objs, targetingTrigger ? TriggerIndicators::maxTriggerClusterDistance : TriggerIndicators::maxObjectClusterDistance);

        for (const auto& cluster : m_state.clusterResult) {
            if (cluster.size() == 1) {
                const auto obj = cluster.front();
                const auto end = targetingTrigger ? this->inputExtraPosFor(obj) : obj->getRealPosition();

                this->drawLine(start, end);

                if (!TriggerIndicators::alwaysDrawExtras && targetingTrigger) {
                    this->drawInputExtra(end, {ui::sx(obj), ui::sy(obj)}, obj->getOpacity() / 255.0f);
                }
            }
            else {
                this->drawToRect(start, getObjectBounds(cluster, true));
            }
        }
    }

    void Drawer::drawToRect(cocos2d::CCPoint start, cocos2d::CCRect rect) {
        rect.origin = ccSub(rect.origin, m_state.thickness);
        rect.size = ccAdd(rect.size, m_state.thickness * 2);

        this->drawLine(start, sillyedit::utils::getLineCut(start, rect));
        
        if (m_state.isCenter && TriggerIndicators::dottedCenterLines) {
            sillyedit::utils::getGridDraw()->drawDashedLine(
                rect.origin, {rect.origin.x, rect.getMaxY()}, m_state.thickness, m_state.color, TriggerIndicators::dottedSegmentSize, TriggerIndicators::dottedDotSize
            );
            sillyedit::utils::getGridDraw()->drawDashedLine(
                {rect.origin.x, rect.getMaxY()}, rect.origin + rect.size, m_state.thickness, m_state.color, TriggerIndicators::dottedSegmentSize, TriggerIndicators::dottedDotSize
            );
            sillyedit::utils::getGridDraw()->drawDashedLine(
                rect.origin + rect.size, {rect.getMaxX(), rect.origin.y}, m_state.thickness, m_state.color, TriggerIndicators::dottedSegmentSize, TriggerIndicators::dottedDotSize
            );
            sillyedit::utils::getGridDraw()->drawDashedLine(
                {rect.getMaxX(), rect.origin.y}, rect.origin, m_state.thickness, m_state.color, TriggerIndicators::dottedSegmentSize, TriggerIndicators::dottedDotSize
            );
        }
        else {
            sillyedit::utils::getGridDraw()->drawRect(
                rect, Col::Clear, m_state.thickness, m_state.color
            );
        }
    }
    void Drawer::drawLine(cocos2d::CCPoint start, cocos2d::CCPoint end) {
        if (m_state.isCenter && TriggerIndicators::dottedCenterLines) {
            sillyedit::utils::getGridDraw()->drawDashedLine(
                start, end, m_state.thickness, m_state.color, TriggerIndicators::dottedSegmentSize, TriggerIndicators::dottedDotSize
            );
        }
        else {
            if (TriggerIndicators::drawLines) {
                sillyedit::utils::getGridDraw()->drawLine(
                    start, end, m_state.thickness, m_state.color
                );
            }
            else {
                sillyedit::utils::getGridDraw()->drawSegment(
                    start, end, m_state.thickness, m_state.color
                );
            }
        }
    }

    void Drawer::drawInputExtra(CCPoint pos, CCSize scale, float opacity) {
        auto drawnode = TriggerIndicators::drawExtrasBehind ? sillyedit::utils::getGridDraw() : sillyedit::utils::getOverlayDraw();

        const auto thickness = TriggerIndicators::thickness * ccMin(scale) * 0.5f;

        scale *= 3.5f * TriggerIndicators::extrasScale;

        drawnode->drawEllipse(
            pos, scale, misc::setOpacity(color_cast<ccColor4F>(TriggerIndicators::extrasFillCol.get()), opacity), 16,
            thickness, misc::setOpacity(color_cast<ccColor4F>(TriggerIndicators::extrasOutlineCol.get()), opacity)
        );
    }
    void Drawer::drawOutputExtra(CCPoint pos, CCSize scale, float opacity) {
        // lol
        if (TriggerIndicators::circleOutputExtras) {
            return drawInputExtra(pos, scale, opacity);
        }

        auto drawnode = TriggerIndicators::drawExtrasBehind ? sillyedit::utils::getGridDraw() : sillyedit::utils::getOverlayDraw();

        const auto thickness = TriggerIndicators::thickness * ccMin(scale) * 0.5f;

        scale.width *= 3.0f * TriggerIndicators::extrasScale;
        scale.height *= 4.0f * TriggerIndicators::extrasScale;

        drawnode->drawPolygon(
            std::array{CCPoint{pos.x - scale.width, pos.y - scale.height}, CCPoint{pos.x - scale.width, pos.y + scale.height}, CCPoint{pos.x + scale.width, pos.y}},
            misc::setOpacity(color_cast<ccColor4F>(TriggerIndicators::extrasFillCol.get()), opacity), thickness,
            misc::setOpacity(color_cast<ccColor4F>(TriggerIndicators::extrasOutlineCol.get()), opacity)
        );
    }

    cocos2d::CCPoint Drawer::inputExtraPosFor(GameObject* obj) {
        return obj->getRealPosition() + (sillyedit::utils::triggerHasBodyOffset(obj->m_objectID) ? sillyedit::utils::TRIGGER_BODY_OFFSET : CCPointZero) - CCPoint{TriggerIndicators::extrasOffset * obj->getScaleX(), 0.0f};
    }
    std::pair<cocos2d::CCPoint, cocos2d::CCPoint> Drawer::outputExtraPosFor(GameObject* obj, bool hasCenter) {
        const auto pos = obj->getRealPosition() + (sillyedit::utils::triggerHasBodyOffset(obj->m_objectID) ? sillyedit::utils::TRIGGER_BODY_OFFSET : CCPointZero);
        const auto hw = TriggerIndicators::extrasOffset * obj->getScaleX();

        if (hasCenter) {
            return {{pos.x + hw, pos.y + 5.0f}, {pos.x + hw, pos.y - 5.0f}};
        }
        else {
            return {{pos.x + hw, pos.y}, CCPointZero};
        }
    }

    void Drawer::updateTargets(int group) {
        auto objs = editor::objectsWithGroup(group);

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

    void Drawer::clusterObjects(std::vector<std::vector<GameObject*>>& out, std::span<GameObject* const> objs, float clusterSize) {
        static std::vector<GameObject*> queue;
        static std::unordered_map<GameObject*, int> map;
        
        queue.clear();
        map.clear();

        for (auto obj : objs) {
            if (map.contains(obj)) {
                continue;
            }

            const int currentClusterIndex = out.size();
            out.emplace_back();
            auto& currentCluster = out.back();

            queue.clear();
            queue.push_back(obj);
            map[obj] = currentClusterIndex;

            while (!queue.empty()) {
                auto currentObj = queue.back();
                const float x = currentObj->m_positionX;
                const float y = currentObj->m_positionY;
                queue.pop_back();
                currentCluster.push_back(currentObj);

                for (auto neighbour : objs) {
                    if (map.find(neighbour) != map.end()) {
                        continue;
                    }

                    if (std::abs(x - neighbour->m_positionX) <= clusterSize && std::abs(y - neighbour->m_positionY) <= clusterSize) {
                        map[neighbour] = currentClusterIndex;
                        queue.push_back(neighbour);
                    }
                }
            }
        }
    }
    CCRect Drawer::getObjectBounds(std::span<GameObject* const> objs, bool addSize) {
        CCPoint min = {std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
        CCPoint max = {std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest()};

        for (auto obj : objs) {
            const float x = obj->m_positionX;
            const float y = obj->m_positionY;
            const auto size = addSize ? ui::ssize(obj) / 2 : CCSizeZero;

            min.x = std::min(min.x, x - size.width);
            min.y = std::min(min.y, y - size.height);
            max.x = std::max(max.x, x + size.width);
            max.y = std::max(max.y, y + size.height);
        }

        return {min, max - min};
    }
    
    void Drawer::draw() {
        const auto shouldSelectionCull = !selection::empty() && std::ranges::any_of(selection::getExt(), [] (GameObject* obj) {
            return object::hasGroups(obj);
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