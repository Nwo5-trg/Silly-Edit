#include "include.hpp"

using namespace geode::prelude;

namespace GroupLabelShenanigans {
    LabelOptions::LabelOptions() {
        this->reset();
    }

    void LabelOptions::addConfig(int iD, const ObjectConfig& config) {
        if (iD < m_objectConfigs.size() && iD > 0) {
            m_objectConfigs[iD] = config;
        }
    }
    void LabelOptions::addExtra(int iD, const ExtraConfig& config) {
        if (iD < m_extraConfigs.size() && iD > 0) {
            m_extraConfigs[iD] = config;
        }
    }
    void LabelOptions::blacklist(int iD) {
        if (iD < m_blacklist.size() && iD > 0) {
            m_blacklist[iD] = true;
        }
    }

    void LabelOptions::stringForObject(char* buf, GameObject* obj) {
        const auto id = obj->m_objectID;

        if (id > editor::constants::OBJECT_IDS || id <= 0) {
            return;
        }

        if (m_blacklist[id]) {
            *buf = '\0';

            return;
        }

        const auto& config = m_objectConfigs[id];

        auto ptr = buf;

        auto pushVal = [&] (int val, bool tryColorLabels, bool trySpecial) {
            if (tryColorLabels && val >= 1000 && val <= 1014 && trigger::primaryTargetType(obj) == trigger::InputType::Color) {
                const char* str = nullptr;

                switch (val) {
                    case editor::constants::BACKGROUND_COLOR: str = "BG"; break;
                    case editor::constants::GROUND_COLOR: str = "G1"; break;
                    case editor::constants::LINE_COLOR: str = "LINE"; break;
                    case editor::constants::LINE_3D_COLOR: str = "3DL"; break;
                    case editor::constants::OBJECT_COLOR: str = "OBJ"; break;
                    case editor::constants::PLAYER_1_COLOR: str = "P1"; break;
                    case editor::constants::PLAYER_2_COLOR: str = "P2"; break;
                    case editor::constants::LIGHT_BACKGROUND_COLOR: str = "LBG"; break;
                    case editor::constants::GROUND_2_COLOR: str = "G2"; break;
                    case editor::constants::BLACK_COLOR: str = "BLACK"; break;
                    case editor::constants::WHITE_COLOR: str = "WHITE"; break;
                    case editor::constants::LIGHTER_COLOR: str = "LIGHTER"; break;
                    case editor::constants::MIDDLEGROUND_COLOR: str = "MG"; break;
                    case editor::constants::MIDDLEGROUND_2_COLOR: str = "MG2"; break;
                    default: break;
                }

                if (str) {
                    while (*str) {
                        *(ptr++) = *(str++);
                    }
                    
                    return;
                }
            }

            if (trySpecial && val >= -11 && val <= -1) {
                const char* str = nullptr;

                switch (val) {
                    case editor::constants::P1_AREA_SPECIAL_TARGET: str = "P1"; break;
                    case editor::constants::P2_AREA_SPECIAL_TARGET: str = "P2"; break;
                    case editor::constants::CAMERA_AREA_SPECIAL_TARGET: str = "C"; break;
                    case editor::constants::BOTTOM_LEFT_AREA_SPECIAL_TARGET: str = "BL"; break;
                    case editor::constants::CENTER_LEFT_AREA_SPECIAL_TARGET: str = "CL"; break;
                    case editor::constants::TOP_LEFT_AREA_SPECIAL_TARGET: str = "TL"; break;
                    case editor::constants::BOTTOM_CENTER_AREA_SPECIAL_TARGET: str = "BC"; break;
                    case editor::constants::TOP_CENTER_AREA_SPECIAL_TARGET: str = "TC"; break;
                    case editor::constants::BOTTOM_RIGHT_AREA_SPECIAL_TARGET: str = "BR"; break;
                    case editor::constants::CENTER_RIGHT_AREA_SPECIAL_TARGET: str = "CR"; break;
                    case editor::constants::TOP_RIGHT_AREA_SPECIAL_TARGET: str = "TR"; break;
                    default: break;
                }

                if (str) {
                    while (*str) {
                        *(ptr++) = *(str++);
                    }
                    
                    return;
                }
            }

            if (const auto [out, ec] = std::to_chars(ptr, buf + (LabelOptions::BUF_SIZE - 1), val); ec == std::errc{}) {
                ptr = out;
            }
        };

        auto pushRow = [&] (const auto& vec) {
            bool first = true;
            for (const auto label : vec) {
                if (!first) {
                    *(ptr++) = '/';
                }
                else { 
                    first = false;
                }

                int val = 0;
                switch (label) {
                    case Label::Primary: {
                        val = trigger::primaryTarget(obj);
                    break; }
                    case Label::Secondary: {
                        val = trigger::secondaryTarget(obj);
                    break; }
                    case Label::PrimaryInput: {
                        val = trigger::primaryInput(obj);
                    break; }
                    case Label::SecondaryInput: {
                        val = trigger::secondaryInput(obj);
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
            pushVal(trigger::primaryTarget(obj), true, true);
        }

        *ptr = '\0';
    }
    std::optional<cocos2d::ccColor4B> LabelOptions::extraForObject(GameObject* obj) {
        const auto id = obj->m_objectID;

        if (id > editor::constants::OBJECT_IDS || id <= 0) {
            return std::nullopt;
        }

        const auto& config = m_extraConfigs[id];

        switch (config.type) {
            case Extra::ActivateGroup: {
                return trigger::activateGroup(obj) ? config.color : config.offColor;
            }
            case Extra::ControlID: {
                return static_cast<EffectGameObject*>(obj)->m_targetControlID ? config.color : config.offColor;
            }
            case Extra::PulseTarget: {
                return static_cast<EffectGameObject*>(obj)->m_pulseTargetType == 1 ? config.color : config.offColor;
            }
            case Extra::Override: {
                return obj->m_objectID == trigger::PICKUP_TRIGGER 
                    ? (static_cast<CountTriggerGameObject*>(obj)->m_isOverride ? config.color : config.offColor)
                    : config.offColor;
            }
            case Extra::Follow: {
                return trigger::targetModeEnabled(obj) ? config.color : config.offColor;
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