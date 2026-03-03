#pragma once

#pragma once

#include "setting-button.hpp"

namespace Settings {
    class SettingsPopup final : public geode::Popup {
    private:
        std::vector<cocos2d::CCNode*> m_pages;
        std::vector<SettingButtonBase*> m_settings;
        cocos2d::CCMenu* m_pageDotMenu = nullptr;
        CCMenuItemSpriteExtra* m_nextPageButton = nullptr;
        CCMenuItemSpriteExtra* m_previousPageButton = nullptr;

        int m_currentPage = 0;

        static constexpr size_t SETTINGS_PER_PAGE = 12;
        
        static constexpr float SETTING_BUTTON_GAP = 5.0f;
        static constexpr float DOT_MENU_HEIGHT = 15.0f;
        static constexpr float DOT_MENU_GAP = 5.0f;
        static constexpr float LOGO_SIZE = 37.5f;
        static constexpr float ARROW_SIZE = 40.0f;
        static constexpr float ARROW_DISTANCE = 25.0f;
        static constexpr float KEYBINDS_BUTTON_SIZE = 150.0f;

        static constexpr float PADDING = 5.0f;
        static constexpr float EDGE_PADDING = 12.5f;
        
        static constexpr float WIDTH = SettingButtonBase::SETTING_BUTTON_SIZE.width * 3 + SETTING_BUTTON_GAP * 2 + EDGE_PADDING * 2;
        static constexpr float HEIGHT = (WIDTH - EDGE_PADDING * 2) * (7.0f / 10.0f);

        bool init();

        cocos2d::CCMenu* createPage(const Category& pCategory);
        
        void goToPage(int pPage);

        void setupKeybindsMenu(cocos2d::CCNode* pContainer);

        void onPageDot(cocos2d::CCObject* pSender);
        void onNextPage(cocos2d::CCObject* pSender);
        void onPreviousPage(cocos2d::CCObject* pSender);

    public:
        static SettingsPopup* create();
    };
}