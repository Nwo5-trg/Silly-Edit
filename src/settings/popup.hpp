#pragma once

#include <alphalaneous.alphas-ui-pack/include/API.hpp>
#include "setting-button.hpp"

namespace nwo5::ui {
    class Tooltip;
}

namespace Settings {
    class SettingsPopup final : public geode::Popup {
    protected:
        struct Theme {
            cocos2d::ccColor3B background;
            cocos2d::ccColor3B sideBar;
            cocos2d::ccColor3B topBar;
        };

        alpha::ui::AdvancedScrollLayer* m_featuresScroll = nullptr;
        alpha::ui::AdvancedScrollLayer* m_settingsScroll = nullptr;

        cocos2d::CCLabelBMFont* m_versionLabel = nullptr;
        CCMenuItemSpriteExtra* m_keybindsButton = nullptr;

        nwo5::ui::Tooltip* m_tooltip = nullptr;

        std::unordered_map<int, std::pair<std::vector<SettingButtonBase*>, nwo5::settings::Category*>> m_settingsMap;
        std::unordered_map<int, CCMenuItemSpriteExtra*> m_featureButtons;

        std::unordered_map<std::string, SettingReload> m_reloadSettingsActivated;

        int m_selectedFeature = 0;

        static constexpr cocos2d::CCSize SIZE = {415.0f, 225.0f};
        static constexpr float EDGE_PADDING = 5.0f;

        static constexpr float CLOSE_BUTTON_SIZE = 50.0f;

        static constexpr float SETTING_BUTTON_GAP = 5.0f;

        static constexpr float TOP_BAR_HEIGHT = 20.0f;
        static constexpr float TOP_BAR_SHADOW_HEIGHT = 5.0f;
        static constexpr float TOP_BAR_BUTTON_SIZE = 10.0f;
        static constexpr float TOP_BAR_BUTTON_GAP = 5.0f;
        static constexpr float VERSION_LABEL_HEIGHT = 10.0f;

        static constexpr float SIDE_BAR_WIDTH = 125.0f;
        static constexpr float TITLE_LABEL_HEIGHT = 10.0f;
        static constexpr float FEATURE_LABEL_HEIGHT = 7.5f;
        static constexpr float FEATURE_LABEL_ICON_GAP = 2.5f;
        static constexpr float SIDE_BAR_LABEL_GAP = 2.5f;
        static constexpr float FEATURE_SCROLLBAR_WIDTH = 10.0f;

        bool init();
        void update(float);

        void goToFeature(int pTag);

        void onFeatureButton(cocos2d::CCObject* pSender);
        void onCredits(cocos2d::CCObject*);
        void onOpenSaveDir(cocos2d::CCObject*);
        void onOpenConfigDir(cocos2d::CCObject*);
        void onKeybinds(cocos2d::CCObject*);

    public:
        void onClose(cocos2d::CCObject* pSender);

        static SettingsPopup* create();
    };
}