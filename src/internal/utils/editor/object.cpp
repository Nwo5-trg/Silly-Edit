#include "object.hpp"
#include "selection.hpp"
#include "editor.hpp"

using namespace geode::prelude;

namespace Editor::Object {
    size_t count() {
        return getAll()->count();
    }

    cocos2d::CCArray* getAll() {
        return layer()->m_objects;
    }
    cocos2d::CCArray* getWithGroup(int pGroup) {
        if (auto ptr = layer()->m_groupDict->objectForKey(pGroup)) {
            return static_cast<CCArray*>(ptr);
        }
        else {
            return CCArray::create();
        }
    }

    std::vector<int> groups(GameObject* pObj) {
        std::vector<int> out;

        if (!pObj->m_groups) {
            return out;
        }

        for (const auto group : *pObj->m_groups) {
            if (group) {
                out.push_back(group);
            }
        }

        return out;
    }
    std::vector<int> groups(std::span<GameObject*> pObjs, bool pSort) {
        std::unordered_set<int> groups;

        for (auto obj : pObjs) {
            if (!obj->m_groups) {
                continue;
            }

            for (const auto group : *obj->m_groups) {
                if (group) {
                    groups.insert(group);
                }
            }
        }

        std::vector<int> out = {groups.begin(), groups.end()};

        if (pSort) {
            std::ranges::sort(out);
        }

        return out;
    }
    std::vector<int> groups(cocos2d::CCArray* pObjs, bool pSort) {
        std::unordered_set<int> groups;

        for (auto obj : CCArrayExt<GameObject*>(pObjs)) {
            if (!obj->m_groups) {
                continue;
            }

            for (const auto group : *obj->m_groups) {
                if (group) {
                    groups.insert(group);
                }
            }
        }

        std::vector<int> out = {groups.begin(), groups.end()};

        if (pSort) {
            std::ranges::sort(out);
        }

        return out;
    }
    bool hasGroups(GameObject* pObj) {
        if (!pObj->m_groups) {
            return false;
        }

        for (const auto group : *pObj->m_groups) {
            if (group) {
                return true;
            }
        }

        return false;
    }
    bool hasGroup(GameObject* pObj, int pGroup) {
        return pObj->m_groups && std::ranges::contains(*pObj->m_groups, pGroup);
    }
    bool sharesGroup(std::span<GameObject*> pObjs, int pGroup) {
        if (pObjs.empty()) {
            return false;
        }

        for (auto obj : pObjs) {
            if (!hasGroup(obj, pGroup)) {
                return false;
            }
        }

        return true;
    }
    bool sharesGroup(cocos2d::CCArray* pObjs, int pGroup) {
        if (!pObjs->count()) {
            return false;
        }

        for (auto obj : CCArrayExt<GameObject*>(pObjs)) {
            if (!hasGroup(obj, pGroup)) {
                return false;
            }
        }

        return true;
    }

    bool hasColor(GameObject* pObj, int pColor, bool pPrimary) {
        const auto col = pPrimary ? pObj->m_baseColor : pObj->m_detailColor;

        return col && pColor == (col->m_colorID ? col->m_colorID : col->m_defaultColorID);
    }

    int id(GameObject* pObj) {
        return pObj->m_objectID;
    }
    std::vector<int> ids(std::span<GameObject*> pObjs, bool pSort) {
        std::unordered_set<int> ids;

        for (auto obj : pObjs) {
            ids.insert(obj->m_objectID);
        }

       std::vector<int> out = {ids.begin(), ids.end()};

        if (pSort) {
            std::ranges::sort(out);
        }

        return out;
    }
    std::vector<int> ids(cocos2d::CCArray* pObjs, bool pSort) {
        std::unordered_set<int> ids;

        for (auto obj : CCArrayExt<GameObject*>(pObjs)) {
            ids.insert(obj->m_objectID);
        }

        std::vector<int> out = {ids.begin(), ids.end()};

        if (pSort) {
            std::ranges::sort(out);
        }

        return out;
    }

    bool canSelectLayer(GameObject* pObj) {
        return Editor::layerSelectable(pObj->m_editorLayer) || Editor::layerSelectable(pObj->m_editorLayer2);
    }

    cocos2d::CCRect bounds(std::span<GameObject*> pObjs, bool pAddSize) {
        if (pObjs.empty()) {
            return CCRectZero;
        }

        CCPoint min = {std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
        CCPoint max = {std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest()};

        for (auto obj : pObjs) {
            const auto pos = obj->getRealPosition();
            const auto size = pAddSize ? obj->getScaledContentSize() / 2 : CCSizeZero;

            min.x = std::min(min.x, pos.x - size.width);
            min.y = std::min(min.y, pos.y - size.height);
            max.x = std::max(max.x, pos.x + size.width);
            max.y = std::max(max.y, pos.y + size.height);
        }

        return {min, max - min};
    }
    cocos2d::CCRect bounds(cocos2d::CCArray* pObjs, bool pAddSize) {
        if (!pObjs->count()) {
            return CCRectZero;
        }

        CCPoint min = {std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
        CCPoint max = {std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest()};

        for (auto obj : CCArrayExt<GameObject*>(pObjs)) {
            const auto pos = obj->getRealPosition();
            const auto size = pAddSize ? obj->getScaledContentSize() / 2 : CCSizeZero;

            min.x = std::min(min.x, pos.x - size.width);
            min.y = std::min(min.y, pos.y - size.height);
            max.x = std::max(max.x, pos.x + size.width);
            max.y = std::max(max.y, pos.y + size.height);
        }

        return {min, max - min};
    }
    cocos2d::CCPoint center(std::span<GameObject*> pObjs, bool pIgnoreParent) {
        if (pObjs.empty()) {
            return CCPointZero;
        }

        CCPoint min = {std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
        CCPoint max = {std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest()};

        GameObject* parent = nullptr;
        bool multipleParents = false;

        for (auto obj : pObjs) {
            const auto pos = obj->getRealPosition();

            min.x = std::min(min.x, pos.x);
            min.y = std::min(min.y, pos.y);
            max.x = std::max(max.x, pos.x);
            max.y = std::max(max.y, pos.y);

            if (!pIgnoreParent && obj->m_hasGroupParent) {
                // highkey code golfing but like who even cares
                parent = (multipleParents = parent) ? parent : obj;
            }
        }

        if (parent && !multipleParents) {
            return parent->getRealPosition();
        } 

        return (min + max) / 2;
    }
    cocos2d::CCPoint center(cocos2d::CCArray* pObjs, bool pIgnoreParent) {
        if (!pObjs->count()) {
            return CCPointZero;
        }

        CCPoint min = {std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
        CCPoint max = {std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest()};

        GameObject* parent = nullptr;
        bool multipleParents = false;

        for (auto obj : CCArrayExt<GameObject*>(pObjs)) {
            const auto pos = obj->getRealPosition();

            min.x = std::min(min.x, pos.x);
            min.y = std::min(min.y, pos.y);
            max.x = std::max(max.x, pos.x);
            max.y = std::max(max.y, pos.y);

            if (!pIgnoreParent && obj->m_hasGroupParent) {
                // highkey code golfing but like who even cares
                parent = (multipleParents = parent) ? parent : obj;
            }
        }

        if (parent && !multipleParents) {
            return parent->getRealPosition();
        } 

        return (min + max) / 2;
    }

    float size(GameObject* pObj) {
        return sizeForID(id(pObj)); 
    }
    float sizeForID(int pID) {
        return ObjectToolbox::sharedState()->gridNodeSizeForKey(pID); 
    }

    CCPoint snappedPos(GameObject* pObj, float pGridSize) {
        const auto id = Object::id(pObj);
        const auto offset = ui()->offsetForKey(id);
        const auto pos = pObj->getRealPosition() - offset;
        const auto gridSize = pGridSize * (Object::size(pObj) / 30.0f);

        return CCPoint{
            std::round(pos.x / gridSize) * gridSize,
            std::round(pos.y / gridSize) * gridSize
        } + offset;
    }

    void remove(GameObject* pObj, bool pUndo) {
        Editor::Selection::remove(pObj);

        layer()->removeObject(pObj, !pUndo);
    }
    void remove(std::span<GameObject*> pObjs, bool pUndo) {
        if (pUndo && !pObjs.empty()) {
            layer()->addToUndoList(UndoObject::createWithArray(CCArrayExt<GameObject*>(pObjs).inner(), UndoCommand::DeleteMulti), false);
        }

        Editor::Selection::remove(pObjs);

        for (auto obj : pObjs) {
            layer()->removeObject(obj, true);
        }
    }
    void remove(cocos2d::CCArray* pObjs, bool pUndo) {
        if (pUndo && pObjs->count()) {
            layer()->addToUndoList(UndoObject::createWithArray(pObjs, UndoCommand::DeleteMulti), false);
        }

        Editor::Selection::remove(pObjs);

        for (auto obj : CCArrayExt<GameObject*>(pObjs)) {
            layer()->removeObject(obj, true);
        }
    }

    void move(GameObject* pObj, cocos2d::CCPoint pTo, bool pUndo) {
        moveBy(pObj, pTo - pObj->getRealPosition());
    }
    void move(std::span<GameObject*> pObjs, cocos2d::CCPoint pTo, bool pUndo, CCPoint pCenter) {
        moveBy(pObjs, pTo - (pCenter == AUTO_CENTER ? center(pObjs) : pCenter));
    }
    void move(cocos2d::CCArray* pObjs, cocos2d::CCPoint pTo, bool pUndo, CCPoint pCenter) {
        moveBy(pObjs, pTo - (pCenter == AUTO_CENTER ? center(pObjs) : pCenter));
    }
    void moveBy(GameObject* pObj, cocos2d::CCPoint pOff, bool pUndo) {
        if (pUndo) {
            Impl::createUndoObject(UndoCommand::Transform);
        }

        ui()->moveObject(pObj, pOff);
    }
    void moveBy(std::span<GameObject*> pObjs, cocos2d::CCPoint pOff, bool pUndo) {
        if (pObjs.empty()) {
            return;
        }

        if (pUndo) {
            Impl::createUndoObject(UndoCommand::Transform);
        }

        for (auto obj : pObjs) {
            ui()->moveObject(obj, pOff);
        }
    }
    void moveBy(cocos2d::CCArray* pObjs, cocos2d::CCPoint pOff, bool pUndo) {
        if (!pObjs->count()) {
            return;
        }

        if (pUndo) {
            Impl::createUndoObject(UndoCommand::Transform);
        }

        for (auto obj : CCArrayExt<GameObject*>(pObjs)) {
            ui()->moveObject(obj, pOff);
        }
    }

    void scale(std::span<GameObject*> pObjs, float pX, float pY, bool pUndo, cocos2d::CCPoint pCenter, bool pMove) {
        CCPoint maxScale = {std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest()};

        for (auto obj : pObjs) {
            maxScale.x = std::max(maxScale.x, obj->m_scaleX);
            maxScale.y = std::max(maxScale.y, obj->m_scaleY);
        }

        scaleBy(pObjs, pX / (maxScale.x ? maxScale.x : 1.0f), pY / (maxScale.y ? maxScale.y : 1.0f), pUndo, pCenter, pMove);
    }
    void scale(std::span<GameObject*> pObjs, float pTo, bool pUndo, cocos2d::CCPoint pCenter, bool pMove) {
        float maxScale = std::numeric_limits<float>::lowest();

        for (auto obj : pObjs) {
            maxScale = std::max(maxScale, std::max(obj->m_scaleX, obj->m_scaleY));
        }

        if (!maxScale) {
            return;
        }

        const auto mod = pTo / maxScale;

        scaleBy(pObjs, mod, mod, pUndo, pCenter, pMove);
    }
    void scaleX(std::span<GameObject*> pObjs, float pTo, bool pUndo, cocos2d::CCPoint pCenter, bool pMove) {
        float maxScale = std::numeric_limits<float>::lowest();

        for (auto obj : pObjs) {
            maxScale = std::max(maxScale, obj->m_scaleX);
        }

        if (!maxScale) {
            return;
        }

        scaleXBy(pObjs, pTo / maxScale, pUndo, pCenter, pMove);
    }
    void scaleY(std::span<GameObject*> pObjs, float pTo, bool pUndo, cocos2d::CCPoint pCenter, bool pMove) {
        float maxScale = std::numeric_limits<float>::lowest();

        for (auto obj : pObjs) {
            maxScale = std::max(maxScale, obj->m_scaleY);
        }

        if (!maxScale) {
            return;
        }

        scaleYBy(pObjs, pTo / maxScale, pUndo, pCenter, pMove);
    }
    void scale(cocos2d::CCArray* pObjs, float pX, float pY, bool pUndo, cocos2d::CCPoint pCenter, bool pMove) {
        CCPoint maxScale = {std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest()};

        for (auto obj : CCArrayExt<GameObject*>(pObjs)) {
            maxScale.x = std::max(maxScale.x, obj->m_scaleX);
            maxScale.y = std::max(maxScale.y, obj->m_scaleY);
        }

        scaleBy(pObjs, pX / (maxScale.x ? maxScale.x : 1.0f), pY / (maxScale.y ? maxScale.y : 1.0f), pUndo, pCenter, pMove);
    }
    void scale(cocos2d::CCArray* pObjs, float pTo, bool pUndo, cocos2d::CCPoint pCenter, bool pMove) {
        float maxScale = std::numeric_limits<float>::lowest();

        for (auto obj : CCArrayExt<GameObject*>(pObjs)) {
            maxScale = std::max(maxScale, std::max(obj->m_scaleX, obj->m_scaleY));
        }

        if (!maxScale) {
            return;
        }

        const auto mod = pTo / maxScale;

        scaleBy(pObjs, mod, mod, pUndo, pCenter, pMove);
    }
    void scaleX(cocos2d::CCArray* pObjs, float pTo, bool pUndo, cocos2d::CCPoint pCenter, bool pMove) {
        float maxScale = std::numeric_limits<float>::lowest();

        for (auto obj : CCArrayExt<GameObject*>(pObjs)) {
            maxScale = std::max(maxScale, obj->m_scaleX);
        }

        if (!maxScale) {
            return;
        }

        scaleXBy(pObjs, pTo / maxScale, pUndo, pCenter, pMove);
    }
    void scaleY(cocos2d::CCArray* pObjs, float pTo, bool pUndo, cocos2d::CCPoint pCenter, bool pMove) {
        float maxScale = std::numeric_limits<float>::lowest();

        for (auto obj : CCArrayExt<GameObject*>(pObjs)) {
            maxScale = std::max(maxScale, obj->m_scaleY);
        }

        if (!maxScale) {
            return;
        }

        scaleYBy(pObjs, pTo / maxScale, pUndo, pCenter, pMove);
    }
    void scaleBy(std::span<GameObject*> pObjs, float pX, float pY, bool pUndo, cocos2d::CCPoint pCenter, bool pMove) {
        if (pObjs.empty()) {
            return;
        }

        if (pCenter == AUTO_CENTER) {
            pCenter = center(pObjs);
        }

        if (pUndo) {
            Impl::createUndoObject(UndoCommand::Transform);
        }

        for (auto obj : pObjs) {
            if (pMove) {
                move(obj, pCenter + (obj->getRealPosition() - pCenter) * CCSize{pX, pY});
            }

            if (pX != 1.0f) {
                obj->updateCustomScaleX(obj->m_scaleX * pX);
            }
            if (pY != 1.0f) {
                obj->updateCustomScaleY(obj->m_scaleY * pY);
            }
        }
    }
    void scaleBy(std::span<GameObject*> pObjs, float pMod, bool pUndo, cocos2d::CCPoint pCenter, bool pMove) {
        scaleBy(pObjs, pMod, pMod, pUndo, pCenter, pMove);
    }
    void scaleXBy(std::span<GameObject*> pObjs, float pMod, bool pUndo, cocos2d::CCPoint pCenter, bool pMove) {
        if (pObjs.empty() || !pMod || pMod == 1.0f) {
            return;
        }

        if (pCenter == AUTO_CENTER) {
            pCenter = center(pObjs);
        }

        if (pUndo) {
            Impl::createUndoObject(UndoCommand::Transform);
        }

        for (auto obj : pObjs) {
            if (pMove) {
                move(obj, pCenter + (obj->getRealPosition() - pCenter) * CCSize{pMod, 1.0f});
            }

            obj->updateCustomScaleX(obj->m_scaleX * pMod);
        }
    }
    void scaleYBy(std::span<GameObject*> pObjs, float pMod, bool pUndo, cocos2d::CCPoint pCenter, bool pMove) {
        if (pObjs.empty() || !pMod || pMod == 1.0f) {
            return;
        }

        if (pCenter == AUTO_CENTER) {
            pCenter = center(pObjs);
        }

        if (pUndo) {
            Impl::createUndoObject(UndoCommand::Transform);
        }

        for (auto obj : pObjs) {
            if (pMove) {
                move(obj, pCenter + (obj->getRealPosition() - pCenter) * CCSize{1.0f, pMod});
            }

            obj->updateCustomScaleY(obj->m_scaleY * pMod);
        }
    }
    void scaleBy(cocos2d::CCArray* pObjs, float pX, float pY, bool pUndo, cocos2d::CCPoint pCenter, bool pMove) {
        if (!pObjs->count()) {
            return;
        }
        
        if (pCenter == AUTO_CENTER) {
            pCenter = center(pObjs);
        }

        if (pUndo) {
            Impl::createUndoObject(UndoCommand::Transform);
        }

        for (auto obj : CCArrayExt<GameObject*>(pObjs)) {
            if (pMove) {
                move(obj, pCenter + (obj->getRealPosition() - pCenter) * CCSize{pX, pY});
            }

            if (pX && pX != 1.0f) {
                obj->updateCustomScaleX(obj->m_scaleX * pX);
            }
            if (pY && pY != 1.0f) {
                obj->updateCustomScaleY(obj->m_scaleY * pY);
            }
        }
    }
    void scaleBy(cocos2d::CCArray* pObjs, float pMod, bool pUndo, cocos2d::CCPoint pCenter, bool pMove) {
        scaleBy(pObjs, pMod, pMod, pUndo, pCenter, pMove);
    }
    void scaleXBy(cocos2d::CCArray* pObjs, float pMod, bool pUndo, cocos2d::CCPoint pCenter, bool pMove) {
        if (!pObjs->count() || !pMod || pMod == 1.0f) {
            return;
        }
        
        if (pCenter == AUTO_CENTER) {
            pCenter = center(pObjs);
        }

        if (pUndo) {
            Impl::createUndoObject(UndoCommand::Transform);
        }

        for (auto obj : CCArrayExt<GameObject*>(pObjs)) {        
            if (pMove) {
                move(obj, pCenter + (obj->getRealPosition() - pCenter) * CCSize{pMod, 1.0f});
            }

            obj->updateCustomScaleX(obj->m_scaleX * pMod);
        }
    }
    void scaleYBy(cocos2d::CCArray* pObjs, float pMod, bool pUndo, cocos2d::CCPoint pCenter, bool pMove) {
        if (!pObjs->count() || !pMod || pMod == 1.0f) {
            return;
        }
        
        if (pCenter == AUTO_CENTER) {
            pCenter = center(pObjs);
        }

        if (pUndo) {
            Impl::createUndoObject(UndoCommand::Transform);
        }

        for (auto obj : CCArrayExt<GameObject*>(pObjs)) {        
            if (pMove) {
                move(obj, pCenter + (obj->getRealPosition() - pCenter) * CCSize{1.0f, pMod});
            }

            obj->updateCustomScaleY(obj->m_scaleY * pMod);
        }
    }
}