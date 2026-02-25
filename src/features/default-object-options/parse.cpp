#include "include.hpp"
#include "settings.hpp"

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

        if (!Settings::DefaultObjectOptions::useJSON.get()) {
            return pObjectOptions.updateSimpleOptionsString(
                Settings::DefaultObjectOptions::dontFade.get(),
                Settings::DefaultObjectOptions::dontEnter.get(),
                Settings::DefaultObjectOptions::noGlow.get()
            );
        }

        const auto path = string::pathToString(Mod::get()->getConfigDir() / Settings::DefaultObjectOptions::path.get()); 

        if (!std::filesystem::exists(path)) {
            return;
        }

        std::ifstream file(path);
        
        const auto parseJsonRes = matjson::parse(file);

        if (!parseJsonRes.isOk()) {
            return;
        }

        const auto json = parseJsonRes.unwrap();

        if (!json.isObject()) {
            return;
        }

        for (const auto& [idKey, options] : json) {
            const auto id = numFromString<int>(idKey).unwrapOr(0);

            for (const auto& [prop, value] : options) {
                const auto key = DefaultObjectOptions::stringToPropKey(prop);

                if (!key) {
                    continue;
                }

                // thx chatgpt im far too lazy to do this
                switch (key) {
                    case 4: [[__fallthrough__]];
                    case 5: [[__fallthrough__]];
                    case 41: [[__fallthrough__]];
                    case 42: [[__fallthrough__]];
                    case 13: [[__fallthrough__]];
                    case 36: [[__fallthrough__]];
                    case 64: [[__fallthrough__]];
                    case 67: [[__fallthrough__]];
                    case 116: [[__fallthrough__]];
                    case 34: [[__fallthrough__]];
                    case 279: [[__fallthrough__]];
                    case 509: [[__fallthrough__]];
                    case 496: [[__fallthrough__]];
                    case 284: [[__fallthrough__]];
                    case 103: [[__fallthrough__]];
                    case 121: [[__fallthrough__]];
                    case 134: [[__fallthrough__]];
                    case 135: [[__fallthrough__]];
                    case 136: [[__fallthrough__]];
                    case 289: [[__fallthrough__]];
                    case 495: [[__fallthrough__]];
                    case 511: [[__fallthrough__]];
                    case 369: [[__fallthrough__]];
                    case 137: [[__fallthrough__]];
                    case 193: [[__fallthrough__]];
                    case 96: [[__fallthrough__]];
                    case 507: [[__fallthrough__]];
                    case 356: [[__fallthrough__]];
                    case 372: [[__fallthrough__]];
                    case 117: [[__fallthrough__]];
                    case 99: [[__fallthrough__]];
                    case 444: {
                        if (value.isBool()) {
                            pObjectOptions.addOption(id, key, value.asBool().unwrap());
                        }
                    break; }
                    case 21: [[__fallthrough__]];
                    case 22: [[__fallthrough__]];
                    case 497: [[__fallthrough__]];
                    case 155: [[__fallthrough__]];
                    case 156: [[__fallthrough__]];
                    case 33: [[__fallthrough__]];
                    case 20: [[__fallthrough__]];
                    case 61: [[__fallthrough__]];
                    case 24: [[__fallthrough__]];
                    case 25: [[__fallthrough__]];
                    case 115: [[__fallthrough__]];
                    case 170: [[__fallthrough__]];
                    case 108: [[__fallthrough__]];
                    case 343: [[__fallthrough__]];
                    case 446: {
                        if (value.isNumber()) {
                            pObjectOptions.addOption(id, key, value.asInt().unwrap());
                        }
                    break; }
                    case 2: [[__fallthrough__]];
                    case 3: [[__fallthrough__]];
                    case 6: [[__fallthrough__]];
                    case 32: [[__fallthrough__]];
                    case 128: [[__fallthrough__]];
                    case 129: [[__fallthrough__]];
                    case 131: [[__fallthrough__]];
                    case 132: [[__fallthrough__]];
                    case 54: {
                        if (value.isNumber()) {
                            pObjectOptions.addOption(id, key, value.asDouble().unwrap());
                        }
                    break; }
                    case 19: [[__fallthrough__]];
                    case 43: [[__fallthrough__]];
                    case 44: [[__fallthrough__]];
                    case 57: [[__fallthrough__]];
                    case 274: [[__fallthrough__]];
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