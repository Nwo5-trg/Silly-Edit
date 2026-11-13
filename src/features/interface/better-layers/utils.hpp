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
        std::array<LayerState, std::numeric_limits<short>::max() + 1> m_layerMap;

        void importSettings();

    public:
        LayerSettings();
        ~LayerSettings();

        void exportSettings();

        bool layerHasState(short layer) const;

        void setFocusedLayer(short layer);
        void unsetFocusedLayer();
        std::optional<short> getFocusedLayer() const;

        void setDefaultOpacity(unsigned char opacity);
        void unsetDefaultOpacity();
        std::optional<unsigned char> getDefaultOpacity();

        void setLayerHidden(int layer, bool hidden);
        bool isLayerHidden(int layer) const;
        void setLayerOpacity(int layer, unsigned char opacity);
        void unsetLayerOpacity(int layer);
        std::optional<unsigned char> getLayerOpacity(int layer) const;

        unsigned char opacityForObject(unsigned char unmodifiedOpacity, GameObject* obj);
    };

    class EditLayerPopup final : public geode::Popup {
    private:
        short m_layer;
        LayerSettings* m_settings = nullptr;;
        geode::TextInput* m_opacityInput = nullptr;

        static constexpr float INPUT_SIZE = 60.0f;
        static constexpr float LABEL_HEIGHT = 25.0f;
        static constexpr float BUTTON_SIZE = 30.0f;

        static constexpr float GAP = 15.0f;
        static constexpr float PADDING = 15.0f;

        static constexpr float WIDTH = 375.0f;
        static constexpr float HEIGHT = 120.0f;

        bool init(LayerSettings* settings);

        void onClearOpacity(cocos2d::CCObject*);
        void onToggleHidden(cocos2d::CCObject*);
        void onToggleFocused(cocos2d::CCObject*);

    public:
        static EditLayerPopup* create(LayerSettings* settings);
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

        static constexpr float WIDTH = 375.0f;
        static constexpr float HEIGHT = 200.0f;

        bool init(LayerSettings* settings);

        void onClearDefaultOpacity(cocos2d::CCObject*);
        void onUnfocusLayer(cocos2d::CCObject*);

    public:
        static EditAllLayersPopup* create(LayerSettings* settings);
    };
}