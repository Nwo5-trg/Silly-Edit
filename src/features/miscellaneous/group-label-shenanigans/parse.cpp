#include "include.hpp"

using namespace geode::prelude;

static std::filesystem::path getPath() {
    if (GroupLabelShenanigans::mode == "More Info") {
        return Mod::get()->getResourcesDir() / "more-info.json";
    }
    else if (GroupLabelShenanigans::mode == "Vanilla") {
        return Mod::get()->getResourcesDir() / "vanilla.json";
    }
    
    return Mod::get()->getConfigDir() / GroupLabelShenanigans::customPath.get();
}

static std::optional<GroupLabelShenanigans::Label> toLabel(std::string_view pStr) {
    static StringMap<GroupLabelShenanigans::Label> map{
        {"primary", GroupLabelShenanigans::Label::Primary},
        {"secondary", GroupLabelShenanigans::Label::Secondary},
        {"primary-input", GroupLabelShenanigans::Label::PrimaryInput},
        {"secondary-input", GroupLabelShenanigans::Label::SecondaryInput}
    };

    const auto it = map.find(pStr);

    return it == map.end() ? std::nullopt : std::optional<GroupLabelShenanigans::Label>((*it).second);
}

static std::optional<GroupLabelShenanigans::Extra> toExtra(std::string_view pStr) {
    static StringMap<GroupLabelShenanigans::Extra> map{
        {"activate-group", GroupLabelShenanigans::Extra::ActivateGroup},
        {"control-id", GroupLabelShenanigans::Extra::ControlID},
        {"pulse-target", GroupLabelShenanigans::Extra::PulseTarget},
        {"override", GroupLabelShenanigans::Extra::Override},
        {"follow", GroupLabelShenanigans::Extra::Follow}
    };

    const auto it = map.find(pStr);

    return it == map.end() ? std::nullopt : std::optional<GroupLabelShenanigans::Extra>((*it).second);
}

namespace GroupLabelShenanigans {
    void parseLabels(LabelOptions& pLabelOptions) {
        pLabelOptions.reset();

        const auto path = string::pathToString(getPath()); 

        if (!asp::fs::exists(path)) {
            log::error("(group label shenanigans) not real path {}", path);
            return;
        }

        std::ifstream file(path);
        
        const auto parseJsonRes = matjson::parse(file);

        if (parseJsonRes.isErr()) {
            log::error("(group label shenanigans) bad json");
            return;
        }

        const auto json = parseJsonRes.unwrap();

        if (!json.isObject()) {
            log::error("(group label shenanigans) bad json");
            return;
        }

        for (const auto& [key, obj] : json) {
            if (key == "blacklist" && obj.isArray()) {
                const auto& blacklist = obj.asArray().unwrap();

                for (const auto& id : blacklist) {
                    if (id.isExactlyUInt()) {
                        const auto num = id.asInt().unwrap();

                        if (num) {
                            pLabelOptions.blacklist(num);
                        }
                    }
                }

                continue;
            }

            if (key == "extras" && obj.isObject()) {
                for (const auto& [extraStr, entry] : obj) {
                    if (!entry.isObject()) {
                        continue;
                    }

                    const auto extra = toExtra(extraStr);

                    if (!extra.has_value() || !entry.contains("objects") || !entry.contains("color")) {
                        continue;
                    }

                    ExtraConfig config;
                    config.type = extra.value();

                    const auto& colObj = entry.get("color").unwrap();

                    if (!colObj.isString()) {
                        continue;
                    }

                    const auto col = cocos::cc4bFromHexString(colObj.asString().unwrap());

                    if (col.isErr()) {
                        continue;
                    }
 
                    config.color = col.unwrap();

                    if (entry.contains("off-color")) {
                        const auto& offColRes = entry.get("off-color").unwrap();

                        if (offColRes.isString()) {
                            const auto res = cocos::cc4bFromHexString(offColRes.asString().unwrap());
                            
                            if (res.isOk()) {
                                config.offColor = res.unwrap();
                            }
                        }
                    }

                    const auto& objects = entry.get("objects").unwrap();

                    if (!objects.isArray()) {
                        continue;
                    }

                    std::vector<int> ids;
                    for (const auto& id : objects.asArray().unwrap()) {
                        if (id.isNumber()) {
                            if (const auto num = id.asInt().unwrap()) {
                                ids.push_back(num);
                            }
                        }
                    }

                    for (const auto id : ids) {
                        pLabelOptions.addExtra(id, config);
                    }
                }

                continue;
            }

            const auto idRes = utils::numFromString<int>(key);

            if (idRes.isErr()) {
                continue;
            }

            const auto id = idRes.unwrap();

            ObjectConfig config;

            if (obj.isString()) {
                const auto res = toLabel(obj.asString().unwrap());

                if (res.has_value()) {
                    config.firstRow.push_back(res.value());

                    pLabelOptions.addConfig(id, std::move(config));
                }
            }
            else if (obj.isArray()) {
                const auto& array = obj.asArray().unwrap();

                if (array.empty() || array.size() > 4) {
                    continue;
                }

                if (array.front().isArray()) {
                    if (array.size() != 2 || !array.back().isArray()) {
                        continue;
                    }


                    for (const auto& label : array.front()) {
                        if (label.isString()) {
                            if (const auto res = toLabel(label.asString().unwrap()); res.has_value()) {
                                config.firstRow.push_back(res.value());
                            }
                        }
                    }
                    for (const auto& label : array.back()) {
                        if (label.isString()) {
                            if (const auto res = toLabel(label.asString().unwrap()); res.has_value()) {
                                config.secondRow.push_back(res.value());
                            }
                        }
                    }
                }
                else if (array.front().isString()) {
                    for (const auto& label : array) {
                        if (label.isString()) {
                            if (const auto res = toLabel(label.asString().unwrap()); res.has_value()) {
                                config.firstRow.push_back(res.value());
                            }
                        }
                    }
                }
                else {
                    continue;
                }

                pLabelOptions.addConfig(id, std::move(config));
            }
        }
    }
}