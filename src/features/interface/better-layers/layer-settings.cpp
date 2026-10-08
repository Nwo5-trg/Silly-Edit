#include <cvolton.level-id-api/include/EditorIDs.hpp>
#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;

static std::filesystem::path getLayerSettingsPath() {
    static const auto dir = Mod::get()->getSaveDir() / "layer-settings.json";

    static const auto cleanupSaveDir = [&] {
        auto json = file::readJson(getLayerSettingsPath()).unwrapOrDefault();
        matjson::Value out;

        for (const auto& obj : json) {
            if (const auto key = obj.getKey(); key.has_value() && obj.size()) {
                out[key.value()] = obj;
            }
        }

        return file::writeToJson(dir, out);
    };

    return dir;
}

namespace BetterLayers {
    LayerSettings::LayerSettings() {
        importSettings();

        sillyedit::shared::getLayerSettingsPtr() = this;
    };
    LayerSettings::~LayerSettings() {
        exportSettings();

        sillyedit::shared::getLayerSettingsPtr() = nullptr;
    }

    void LayerSettings::importSettings() {
        m_id = EditorIDs::getID(editor::layer()->m_level);
        
        const auto json = file::readJson(getLayerSettingsPath()).unwrapOrDefault();

        const auto& levelRes = json.get(misc::numToString(m_id));

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

        auto& levelObj = json[misc::numToString(m_id)];
        levelObj.clear();

        if (m_focusedLayer.has_value()) {
            levelObj["focused"] = m_focusedLayer.value();
        }
        if (m_defaultOpacity.has_value()) {
            levelObj["opacity"] = m_defaultOpacity.value();
        } 

        for (size_t i = 0; i < m_layerMap.size(); i++) {
            const auto& state = m_layerMap[i];

            if (!state.hidden && !state.opacity.has_value()) {
                continue;
            }

            auto& layerObj = levelObj[misc::numToString(i)];

            if (state.hidden) {
                layerObj["hidden"] = state.hidden;
            }
            if (state.opacity.has_value()) {
                layerObj["opacity"] = state.opacity.value();
            }
        }

        (void)file::writeToJson(getLayerSettingsPath(), json);
    }
    
    bool LayerSettings::layerHasState(short pLayer) const {
        return m_layerMap[pLayer].hidden || m_layerMap[pLayer].opacity.has_value();
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
        m_layerMap[pLayer].hidden = pHidden;
    }
    bool LayerSettings::isLayerHidden(int pLayer) const {
        return m_layerMap[pLayer].hidden;
    }
    void LayerSettings::setLayerOpacity(int pLayer, unsigned char pOpacity) {
        m_layerMap[pLayer].opacity = pOpacity;
    }
    void LayerSettings::unsetLayerOpacity(int pLayer) {
        m_layerMap[pLayer].opacity = std::nullopt;
    }
    std::optional<unsigned char> LayerSettings::getLayerOpacity(int pLayer) const {
        return m_layerMap[pLayer].opacity;
    }

    unsigned char LayerSettings::opacityForObject(unsigned char pUnmodifiedOpacity, GameObject* pObj) {
        if (!pUnmodifiedOpacity) {
            return pUnmodifiedOpacity;
        }

        const auto layer = pObj->m_editorLayer;
        const auto layer2 = pObj->m_editorLayer2;

        if (this->isLayerHidden(layer) || this->isLayerHidden(layer2)) {
            return 0;
        }
        
        float opacity = static_cast<float>(pUnmodifiedOpacity);
        
        const auto canSelectLayer = sillyedit::utils::canSelectLayerFast(pObj, editor::layer()->m_currentLayer);

        // undo robtops thing
        if (!canSelectLayer) {
            opacity *= (255.0f / 50.0f);
        }

        std::optional<float> layerOpacity = std::nullopt;

        // checking first cuz (i atleast) use editor layer 2 as a way of grouping objs from multiple layers into 1 thing
        if (const auto layerOpacityRes2 = this->getLayerOpacity(layer2); layer2 && layerOpacityRes2.has_value()) {
            layerOpacity = layerOpacityRes2.value();
        }
        else if (const auto layerOpacityRes = this->getLayerOpacity(layer); layerOpacityRes.has_value()) {
            layerOpacity = layerOpacityRes.value();
        }

        if (layerOpacity.has_value()) {
            opacity = std::clamp(opacity / (255.0f / layerOpacity.value()), 0.0f, 255.0f);
        }

        if (const auto res = this->getFocusedLayer(); res.has_value() && layer != res.value() && (!layer2 || layer2 != res.value())) {
            return sillyedit::utils::modifyOpacity(opacity, BetterLayers::unfocusedLayerOpacity);
        }

        if (!canSelectLayer) {
            return sillyedit::utils::modifyOpacity(opacity, m_defaultOpacity.value_or(BetterLayers::layerOpacity));
        }

        return static_cast<unsigned char>(std::clamp(opacity, 0.0f, 255.0f));
    }
}