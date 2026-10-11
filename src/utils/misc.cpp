#include <settings/include.hpp>
#include "include.hpp"

#define FTS_FUZZY_MATCH_IMPLEMENTATION
#include <Geode/external/fts/fts_fuzzy_match.h>

using namespace geode::prelude;

namespace sillyedit::utils {
    std::pair<float, float> getChromaSettings() {
        return {sillyedit::settings::sayoDeviceSensitivity, sillyedit::settings::sayoDeviceScreenBrightness};
    }

    bool isProbablierObjectString(std::string_view pStr) {
        if (pStr.find_first_of("1234567890") != 0) {
            return false;
        }

        if (pStr.ends_with(',') || pStr.ends_with('.') || misc::stringCount(pStr, ',') < 5) {
            return false;
        }

        return true;
    }

    bool triggerHasBodyOffset(int pID) {
        switch (pID) {
            case 22: [[fallthrough]];
            case 24: [[fallthrough]];
            case 23: [[fallthrough]];
            case 25: [[fallthrough]];
            case 26: [[fallthrough]];
            case 27: [[fallthrough]];
            case 28: [[fallthrough]];
            case 55: [[fallthrough]];
            case 56: [[fallthrough]];
            case 57: [[fallthrough]];
            case 58: [[fallthrough]];
            case 59: [[fallthrough]];
            case 1816: [[fallthrough]];
            case 1915: [[fallthrough]];
            case 3640: [[fallthrough]];
            case 3643: {
                return false;
            break; }
            default: {};
        }

        return trigger::is(pID);
    }

    bool weightedFuzzyMatch(geode::ZStringView pStr, geode::ZStringView pQuery, double pWight, double& pOut) {
        int score;

        if (fts::fuzzy_match(pQuery.c_str(), pStr.c_str(), score)) {
            pOut = std::max(pOut, score * pWight);
            
            return true;
        }
        
        return false;
    } 

    bool modifierDown(const settings::SillySetting<std::string>& pStr) {
        // lollllll
        switch (pStr.get().length()) {
            case 3: return CCKeyboardDispatcher::get()->getAltKeyPressed();
            case 4: return CCKeyboardDispatcher::get()->getControlKeyPressed();
            case 5: return CCKeyboardDispatcher::get()->getShiftKeyPressed();
            case 7: return CCKeyboardDispatcher::get()->getCommandKeyPressed();
            case 12: return CCKeyboardDispatcher::get()->getControlKeyPressed() || CCKeyboardDispatcher::get()->getCommandKeyPressed();
            default: return false;
        }
    }

    CCTextInputNode* findUITextInputs(cocos2d::CCPoint pPos) {
        std::vector<CCTextInputNode*> inputs;

        auto find = [&] (this auto&& pSelf, CCNode* pNode) -> void {
            for (auto node : pNode->getChildrenExt()) {
                if (!node->isVisible()) {
                    continue;
                }

                if (node->getChildrenCount() >= 3 && typeinfo_cast<CCTextInputNode*>(node)) {
                    inputs.push_back(static_cast<CCTextInputNode*>(node));
                }
                else {
                    pSelf(node);
                }
            }
        };

        for (auto node : std::initializer_list<CCNode*>{editor::ui()->m_scaleControl, editor::ui()->m_rotationControl, editor::ui()->m_transformControl, editor::ui()}) {
            find(node);
            
            for (auto input : inputs) {
                const auto rect = input->boundingBox();

                if (CCRect{rect.origin - rect.size / 2, rect.size}.containsPoint(
                    input->getParent()->convertToNodeSpace(pPos)
                )) {
                    return input;
                }
            }
        }

        return nullptr;
    }
}