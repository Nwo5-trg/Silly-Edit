#include "settings.hpp"
#include "include.hpp"
#include <internal/utils/utils.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <cvolton.level-id-api/include/EditorIDs.hpp>

using namespace geode::prelude;

namespace BetterLayers {
    static std::filesystem::path getLayerSettingsPath() {
        return Mod::get()->getSaveDir() / "layer-settings.json";
    }

    void LayerSettings::importSettings() {
        m_id = EditorIDs::getID(editor::layer()->m_level);
        
        const auto json = file::readJson(getLayerSettingsPath()).unwrapOrDefault();

        const auto& levelRes = json.get(nwo5::utils::numToString(m_id));

        if (levelRes.isErr()) {
            return;
        }

        const auto& levelObj = levelRes.unwrap();

        if (auto res = levelObj.get("focused"); res.isOk() && res.unwrap().isExactlyUInt()) {
            m_focusedLayer = res.unwrap().asUInt().unwrap();
        }
        if (auto res = levelObj.get("opacity"); res.isOk() && res.unwrap().isExactlyUInt()) {
            m_defaultOpacity = res.unwrap().asUInt().unwrap();
        }

        for (const auto& layerObj : levelObj) {
            if (!layerObj.isObject() || !layerObj.getKey().has_value()) {
                continue;
            }

            const auto layer = utils::numFromString<short>(layerObj.getKey().value()).unwrapOrDefault();

            if (const auto& res = layerObj.get("hidden"); res.isOk() && res.unwrap().isBool()) {
                m_layerMap[layer].hidden = res.unwrap().asBool().unwrap();
            }
            if (const auto& res = layerObj.get("opacity"); res.isOk() && res.unwrap().isExactlyUInt()) {
                m_layerMap[layer].opacity = res.unwrap().asUInt().unwrap();
            }
        }
    }

    void LayerSettings::exportSettings() {
        auto json = file::readJson(getLayerSettingsPath()).unwrapOrDefault();

        auto& levelObj = json[nwo5::utils::numToString(m_id)];
        levelObj.clear();

        if (m_focusedLayer.has_value()) {
            levelObj["focused"] = m_focusedLayer.value();
        }
        if (m_defaultOpacity.has_value()) {
            levelObj["opacity"] = m_defaultOpacity.value();
        } 

        for (const auto [layer, state] : m_layerMap) {
            if (!state.hidden && !state.opacity.has_value()) {
                continue;
            }

            auto& layerObj = levelObj[nwo5::utils::numToString(layer)];

            if (state.hidden) {
                layerObj["hidden"] = state.hidden;
            }
            if (state.opacity.has_value()) {
                layerObj["opacity"] = state.opacity.value();
            }
        }

        (void)file::writeToJson(getLayerSettingsPath(), json);
    }

    unsigned char LayerSettings::globalLayerOpacity() {
        return static_cast<unsigned char>(std::clamp(Settings::BetterLayers::layerOpacity.get(), 0, 255));
    }
    unsigned char LayerSettings::unfocusedLayerOpacity() {
        return static_cast<unsigned char>(std::clamp(Settings::BetterLayers::unfocusedLayerOpacity.get(), 0, 255));
    }

    bool LayerSettings::layerHasState(short pLayer) const {
        return m_layerMap.find(pLayer) != m_layerMap.end();
    }

    void LayerSettings::setFocusedLayer(short pLayer) {
        m_focusedLayer = pLayer;
    }
    void LayerSettings::unsetFocusedLayer() {
        m_focusedLayer = std::nullopt;
    }
    std::optional<short> LayerSettings::getFocusedLayer() const {
        return m_focusedLayer;
    }

    void LayerSettings::setDefaultOpacity(unsigned char pOpacity) {
        m_defaultOpacity = pOpacity;
    }
    void LayerSettings::unsetDefaultOpacity() {
        m_defaultOpacity = std::nullopt;
    }
    std::optional<unsigned char> LayerSettings::getDefaultOpacity() {
        return m_defaultOpacity;
    }

    void LayerSettings::setLayerHidden(int pLayer, bool pHidden) {
        m_layerMap[pLayer].hidden = true;
    }
    bool LayerSettings::isLayerHidden(int pLayer) const {
        const auto it = m_layerMap.find(pLayer);

        return it == m_layerMap.end() ? false : it->second.hidden;
    }
    void LayerSettings::setLayerOpacity(int pLayer, unsigned char pOpacity) {
        m_layerMap[pLayer].opacity = pOpacity;
    }
    void LayerSettings::unsetLayerOpacity(int pLayer) {
        m_layerMap[pLayer].opacity = std::nullopt;
    }
    std::optional<unsigned char> LayerSettings::getLayerOpacity(int pLayer) const {
        const auto it = m_layerMap.find(pLayer);

        return it == m_layerMap.end() ? std::nullopt : it->second.opacity;
    }

    unsigned char LayerSettings::opacityForObject(unsigned char pUnmodifiedOpacity, GameObject* pObj) {
        if (!pUnmodifiedOpacity) {
            return pUnmodifiedOpacity;
        }

        if (isLayerHidden(pObj->m_editorLayer) || isLayerHidden(pObj->m_editorLayer2)) {
            return 0;
        }
        
        float opacity = static_cast<float>(pUnmodifiedOpacity);
        
        const auto canSelectLayer = editor::object::canSelectLayer(pObj, true);

        // undo robtops thing
        if (!canSelectLayer) {
            opacity *= (255.0f / 50.0f);
        }

        std::optional<float> layerOpacity = std::nullopt;

        // checking first cuz (i atleast) use editor layer 2 as a way of grouping objs from multiple layers into 1 thing
        if (const auto layerOpacityRes2 = getLayerOpacity(pObj->m_editorLayer2); pObj->m_editorLayer2 && layerOpacityRes2.has_value()) {
            layerOpacity = layerOpacityRes2.value();
        }
        else if (const auto layerOpacityRes = getLayerOpacity(pObj->m_editorLayer); layerOpacityRes.has_value()) {
            layerOpacity = layerOpacityRes.value();
        }

        if (layerOpacity.has_value()) {
            opacity = std::clamp(opacity / (255.0f / layerOpacity.value()), 0.0f, 255.0f);
        }

        if (const auto res = getFocusedLayer(); res.has_value() && pObj->m_editorLayer != res.value() && pObj->m_editorLayer2 != res.value()) {
            if (const auto unfocusedOpacity = unfocusedLayerOpacity()) {
                return static_cast<unsigned char>(std::clamp(opacity / (255.0f / unfocusedOpacity), 0.0f, 255.0f));
            }
            else {
                return 0;
            }
        }

        if (!canSelectLayer) {
            if (m_defaultOpacity.has_value()) {
                if (m_defaultOpacity.value()) {
                    return static_cast<unsigned char>(std::clamp(opacity / (255.0f / m_defaultOpacity.value()), 0.0f, 255.0f));
                }
                else {
                    return 0;
                }
            }
            else {
                if (const auto globalOpacity = globalLayerOpacity()) {
                    return static_cast<unsigned char>(std::clamp(opacity / (255.0f / globalOpacity), 0.0f, 255.0f));
                }
                else {
                    return 0;
                }
            }
        }

        return static_cast<unsigned char>(std::clamp(opacity, 0.0f, 255.0f));
    }
}