#include "include.hpp"

using namespace geode::prelude;

namespace GroupLabelShenanigans {
    LabelOptions::LabelOptions() {
        this->reset();
    }

    void LabelOptions::addConfig(int pID, const ObjectConfig& pConfig) {
        if (pID < m_objectConfigs.size() && pID > 0) {
            m_objectConfigs[pID] = pConfig;
        }
    }
    void LabelOptions::addExtra(int pID, const ExtraConfig& pConfig) {
        if (pID < m_extraConfigs.size() && pID > 0) {
            m_extraConfigs[pID] = pConfig;
        }
    }
    void LabelOptions::blacklist(int pID) {
        if (pID < m_blacklist.size() && pID > 0) {
            m_blacklist[pID] = true;
        }
    }

    void LabelOptions::stringForObject(char* pBuf, GameObject* pObj) {
        const auto id = pObj->m_objectID;

        if (id > editor::constants::OBJECT_IDS || id <= 0) {
            return;
        }

        if (m_blacklist[id]) {
            *pBuf = '\0';

            return;
        }

        const auto& config = m_objectConfigs[id];

        auto ptr = pBuf;

        auto pushVal = [&] (int pVal, bool pTryColorLabels, bool pTrySpecial) {
            if (pTryColorLabels && pVal >= 1000 && pVal <= 1014 && trigger::primaryTargetType(pObj) == trigger::InputType::Color) {
                const char* str = nullptr;

                switch (pVal) {
                    case 1000: str = "BG"; break;
                    case 1001: str = "G1"; break;
                    case 1002: str = "LINE"; break;
                    case 1003: str = "3DL"; break;
                    case 1004: str = "OBJ"; break;
                    case 1005: str = "P1"; break;
                    case 1006: str = "P2"; break;
                    case 1007: str = "LBG"; break;
                    case 1009: str = "G2"; break;
                    case 1010: str = "BLACK"; break;
                    case 1011: str = "WHITE"; break;
                    case 1012: str = "LIGHTER"; break;
                    case 1013: str = "MG"; break;
                    case 1014: str = "MG2"; break;
                    default: break;
                }

                if (str) {
                    while (*str) {
                        *(ptr++) = *(str++);
                    }
                    
                    return;
                }
            }

            if (pTrySpecial && pVal >= -11 && pVal <= -1) {
                const char* str = nullptr;

                switch (pVal) {
                    case -1: str = "P1"; break;
                    case -2: str = "P2"; break;
                    case -3: str = "C"; break;
                    case -4: str = "BL"; break;
                    case -5: str = "CL"; break;
                    case -6: str = "TL"; break;
                    case -7: str = "BC"; break;
                    case -8: str = "TC"; break;
                    case -9: str = "BR"; break;
                    case -10: str = "CR"; break;
                    case -11: str = "TR"; break;
                    default: break;
                }

                if (str) {
                    while (*str) {
                        *(ptr++) = *(str++);
                    }
                    
                    return;
                }
            }

            if (const auto [out, ec] = std::to_chars(ptr, pBuf + (LabelOptions::BUF_SIZE - 1), pVal); ec == std::errc{}) {
                ptr = out;
            }
        };

        auto pushRow = [&] (const auto& pVec) {
            bool first = true;
            for (const auto label : pVec) {
                if (!first) {
                    *(ptr++) = '/';
                }
                else { 
                    first = false;
                }

                int val = 0;
                switch (label) {
                    case Label::Primary: {
                        val = trigger::primaryTarget(pObj);
                    break; }
                    case Label::Secondary: {
                        val = trigger::secondaryTarget(pObj);
                    break; }
                    case Label::PrimaryInput: {
                        val = trigger::primaryInput(pObj);
                    break; }
                    case Label::SecondaryInput: {
                        val = trigger::secondaryInput(pObj);
                    break; }
                }

                pushVal(val, label == Label::Primary, label == Label::Secondary);
            }
        };

        if (!config.firstRow.empty() || !config.secondRow.empty() ) {
           pushRow(config.firstRow);

            if (!config.secondRow.empty()) {
                *(ptr++) = '\n';
                pushRow(config.secondRow);
            }
        }
        else {
            pushVal(trigger::primaryTarget(pObj), true, true);
        }

        *ptr = '\0';
    }
    std::optional<cocos2d::ccColor4B> LabelOptions::extraForObject(GameObject* pObj) {
        const auto id = pObj->m_objectID;

        if (id > editor::constants::OBJECT_IDS || id <= 0) {
            return std::nullopt;
        }

        const auto& config = m_extraConfigs[id];

        switch (config.type) {
            case Extra::ActivateGroup: {
                return trigger::activateGroup(pObj) ? config.color : config.offColor;
            }
            case Extra::ControlID: {
                return static_cast<EffectGameObject*>(pObj)->m_targetControlID ? config.color : config.offColor;
            }
            case Extra::PulseTarget: {
                return static_cast<EffectGameObject*>(pObj)->m_pulseTargetType == 1 ? config.color : config.offColor;
            }
            case Extra::Override: {
                return pObj->m_objectID == trigger::PICKUP_TRIGGER 
                    ? (static_cast<CountTriggerGameObject*>(pObj)->m_isOverride ? config.color : config.offColor)
                    : config.offColor;
            }
            case Extra::Follow: {
                return trigger::targetModeEnabled(pObj) ? config.color : config.offColor;
            }
            default: {
                return std::nullopt;
            }
        }
    }

    void LabelOptions::reset() {
        m_objectConfigs.fill({});
        m_extraConfigs.fill({});
        m_blacklist.fill(false);
    }
}