#pragma once

#include "editor.hpp"

namespace Editor::Selection {
    template<typename ImplT, typename T = std::remove_pointer_t<ImplT>>
    requires std::derived_from<T, GameObject>
    std::vector<T*> get() {
        if (auto obj = ui()->m_selectedObject) {
            return {obj};
        }
        else {
            return geode::cocos::CCArrayExt<T>(ui()->m_selectedObjects).toVector();
        }
    }
    template<typename ImplT = cocos2d::CCArray, typename T = std::remove_pointer_t<ImplT>>
    requires std::same_as<T, cocos2d::CCArray>
    T* get() {
        if (auto obj = ui()->m_selectedObject) {
            return cocos2d::CCArray::createWithObject(obj);
        }
        else {
            return ui()->m_selectedObjects;
        }
    }
    template<typename ImplT = GameObject, typename T = std::remove_pointer_t<ImplT>>
    auto getExt() {
        return geode::cocos::CCArrayExt<T*>(get());
    } 
    template<typename ImplT = GameObject, typename T = std::remove_pointer_t<ImplT>>
    requires std::derived_from<T, GameObject>
    T* getFirst() {
        if (auto obj = ui()->m_selectedObject) {
            return static_cast<T*>(obj);
        }
        else if (auto objs = ui()->m_selectedObjects; objs->count()) {
            return static_cast<T*>(objs->firstObject());
        }
        else {
            return nullptr;
        }
    }
    template<typename ImplT = GameObject, typename T = std::remove_pointer_t<ImplT>>
    requires std::derived_from<T, GameObject>
    std::vector<T*> getType() {
        if constexpr (std::same_as<T, GameObject>) {
            return get<T>();
        }
            
        if (auto obj = ui()->m_selectedObject; obj && geode::cast::typeinfo_cast<T*>(obj)) {
            return {static_cast<T*>(obj)};
        }
        else {
            std::vector<T*> out;
            out.reserve(ui()->m_selectedObjects->count());

            for (auto obj : geode::cocos::CCArrayExt<T*>(ui()->m_selectedObjects)) {
                if constexpr (std::derived_from<T, EffectGameObject>) {
                    if (obj->m_classType != GameObjectClassType::Effect) {
                        continue;
                    }
                }
                else if constexpr (std::derived_from<T, EnhancedGameObject>) {
                    if (obj->m_classType != GameObjectClassType::Enhanced) {
                        continue;
                    }
                }

                if (auto casted = geode::cast::typeinfo_cast<T*>(obj)) {
                    out.push_back(casted);
                }
            }

            return out;
        }
    }

    bool empty();
    size_t count();

    cocos2d::CCRect bounds();
    cocos2d::CCPoint center(bool pIgnoreParent = false);

    void add(GameObject* pObj, bool pUndo = false, bool pUseFilter = false, bool pAlsoCheckLayers = true);
    void add(std::span<GameObject*> pObjs, bool pUndo = false, bool pUseFilter = false, bool pAlsoCheckLayers = true);
    void add(cocos2d::CCArray* pObjs, bool pUndo = false, bool pUseFilter = false, bool pAlsoCheckLayers = true);
    void set(GameObject* pObj, bool pUndo = false, bool pUseFilter = false, bool pAlsoCheckLayers = true);
    void set(std::span<GameObject*> pObjs, bool pUndo = false, bool pUseFilter = false, bool pAlsoCheckLayers = true);
    void set(cocos2d::CCArray* pObjs, bool pUndo = false, bool pUseFilter = false, bool pAlsoCheckLayers = true);

    void clear(bool pUndo = false);
    void remove(GameObject* pObj, bool pUndo = false);
    void remove(std::span<GameObject*> pObjs, bool pUndo = false);
    void remove(cocos2d::CCArray* pObjs, bool pUndo = false);

    /// updates objects selection color again
    void repairColors(std::span<GameObject*> pObjs);
    /// updates objects selection color again
    void repairColors(cocos2d::CCArray* pObjs = get());
}