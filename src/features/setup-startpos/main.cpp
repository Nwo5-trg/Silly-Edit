#include <Geode/modify/LevelEditorLayer.hpp>
#include <internal/utils/utils.hpp>
#include "settings.hpp"

using namespace geode::prelude;

static int portalToGamemode(GameObject* pObj) {
    switch (pObj->m_objectID) {
        case 13: return 1;
        case 47: return 2;
        case 111: return 3;
        case 660: return 4;
        case 745: return 5;
        case 1331: return 6;
        case 1933: return 7;
        default: return 0;
    }
}

static Speed portalToSpeed(GameObject* pObj) {
    switch (pObj->m_objectID) {
        case 200: return Speed::Slow;
        case 202: return Speed::Fast;
        case 203: return Speed::Faster;
        case 1334: return Speed::Fastest;
        default: return Speed::Normal;
    }
}

class $modify(LevelEditorLayer) {
    GameObject* createObject(int objectID, CCPoint position, bool noUndo) {
        GameObject* ret = LevelEditorLayer::createObject(objectID, position, noUndo);

        if (Settings::SetupStartpos::enabled.get() && !noUndo && ret && ret->m_isStartPos) {
            setupStartpos(static_cast<StartPosObject*>(ret));
        }
        
        return ret;
    }
    
    void setupStartpos(StartPosObject* pStartpos) {
        const auto startposX = pStartpos->getRealPosition().x;
        
        GameObject* gamemodeObj = nullptr;
        GameObject* speedObj = nullptr;
        GameObject* miniObj = nullptr;
        GameObject* mirrorObj = nullptr;
        GameObject* dualObj = nullptr;

        std::vector<GameObject*> gravityObjs;

        for (auto obj : CCArrayExt<GameObject>(m_objects)) {
            // hopefully make things quicker
            if (obj->m_isDecoration || obj->m_isNoTouch) {
                continue;
            }

            const auto x = obj->getRealPosition().x;

            if (x > startposX) {
                continue;
            }

            const auto id = obj->m_objectID;

            switch (id) {
                case 13: [[__fallthrough__]];
                case 47: [[__fallthrough__]];
                case 111: [[__fallthrough__]];
                case 660: [[__fallthrough__]];
                case 745: [[__fallthrough__]];
                case 1331: [[__fallthrough__]];
                case 1933: {
                    if (!gamemodeObj || x >= gamemodeObj->getRealPosition().x) {
                        gamemodeObj = obj;
                    }
                break; }
                case 200: [[__fallthrough__]];
                case 201: [[__fallthrough__]];
                case 202: [[__fallthrough__]];
                case 203: [[__fallthrough__]];
                case 1334: {
                    if (!speedObj || x >= speedObj->getRealPosition().x) {
                        speedObj = obj;
                    }
                break; }
                case 99: [[__fallthrough__]];
                case 101: {
                    if (!miniObj || x >= miniObj->getRealPosition().x) {
                        miniObj = obj;
                    }
                break; }
                case 45: [[__fallthrough__]];
                case 46: {
                    if (!mirrorObj || x >= mirrorObj->getRealPosition().x) {
                        mirrorObj = obj;
                    }
                break; }
                case 286: [[__fallthrough__]];
                case 287: {
                    if (!dualObj || x >= dualObj->getRealPosition().x) {
                        dualObj = obj;
                    }
                break; }
                case 747: {
                    if (!static_cast<TeleportPortalObject*>(obj)->m_gravityMode) { // unset
                        break;
                    }
                [[__fallthrough__]]; }
                case 67: [[__fallthrough__]];
                case 3004: [[__fallthrough__]];
                case 3005: [[__fallthrough__]];
                case 10: [[__fallthrough__]];
                case 11: [[__fallthrough__]];
                case 84: [[__fallthrough__]];
                case 1022: [[__fallthrough__]];
                case 2926: [[__fallthrough__]];
                case 1751: {
                    gravityObjs.push_back(obj);
                break;}
                default: break;
            }
        }

        if (Settings::SetupStartpos::gamemode.get()) {
            pStartpos->m_startSettings->m_startMode = gamemodeObj ? portalToGamemode(gamemodeObj) : m_levelSettings->m_startMode;
        }
        if (Settings::SetupStartpos::freeMode.get() && gamemodeObj) {
            // clean startpos ues this for freemode
            pStartpos->m_isIceBlock = static_cast<EffectGameObject*>(gamemodeObj)->m_cameraIsFreeMode;
        }
        if (Settings::SetupStartpos::speed.get()) {
            pStartpos->m_startSettings->m_startSpeed = speedObj ? portalToSpeed(speedObj) : m_levelSettings->m_startSpeed;
        }
        if (Settings::SetupStartpos::mini.get()) {
            pStartpos->m_startSettings->m_startMini = miniObj ? miniObj->m_objectID == 101 : m_levelSettings->m_startMini;
        }
        if (Settings::SetupStartpos::mirror.get()) {
            pStartpos->m_startSettings->m_mirrorMode = mirrorObj ? mirrorObj->m_objectID == 45 : m_levelSettings->m_mirrorMode;
        }
        if (Settings::SetupStartpos::dual.get()) {
            pStartpos->m_startSettings->m_startDual = dualObj ? dualObj->m_objectID == 286 : m_levelSettings->m_startDual;
        }
        if (Settings::SetupStartpos::gravity.get()) {
            std::ranges::sort(gravityObjs, [] (auto pA, auto pB) {
                return pA->getRealPosition().x < pB->getRealPosition().x;
            });

            bool flip = m_levelSettings->m_isFlipped;

            for (auto obj : gravityObjs) {
                switch (obj->m_objectID) {
                    case 67: [[__fallthrough__]];
                    case 3004: [[__fallthrough__]];
                    case 3005: {
                        flip = !obj->isFacingDown();
                    break; }
                    case 10: {
                        flip = false;
                    break;}
                    case 11: {
                        flip = true;
                    break; }
                    case 747: { // portal is a special little girl
                        auto portal = static_cast<TeleportPortalObject*>(obj);

                        if (portal->m_gravityMode == 3) {
                            flip = !flip;
                        }
                        else {
                            flip = portal->m_gravityMode == 2; // flipped is 2
                        }
                    break; }
                    case 84: [[__fallthrough__]]; // gravity *flipping* object
                    case 1022: [[__fallthrough__]];
                    case 2926: [[__fallthrough__]];
                    case 1751: {
                        flip = !flip;
                    break;}
                }
            }

            pStartpos->m_startSettings->m_isFlipped = flip;
        }
    }
};