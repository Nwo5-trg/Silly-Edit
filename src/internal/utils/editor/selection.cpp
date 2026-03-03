#include "selection.hpp"
#include "object.hpp"

using namespace geode::prelude;

namespace Editor::Selection {
    void add(GameObject* pObj, bool pUndo, bool pUseFilter, bool pAlsoCheckLayers) {
        if (pUseFilter && pAlsoCheckLayers && !Object::canSelectLayer(pObj)) {
            return;
        }

        if (pUndo) {
            ui()->createUndoSelectObject(false);
        }
        
        ui()->selectObject(pObj, !pUseFilter);
    }
    void add(std::span<GameObject*> pObjs, bool pUndo, bool pUseFilter, bool pAlsoCheckLayers) {
        add(CCArrayExt(pObjs).inner(), pUndo, pUseFilter, pAlsoCheckLayers);
    }
    void add(cocos2d::CCArray* pObjs, bool pUndo, bool pUseFilter, bool pAlsoCheckLayers) {
        const auto checkLayers = pUseFilter && pAlsoCheckLayers;

        auto objs = checkLayers ? CCArray::create() : pObjs;

        if (checkLayers) {
            for (auto obj : CCArrayExt<GameObject*>(pObjs)) {
                if (Object::canSelectLayer(obj)) {
                    objs->addObject(obj);
                }
            }
        }

        if (pUndo) {
            ui()->createUndoSelectObject(false);
        }
        
        ui()->selectObjects(objs, !pUseFilter);
    }
    void set(GameObject* pObj, bool pUndo, bool pUseFilter, bool pAlsoCheckLayers) {
        if (pUseFilter && pAlsoCheckLayers && !Object::canSelectLayer(pObj)) {
            return;
        }

        if (pUndo) {
            ui()->createUndoSelectObject(false);
        }
        
        clear();
        
        ui()->selectObject(pObj, !pUseFilter);
    }
    void set(std::span<GameObject*> pObjs, bool pUndo, bool pUseFilter, bool pAlsoCheckLayers) {
        set(CCArrayExt(pObjs).inner(), pUndo, pUseFilter, pAlsoCheckLayers);
    }
    void set(cocos2d::CCArray* pObjs, bool pUndo, bool pUseFilter, bool pAlsoCheckLayers) {
        const auto checkLayers = pUseFilter && pAlsoCheckLayers;

        auto objs = checkLayers ? CCArray::create() : pObjs;

        if (checkLayers) {
            for (auto obj : CCArrayExt<GameObject*>(pObjs)) {
                if (Object::canSelectLayer(obj)) {
                    objs->addObject(obj);
                }
            }
        }
        
        if (pUndo) {
            ui()->createUndoSelectObject(false);
        }

        clear();
        
        ui()->selectObjects(pObjs, !pUseFilter);
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