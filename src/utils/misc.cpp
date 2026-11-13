#include <settings/include.hpp>
#include "include.hpp"

#define FTS_FUZZY_MATCH_IMPLEMENTATION
#include <Geode/external/fts/fts_fuzzy_match.h>

using namespace geode::prelude;

namespace sillyedit::utils {
    std::pair<float, float> getChromaSettings() {
        return {sillyedit::settings::sayoDeviceSensitivity, sillyedit::settings::sayoDeviceScreenBrightness};
    }

    bool isProbablierObjectString(std::string_view str) {
        if (str.find_first_of("1234567890") != 0) {
            return false;
        }

        if (str.ends_with(',') || str.ends_with('.') || misc::stringCount(str, ',') < 5) {
            return false;
        }

        return true;
    }

    bool triggerHasBodyOffset(int iD) {
        switch (iD) {
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

        return trigger::is(iD);
    }

    bool weightedFuzzyMatch(geode::ZStringView str, geode::ZStringView query, double wight, double& out) {
        int score;

        if (fts::fuzzy_match(query.c_str(), str.c_str(), score)) {
            out = std::max(out, score * wight);
            
            return true;
        }
        
        return false;
    } 

    bool modifierDown(const settings::Setting<std::string>& str) {
        // lollllll
        switch (str.get().length()) {
            case 3: return CCKeyboardDispatcher::get()->getAltKeyPressed();
            case 4: return CCKeyboardDispatcher::get()->getControlKeyPressed();
            case 5: return CCKeyboardDispatcher::get()->getShiftKeyPressed();
            case 7: return CCKeyboardDispatcher::get()->getCommandKeyPressed();
            case 12: return CCKeyboardDispatcher::get()->getControlKeyPressed() || CCKeyboardDispatcher::get()->getCommandKeyPressed();
            default: return false;
        }
    }

    CCTextInputNode* findUITextInputs(cocos2d::CCPoint pos) {
        std::vector<CCTextInputNode*> inputs;

        auto find = [&] (this auto&& self, CCNode* node) -> void {
            for (auto node : node->getChildrenExt()) {
                if (!node->isVisible()) {
                    continue;
                }

                if (node->getChildrenCount() >= 3 && typeinfo_cast<CCTextInputNode*>(node)) {
                    inputs.push_back(static_cast<CCTextInputNode*>(node));
                }
                else {
                    self(node);
                }
            }
        };

        for (auto node : std::initializer_list<CCNode*>{editor::ui()->m_scaleControl, editor::ui()->m_rotationControl, editor::ui()->m_transformControl, editor::ui()}) {
            find(node);
            
            for (auto input : inputs) {
                const auto rect = input->boundingBox();

                if (CCRect{rect.origin - rect.size / 2, rect.size}.containsPoint(
                    input->getParent()->convertToNodeSpace(pos)
                )) {
                    return input;
                }
            }
        }

        return nullptr;
    }
}