#include <utils/utils.hpp>
#include "include.hpp"

using namespace geode::prelude;

namespace DefaultObjectOptions {
    static int stringToPropKey(const std::string& pString) {
        static std::unordered_map<std::string, int> stringToPropKeyMap {
            {"x", 2}, {"y", 3}, {"rotation", 6}, {"scale", 32}, {"scale_x", 128}, 
            {"scale_y", 129}, {"flip_horiz", 4}, {"flip_vert", 5}, {"warp_x_angle", 132}, 
            {"warp_y_angle", 131}, {"color_1point9", 19}, {"color_1", 21}, {"color_2", 22}, 
            {"single_color_type", 497}, {"no_glow", 96}, {"no_particle", 507}, 
            {"color_1_hsv_enabled", 41}, {"color_1_hsv", 43}, {"color_2_hsv_enabled", 42}, 
            {"color_2_hsv", 44}, {"color_1_index", 155}, {"color_2_index", 156}, 
            {"single_group", 33}, {"groups", 57}, {"parent_groups", 274}, {"group_parent", 34}, 
            {"area_parent", 279}, {"linked_group", 108}, {"editor_layer", 20}, 
            {"editor_layer_2", 61}, {"z_layer", 24}, {"z_order", 25}, {"ord", 115}, 
            {"channel", 170}, {"enter_channel", 343}, {"interactible", 36}, {"passable", 134}, 
            {"hide", 135}, {"non_stick_x", 136}, {"non_stick_y", 289}, {"extra_sticky", 495}, 
            {"extended_collision", 511}, {"ice_block", 137}, {"grip_slope", 193}, {"reverse", 117}, 
            {"material", 446}, {"control_id", 534}, {"multi_activate_classic", 99}, 
            {"no_multi_activate_platformer", 444}, {"dont_fade", 64}, {"dont_enter", 67}, 
            {"no_effects", 116}, {"dont_boost_x", 509}, {"dont_boost_y", 496}, {"single_ptouch", 284}, 
            {"high_detail", 103}, {"no_touch", 121}, {"center_effect", 369}, {"scale_stick", 356}, 
            {"no_audio_scale", 372}, {"preview", 13}, {"orange_tp_portal_distance", 54},
            {"custom_string", -1}
        };
        return stringToPropKeyMap[pString];
    }

    void parseOptions(ObjectOptions& pObjectOptions) {
        pObjectOptions.reset();

        const auto path = string::pathToString(
            Mod::get()->getConfigDir() / DefaultObjectOptions::path.get()
        ); 

        if (!asp::fs::exists(path)) {
            log::error("not real {}", path);
            return;
        }

        std::ifstream file(path);
        
        const auto parseJsonRes = matjson::parse(file);

        if (parseJsonRes.isErr()) {
            log::error("bad");
            return;
        }

        const auto json = parseJsonRes.unwrap();

        if (!json.isObject()) {
            log::error("not obj");
            return;
        }

        for (const auto& [idKey, options] : json) {
            const auto id = utils::numFromString<int>(idKey).unwrapOr(0);

            for (const auto& [prop, value] : options) {
                const auto key = DefaultObjectOptions::stringToPropKey(prop);

                if (!key) {
                    continue;
                }

                // thx chatgpt im far too lazy to do this
                switch (key) {
                    case 4: [[fallthrough]];
                    case 5: [[fallthrough]];
                    case 41: [[fallthrough]];
                    case 42: [[fallthrough]];
                    case 13: [[fallthrough]];
                    case 36: [[fallthrough]];
                    case 64: [[fallthrough]];
                    case 67: [[fallthrough]];
                    case 116: [[fallthrough]];
                    case 34: [[fallthrough]];
                    case 279: [[fallthrough]];
                    case 509: [[fallthrough]];
                    case 496: [[fallthrough]];
                    case 284: [[fallthrough]];
                    case 103: [[fallthrough]];
                    case 121: [[fallthrough]];
                    case 134: [[fallthrough]];
                    case 135: [[fallthrough]];
                    case 136: [[fallthrough]];
                    case 289: [[fallthrough]];
                    case 495: [[fallthrough]];
                    case 511: [[fallthrough]];
                    case 369: [[fallthrough]];
                    case 137: [[fallthrough]];
                    case 193: [[fallthrough]];
                    case 96: [[fallthrough]];
                    case 507: [[fallthrough]];
                    case 356: [[fallthrough]];
                    case 372: [[fallthrough]];
                    case 117: [[fallthrough]];
                    case 99: [[fallthrough]];
                    case 444: {
                        if (value.isBool()) {
                            pObjectOptions.addOption(id, key, value.asBool().unwrap());
                        }
                    break; }
                    case 21: [[fallthrough]];
                    case 22: [[fallthrough]];
                    case 497: [[fallthrough]];
                    case 155: [[fallthrough]];
                    case 156: [[fallthrough]];
                    case 33: [[fallthrough]];
                    case 20: [[fallthrough]];
                    case 61: [[fallthrough]];
                    case 24: [[fallthrough]];
                    case 25: [[fallthrough]];
                    case 115: [[fallthrough]];
                    case 170: [[fallthrough]];
                    case 108: [[fallthrough]];
                    case 343: [[fallthrough]];
                    case 446: {
                        if (value.isNumber()) {
                            pObjectOptions.addOption(id, key, value.asInt().unwrap());
                        }
                    break; }
                    case 2: [[fallthrough]];
                    case 3: [[fallthrough]];
                    case 6: [[fallthrough]];
                    case 32: [[fallthrough]];
                    case 128: [[fallthrough]];
                    case 129: [[fallthrough]];
                    case 131: [[fallthrough]];
                    case 132: [[fallthrough]];
                    case 54: {
                        if (value.isNumber()) {
                            pObjectOptions.addOption(id, key, value.asDouble().unwrap());
                        }
                    break; }
                    case 19: [[fallthrough]];
                    case 43: [[fallthrough]];
                    case 44: [[fallthrough]];
                    case 57: [[fallthrough]];
                    case 274: [[fallthrough]];
                    case 534: {
                        if (value.isString()) {
                            pObjectOptions.addOption(id, key, value.asString().unwrap());
                        }
                    break; }
                    default: {
                        if (value.isString()) {
                            pObjectOptions.addOption(id, value.asString().unwrap());
                        }
                    break; }
                }
            }
        }
    }
}