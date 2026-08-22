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

        auto pushVal = [&] (int pVal, bool pTryColorLabels) {
            if (pTryColorLabels && pVal >= 1000 && pVal <= 1012 && nwo5::editor::trigger::primaryTargetType(pObj) == editor::trigger::InputType::Color) {
                const static std::unordered_map<int, const char*> map{
                    {1004, "obj"}, {1000, "bg"}, {1001, "g1"}, {1009, "g2"},
                    {1013, "mg"}, {1014, "mg2"}, {1002, "line"}, {1003, "3dl"},
                    {1005, "p1"}, {1006, "p2"}, {1007, "lbg"}, {1010, "black"},
                    {1011, "white"}, {1012, "lighter"}
                };

                const auto it = map.find(pVal);

                if (it != map.end()) {
                    auto str = (*it).second;

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
                        val = editor::trigger::primaryTarget(pObj);
                    break; }
                    case Label::Secondary: {
                        val = editor::trigger::secondaryTarget(pObj);
                    break; }
                    case Label::PrimaryInput: {
                        val = editor::trigger::primaryInput(pObj);
                    break; }
                    case Label::SecondaryInput: {
                        val = editor::trigger::secondaryInput(pObj);
                    break; }
                }

                pushVal(val, label == Label::Primary);
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
            pushVal(editor::trigger::primaryTarget(pObj), true);
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
                return editor::trigger::activateGroup(pObj) ? config.color : config.offColor;
            }
            case Extra::ControlID: {
                return static_cast<EffectGameObject*>(pObj)->m_targetControlID ? config.color : config.offColor;
            }
            case Extra::PulseTarget: {
                return static_cast<EffectGameObject*>(pObj)->m_pulseTargetType == 1 ? config.color : config.offColor;
            }
            case Extra::Override: {
                return pObj->m_objectID == editor::trigger::PICKUP_TRIGGER 
                    ? (static_cast<CountTriggerGameObject*>(pObj)->m_isOverride ? config.color : config.offColor)
                    : config.offColor;
            }
            case Extra::Follow: {
                return editor::trigger::targetModeEnabled(pObj) ? config.color : config.offColor;
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