#include <utils/include.hpp>
#include "include.hpp"

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

namespace SetupStartpos {
    void LevelEditorLayer::setupStartpos(StartPosObject* pStartpos) {
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
                case 13: [[fallthrough]];
                case 47: [[fallthrough]];
                case 111: [[fallthrough]];
                case 660: [[fallthrough]];
                case 745: [[fallthrough]];
                case 1331: [[fallthrough]];
                case 1933: {
                    if (!gamemodeObj || x >= gamemodeObj->getRealPosition().x) {
                        gamemodeObj = obj;
                    }
                break; }
                case 200: [[fallthrough]];
                case 201: [[fallthrough]];
                case 202: [[fallthrough]];
                case 203: [[fallthrough]];
                case 1334: {
                    if (!speedObj || x >= speedObj->getRealPosition().x) {
                        speedObj = obj;
                    }
                break; }
                case 99: [[fallthrough]];
                case 101: {
                    if (!miniObj || x >= miniObj->getRealPosition().x) {
                        miniObj = obj;
                    }
                break; }
                case 45: [[fallthrough]];
                case 46: {
                    if (!mirrorObj || x >= mirrorObj->getRealPosition().x) {
                        mirrorObj = obj;
                    }
                break; }
                case 286: [[fallthrough]];
                case 287: {
                    if (!dualObj || x >= dualObj->getRealPosition().x) {
                        dualObj = obj;
                    }
                break; }
                case 747: {
                    if (!static_cast<TeleportPortalObject*>(obj)->m_gravityMode) { // unset
                        break;
                    }
                [[fallthrough]]; }
                case 67: [[fallthrough]];
                case 3004: [[fallthrough]];
                case 3005: [[fallthrough]];
                case 10: [[fallthrough]];
                case 11: [[fallthrough]];
                case 84: [[fallthrough]];
                case 1022: [[fallthrough]];
                case 2926: [[fallthrough]];
                case 1751: {
                    gravityObjs.push_back(obj);
                break;}
                default: break;
            }
        }

        if (SetupStartpos::gamemode) {
            pStartpos->m_startSettings->m_startMode = gamemodeObj ? portalToGamemode(gamemodeObj) : m_levelSettings->m_startMode;
        }
        if (SetupStartpos::freeMode && gamemodeObj) {
            // clean startpos ues this for freemode
            pStartpos->m_isIceBlock = static_cast<EffectGameObject*>(gamemodeObj)->m_cameraIsFreeMode;
        }
        if (SetupStartpos::speed) {
            pStartpos->m_startSettings->m_startSpeed = speedObj ? portalToSpeed(speedObj) : m_levelSettings->m_startSpeed;
        }
        if (SetupStartpos::mini) {
            pStartpos->m_startSettings->m_startMini = miniObj ? miniObj->m_objectID == 101 : m_levelSettings->m_startMini;
        }
        if (SetupStartpos::mirror) {
            pStartpos->m_startSettings->m_mirrorMode = mirrorObj ? mirrorObj->m_objectID == 45 : m_levelSettings->m_mirrorMode;
        }
        if (SetupStartpos::dual) {
            pStartpos->m_startSettings->m_startDual = dualObj ? dualObj->m_objectID == 286 : m_levelSettings->m_startDual;
        }
        if (SetupStartpos::gravity) {
            std::ranges::sort(gravityObjs, [] (auto pA, auto pB) {
                return pA->getRealPosition().x < pB->getRealPosition().x;
            });

            bool flip = m_levelSettings->m_isFlipped;

            for (auto obj : gravityObjs) {
                switch (obj->m_objectID) {
                    case 67: [[fallthrough]];
                    case 3004: [[fallthrough]];
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
                    case 84: [[fallthrough]]; // gravity *flipping* object
                    case 1022: [[fallthrough]];
                    case 2926: [[fallthrough]];
                    case 1751: {
                        flip = !flip;
                    break;}
                }
            }

            pStartpos->m_startSettings->m_isFlipped = flip;
        }
    }



    GameObject* LevelEditorLayer::createObject(int objectID, CCPoint position, bool noUndo) {
        GameObject* ret = GD::LevelEditorLayer::createObject(objectID, position, noUndo);

        if (SetupStartpos::enabled() && Sillyedit::shouldApplyCustomPlacedObjectOptions() && objectID == 31) {
            setupStartpos(static_cast<StartPosObject*>(ret));
        }
        
        return ret;
    }
}