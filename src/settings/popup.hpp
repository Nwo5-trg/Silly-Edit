#pragma once

#include <alphalaneous.alphas-ui-pack/include/API.hpp>
#include <feature/include.hpp>
#include "setting-button.hpp"

namespace nwo5::ui {
    class Tooltip;
}

namespace sillyedit::settings {
    class SettingsPopup final : public geode::Popup {
    protected:
        struct Theme {
            cocos2d::ccColor4B background;
            cocos2d::ccColor4B sideBar;
            cocos2d::ccColor4B topBar;
            cocos2d::ccColor4B topShadow;
            cocos2d::ccColor4B floatingMenuBackground;

            cocos2d::ccColor3B textColor;
            cocos2d::ccColor3B unmatchedSettingColor;
            float unmatchedSelectedFeatureDarkness;
        } m_theme;

        cocos2d::CCSprite* m_background = nullptr;
        cocos2d::CCLayerColor* m_sideBar = nullptr;
        cocos2d::CCLayerColor* m_topBar = nullptr;
        cocos2d::CCSprite* m_topShadow = nullptr;
        cocos2d::CCLayerColor* m_floatingMenuBackground = nullptr;

        alpha::ui::AdvancedScrollLayer* m_featuresScroll = nullptr;
        alpha::ui::AdvancedScrollLayer* m_settingsScroll = nullptr;

        geode::Label* m_versionLabel = nullptr;
        CCMenuItemSpriteExtra* m_keybindsButton = nullptr;

        CCMenuItemSpriteExtra* m_incompatWarningButton = nullptr;

        nwo5::ui::Tooltip* m_tooltip = nullptr;

        struct CategoryInfo {
            features::FeatureBase* feature = nullptr;
            nwo5::settings::Category* category = nullptr;
            CCMenuItemSpriteExtra* featureButton = nullptr;
            std::vector<geode::Ref<SettingButtonBase>> buttons;

            bool validOnPlatform() {
                return buttons.size();
            }
        };
        geode::utils::StringMap<CategoryInfo> m_settingsMap;

        geode::utils::StringMap<SettingReload> m_reloadSettingsActivated;

        std::string m_selectedFeature = "General";

        std::string m_currentSearch;
        int m_searchIndex = 0;
        int m_currentSearchMatches = 0;

        geode::Label* m_searchIndexLabel = nullptr;
        geode::TextInput* m_searchInput = nullptr;
        std::vector<cocos2d::CCNode*> m_searchNodes;

        geode::utils::StringSet m_unmatchedFeatures;
        std::unordered_set<SettingButtonBase*> m_unmatchedSettings;

        enum SearchCycleArrow {
            Up,
            Down
        };

        static constexpr cocos2d::CCSize SIZE = {415.0f, 225.0f};
        static constexpr float PADDING = 5.0f;

        static constexpr float FLOATING_MENU_BUTTON_SIZE = 15.0f;
        static constexpr float FLOATING_MENU_GAP = 10.0f;

        static constexpr cocos2d::CCSize SEARCH_INPUT_SIZE{100.0f, FLOATING_MENU_BUTTON_SIZE};
        static constexpr float SEARCH_GAP = 20.0f;
        static constexpr float SEARCH_CYCLE_BUTTON_SIZE = 10.0f;
        static constexpr float SEARCH_CYCLE_BUTTON_GAP = 5.0f;

        static constexpr float SETTING_BUTTON_GAP = 5.0f;

        static constexpr float TOP_BAR_HEIGHT = 20.0f;
        static constexpr float TOP_BAR_SHADOW_HEIGHT = 5.0f;
        static constexpr float TOP_BAR_BUTTON_SIZE = 10.0f;
        static constexpr float INCOMPAT_WARNING_HEIGHT = 12.5f;
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

        void updateCurrentFeature();
        void updateCurrentSearch();

        void onFeatureButton(cocos2d::CCObject* pSender);
        void onIncompatWarning(cocos2d::CCObject*);
        void onOpenSearch(cocos2d::CCObject*);
        void onCycleSearch(cocos2d::CCObject* pSender);
        void onCredits(cocos2d::CCObject*);
        void onOpenSaveDir(cocos2d::CCObject*);
        void onOpenConfigDir(cocos2d::CCObject*);
        void onResetAll(cocos2d::CCObject*);
        void onKeybinds(cocos2d::CCObject*);

        void updateTheme();
        void updateSettingLabels();

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
- reverse engineering most of editor ui
- help with their api
- answering some dumb questions
- some code >:3

### Ery
- geode gremlin
- pr for obj tab icons
- permission to use their pfp as assets
- editor position bar logo
- prolly accepting this mod

### HJFod
- made better edit
- let me steal a bunch of stuff
- let me have a bunch of other stuff

## Credits
### cheeseworks
- testing android

### gdjayy
- replace object suggestion

### CreatorCreepy
- feedback for replace object
- feedback for floodfill

### CarlIsBored
- trigger id search suggestion

### like all the hosts of cornbread megacollab
- better select all suggestion

#### Talindrusk
- answering some dumb questions

### Doranell
- text obj utils suggestion

### DasshuDev
- copy particle string idea
- testing windows/better edit
)";
    };
}