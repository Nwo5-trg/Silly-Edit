#pragma once

#include <alphalaneous.alphas-ui-pack/include/API.hpp>
#include "setting-button.hpp"

namespace nwo5::ui {
    class Tooltip;
}

namespace sillyedit::settings {
    class SettingsPopup final : public geode::Popup {
    protected:
        struct Theme {
            cocos2d::ccColor3B background;
            cocos2d::ccColor3B sideBar;
            cocos2d::ccColor3B topBar;
        };

        struct CategoryInfo {
            nwo5::settings::Category* category = nullptr;
            CCMenuItemSpriteExtra* featureButton = nullptr;
            std::vector<SettingButtonBase*> buttons;
        };

        alpha::ui::AdvancedScrollLayer* m_featuresScroll = nullptr;
        alpha::ui::AdvancedScrollLayer* m_settingsScroll = nullptr;

        geode::Label* m_versionLabel = nullptr;
        CCMenuItemSpriteExtra* m_keybindsButton = nullptr;

        nwo5::ui::Tooltip* m_tooltip = nullptr;

        std::unordered_map<int, CategoryInfo> m_settingsMap;

        std::unordered_map<std::string, SettingReload> m_reloadSettingsActivated;

        int m_selectedFeature = 0;

        static constexpr cocos2d::CCSize SIZE = {415.0f, 225.0f};
        static constexpr float PADDING = 5.0f;

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
        void onResetAll(cocos2d::CCObject*);
        void onKeybinds(cocos2d::CCObject*);

    public:
        void toggleSettingsDrag(bool pEnable);

        void onClose(cocos2d::CCObject* pSender);

        static SettingsPopup* create();

    protected:
        static constexpr auto CREDITS_STRING =
R"(tyyyy <cr>\<3</c> !

## Special Thanks
### Alpha
- made tinker
- replace obj impl
- setting popup inspo
- some general help with stuff

### Ery
- geode gremlin
- pr for obj tab icons
- permission to use them as assets
- prolly accepting this mod

### HJFod
- made better edit
- let me steal a bunch of stuff
- let me have a bunch of other stuff

## Credits

### gdjayy
- replace object suggestion

### CreatorCreepy
- feedback for replace object
- feedback for floodfill

### CarlIsBored
- trigger id search suggestion

### like all the hosts of cornbread megacollab
- better select all suggestion

### Doranell
- text obj utils suggestion

### DasshuDev
- copy particle string idea
)";
    };
}