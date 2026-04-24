#pragma once

namespace BetterLayers {
    struct LayerState final {
        std::optional<unsigned char> opacity;
        bool hidden = false;
    };

    class LayerSettings final{
    private:
        int m_id = 0;

        std::optional<short> m_focusedLayer;
        std::optional<unsigned char> m_defaultOpacity;
        std::unordered_map<short, LayerState> m_layerMap;

        void importSettings();

    public:
        LayerSettings() {
            importSettings();
        };
        ~LayerSettings() {
            exportSettings();
        }

        void exportSettings();

        static unsigned char globalLayerOpacity();
        static unsigned char unfocusedLayerOpacity();

        bool layerHasState(short pLayer) const;

        void setFocusedLayer(short pLayer);
        void unsetFocusedLayer();
        std::optional<short> getFocusedLayer() const;

        void setDefaultOpacity(unsigned char pOpacity);
        void unsetDefaultOpacity();
        std::optional<unsigned char> getDefaultOpacity();

        void setLayerHidden(int pLayer, bool pHidden);
        bool isLayerHidden(int pLayer) const;
        void setLayerOpacity(int pLayer, unsigned char pOpacity);
        void unsetLayerOpacity(int pLayer);
        std::optional<unsigned char> getLayerOpacity(int pLayer) const;

        unsigned char opacityForObject(unsigned char pUnmodifiedOpacity, GameObject* pObj);
    };

    class EditLayerPopup final : public geode::Popup {
    private:
        short m_layer;
        LayerSettings* m_settings = nullptr;;
        geode::TextInput* m_opacityInput = nullptr;

        static constexpr float INPUT_SIZE = 60.0f;
        static constexpr float LABEL_HEIGHT = 25.0f;
        static constexpr float BUTTON_SIZE = 30.0f;

        static constexpr float PADDING = 15.0f;

        static constexpr float WIDTH = 325.0f;
        static constexpr float HEIGHT = 60.0f;

        bool init(LayerSettings* pSettings);

        void onToggleHidden(cocos2d::CCObject*);
        void onToggleFocused(cocos2d::CCObject*);

    public:
        static EditLayerPopup* create(LayerSettings* pSettings);
    };

    class EditAllLayersPopup final : public geode::Popup {
    private:
        LayerSettings* m_settings = nullptr;
        geode::TextInput* m_defaultOpacityInput = nullptr;
        geode::TextInput* m_focusedLayerInput = nullptr;

        static constexpr float INPUT_SIZE = 50.0f;
        static constexpr float LABEL_HEIGHT = 25.0f;
        static constexpr float BUTTON_SIZE = 25.0f;

        static constexpr float GAP = 15.0f;

        static constexpr float WIDTH = 350.0f;
        static constexpr float HEIGHT = 200.0f;

        bool init(LayerSettings* pSettings);

        void onUnfocusLayer(cocos2d::CCObject*);

    public:
        static EditAllLayersPopup* create(LayerSettings* pSettings);
    };
}