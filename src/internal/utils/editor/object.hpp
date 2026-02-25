#pragma once

#include "editor.hpp"

namespace Editor::Object {
    size_t count();

    cocos2d::CCArray* getAll();
    cocos2d::CCArray* getWithGroup(int pGroup);

    std::vector<int> groups(GameObject* pObj);
    std::vector<int> groups(std::span<GameObject*> pObjs, bool pSort = false);
    std::vector<int> groups(cocos2d::CCArray* pObjs, bool pSort = false);
    bool hasGroups(GameObject* pObj);
    bool hasGroup(GameObject* pObj, int pGroup);
    bool sharesGroup(std::span<GameObject*> pObjs, int pGroup);
    bool sharesGroup(cocos2d::CCArray* pObjs, int pGroup);

    bool hasColor(GameObject* pObj, int pColor, bool pPrimary);

    int id(GameObject* pObj);
    std::vector<int> ids(std::span<GameObject*> pObjs, bool pSort = false);
    std::vector<int> ids(cocos2d::CCArray* pObjs, bool pSort = false);

    cocos2d::CCRect bounds(std::span<GameObject*> pObjs, bool pAddSize = false);
    cocos2d::CCRect bounds(cocos2d::CCArray* pObjs, bool pAddSize = false);
    cocos2d::CCPoint center(std::span<GameObject*> pObjs, bool pIgnoreParent = false);
    cocos2d::CCPoint center(cocos2d::CCArray* pObjs, bool pIgnoreParent = false);

    float size(GameObject* pObj);
    float sizeForID(int pID);

    cocos2d::CCPoint snappedPos(GameObject* pObj, float pGridSize = 30.0f);

    void remove(GameObject* pObj, bool pUndo = false);
    void remove(std::span<GameObject*> pObjs, bool pUndo = false);
    void remove(cocos2d::CCArray* pObjs, bool pUndo = false);

    void move(GameObject* pObj, cocos2d::CCPoint pTo, bool pUndo = false);
    void move(std::span<GameObject*> pObjs, cocos2d::CCPoint pTo, bool pUndo = false, cocos2d::CCPoint pCenter = AUTO_CENTER);
    void move(cocos2d::CCArray* pObjs, cocos2d::CCPoint pTo, bool pUndo = false, cocos2d::CCPoint pCenter = AUTO_CENTER);
    void moveBy(GameObject* pObj, cocos2d::CCPoint pOff, bool pUndo = false);
    void moveBy(std::span<GameObject*> pObjs, cocos2d::CCPoint pOff, bool pUndo = false);
    void moveBy(cocos2d::CCArray* pObjs, cocos2d::CCPoint pOff, bool pUndo = false);

    void scale(std::span<GameObject*> pObjs, float pX, float pY, bool pUndo = false, cocos2d::CCPoint pCenter = AUTO_CENTER, bool pMove = true);
    void scale(std::span<GameObject*> pObjs, float pTo, bool pUndo = false, cocos2d::CCPoint pCenter = AUTO_CENTER, bool pMove = true);
    void scale(cocos2d::CCArray* pObjs, float pX, float pY, bool pUndo = false, cocos2d::CCPoint pCenter = AUTO_CENTER, bool pMove = true);
    void scale(cocos2d::CCArray*  pObjs, float pTo, bool pUndo = false, cocos2d::CCPoint pCenter = AUTO_CENTER, bool pMove = true);
    void scaleBy(std::span<GameObject*> pObjs, float pX, float pY, bool pUndo = false, cocos2d::CCPoint pCenter = AUTO_CENTER, bool pMove = true);
    void scaleBy(std::span<GameObject*> pObjs, float pMod, bool pUndo = false, cocos2d::CCPoint pCenter = AUTO_CENTER, bool pMove = true);
    void scaleBy(cocos2d::CCArray* pObjs, float pX, float pY, bool pUndo = false, cocos2d::CCPoint pCenter = AUTO_CENTER, bool pMove = true);
    void scaleBy(cocos2d::CCArray* pObjs, float pMod, bool pUndo = false, cocos2d::CCPoint pCenter = AUTO_CENTER, bool pMove = true);
}