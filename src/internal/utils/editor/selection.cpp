#include "selection.hpp"
#include "object.hpp"

using namespace geode::prelude;

namespace Editor::Selection {
    void add(GameObject* pObj, bool pUndo, bool pUseFilter) {
        if (pUseFilter && !layer()->validGroup(pObj, true)) {
            return;
        }

        if (pUndo) {
            ui()->createUndoSelectObject(false);
        }
        
        ui()->selectObject(pObj, !pUseFilter);
    }
    void add(std::span<GameObject*> pObjs, bool pUndo, bool pUseFilter) {
        add(CCArrayExt(pObjs).inner(), pUndo, pUseFilter);
    }
    void add(cocos2d::CCArray* pObjs, bool pUndo, bool pUseFilter) {
        auto objsToSelect = pUseFilter ? CCArray::create() : CCArray::createWithArray(pObjs);

        if (pUseFilter) {
            for (auto obj : CCArrayExt<GameObject*>(pObjs)) {
                if (layer()->validGroup(obj, true)) {
                    objsToSelect->addObject(obj);
                }
            }
        }

        if (pUndo) {
            ui()->createUndoSelectObject(false);
        }
        
        ui()->selectObjects(pObjs, objsToSelect);
    }
    void set(GameObject* pObj, bool pUndo, bool pUseFilter) {
        if (pUseFilter && !layer()->validGroup(pObj, true)) {
            return;
        }

        if (pUndo) {
            ui()->createUndoSelectObject(false);
        }
        
        clear();
        
        ui()->selectObject(pObj, !pUseFilter);
    }
    void set(std::span<GameObject*> pObjs, bool pUndo, bool pUseFilter) {
        set(CCArrayExt(pObjs).inner(), pUndo, pUseFilter);
    }
    void set(cocos2d::CCArray* pObjs, bool pUndo, bool pUseFilter) {
        auto objsToSelect = pUseFilter ? CCArray::create() : CCArray::createWithArray(pObjs);

        if (pUseFilter) {
            for (auto obj : CCArrayExt<GameObject*>(pObjs)) {
                if (layer()->validGroup(obj, true)) {
                    objsToSelect->addObject(obj);
                }
            }
        }

        if (pUndo) {
            ui()->createUndoSelectObject(false);
        }

        clear();
        
        ui()->selectObjects(pObjs, objsToSelect);
    }

    bool empty() {
        return !count();
    }
    size_t count() {
        if (ui()->m_selectedObject) {
            return 1;
        }
        else {
            return ui()->m_selectedObjects->count();
        }
    }

    cocos2d::CCRect bounds() {
        return Editor::Object::bounds(get());
    }
    cocos2d::CCPoint center(bool pIgnoreParent) {
        return Editor::Object::center(get(), pIgnoreParent);
    }

    void clear(bool pUndo) {
        remove(get(), pUndo);
    }
    void remove(GameObject* pObj, bool pUndo) {
        if (pUndo) {
            ui()->createUndoSelectObject(false);
        }

        pObj->deselectObject();

        if (ui()->m_selectedObject == pObj) {
            ui()->m_selectedObject = nullptr;
        }
        
        ui()->m_selectedObjects->removeObject(pObj, false);
    }
    void remove(std::span<GameObject*> pObjs, bool pUndo) {
        if (pUndo) {
            ui()->createUndoSelectObject(false);
        }

        for (auto obj : pObjs) {
            remove(obj);
        }
    }
    void remove(cocos2d::CCArray* pObjs, bool pUndo) {
        if (pUndo) {
            ui()->createUndoSelectObject(false);
        }

        for (auto obj : CCArrayExt<GameObject*>(pObjs)) {
            remove(obj);
        }
    }

    void repairColors(std::span<GameObject*> pObjs) {
        for (auto obj : pObjs) {
            if (!obj->m_isSelected) {
                continue;
            }

            obj->updateObjectEditorColor();
        }
    }
    void repairColors(cocos2d::CCArray* pObjs) {
        for (auto obj : CCArrayExt<GameObject*>(pObjs)) {
            if (!obj->m_isSelected) {
                continue;
            }

            obj->updateObjectEditorColor();
        }
    }
}