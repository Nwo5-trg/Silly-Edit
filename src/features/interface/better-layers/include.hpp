#pragma once

#include <Geode/modify/EditorUI.hpp>
#include <feature/include.hpp>
#include "utils.hpp"
#include "shared.hpp"

namespace BetterLayers {
    class $feature(BetterLayers) {
        void onEditor() override;
        void onUpdate() override;
        void onToggled(bool pEnabled) override;
        void onUIUpdated(float pScale) override;
        void onSettingChanged(std::string pName, GenericSetting*) override;
    } feature;

    static constexpr float GAP = 5.0f;
    static constexpr float INPUT_GAP = 10.0f;
    static constexpr cocos2d::CCSize LAYER_INPUT_SIZE = {35.0f, 20.0f};
    static constexpr float LAYER_SHIFT_BUTTON_SIZE = 25.0f;
    static constexpr float LAYER_EXTRA_BUTTON_SIZE = 20.0f;
    static constexpr float LAYER_LOCK_SIZE = 15.0f;

    class $feature_modify(EditorUI) {
        struct Fields {
            cocos2d::CCMenu* newLayerMenu = nullptr;
            geode::TextInput* layerInput = nullptr;
            CCMenuItemSpriteExtra* nextFreeButton = nullptr;
            CCMenuItemSpriteExtra* allLayersButton = nullptr;
            CCMenuItemSpriteExtra* layerSettingsButton = nullptr;
            CCMenuItemToggler* lockLayerButton = nullptr;
            
            std::unique_ptr<BetterLayers::LayerSettings> settings = nullptr;

            bool canDestroyUndo = false;
            bool updateLockButton = true;

            ~Fields();
        };

        void updateLayerMenu();

        void onNextFreeLayer(cocos2d::CCObject* sender);
        void onLayerSettings(cocos2d::CCObject* sender);
        void onToggleLayerLocked(cocos2d::CCObject* sender);
        void updateGroupIDLabel();
        void createUndoSelectObject(bool redo);
        bool canSelectObject(GameObject* object);
        void selectObjects(cocos2d::CCArray* objects, bool ignoreFilter);
    };

    class $setting_category("better-layers-logo.png"_spr, "Layer input and opacity settings and stuff");

    inline SillySetting<bool> nextFreeButton{
        "Next Free Button", feature, true
    };
    inline SillySetting<bool> lockButton{
        "Lock Button", feature, true,
    };
    inline SillySetting<bool> layerSettingsButton{
        "Layer Settings Button", feature, true, "disable if u dont plan on using hidden layers and stuff"
    };
    inline SillySetting<bool> unselectableHiddenLayers{
        "Unselectable Hidden Layers", feature, true
    };
    inline SillySetting<int> layerOpacity{
        "Layer Opacity", feature, 50, {0, 255}, "from 0-255"
    };
    inline SillySetting<int> unfocusedLayerOpacity{
        "Unfocused Layer Opacity", feature, 25, {0, 255}, "from 0-255"
    };
    inline SillySetting<bool> unselectableUnfocusedLayers{
        "Unselectable Unfocused Layers", feature, false
    };
}