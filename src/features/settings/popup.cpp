#include <internal/utils/utils.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/Button.hpp>
#include "popup.hpp"
#include "setting-button.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

namespace Settings {
    bool SettingsPopup::init() {
        if (!Popup::init(WIDTH, HEIGHT)) {
            return false;
        }

        m_pageDotMenu = ui::node(Setup(ui::menu(ui::row(AxisAlignment::Center, DOT_MENU_GAP)
            .autoScale()
        ))
            .id("dot-menu"_spr)
            .size(WIDTH - EDGE_PADDING * 2, DOT_MENU_HEIGHT)
            .pos(WIDTH / 2, EDGE_PADDING + DOT_MENU_HEIGHT / 2)
            .parent(m_mainLayer)
        );

        for (auto& category : SettingsManager::get()->getCategories()) {
            const auto size = category->getSettings().size();

            auto menu = createPage(category);

            for (size_t i = 0; i < size; i++) {
                auto button = createSettingButton(category->getSettings()[i], this);
                    
                menu->addChild(button);
                m_settings.push_back(button);

                menu->updateLayout();
            }
        }

        Setup(ui::buttonFrame(
            "GJ_arrow_01_001.png", this, menu_selector(SettingsPopup::onNextPage)
        ))
            .id("next-page-button"_spr)
            .scaleToFit(ARROW_SIZE)
            .pos(WIDTH + ARROW_DISTANCE, HEIGHT / 2)
            .visible(Settings::General::showPageArrows.get())
            .parent(m_buttonMenu)
            .flipX();
        Setup(ui::buttonFrame(
            "GJ_arrow_01_001.png", this, menu_selector(SettingsPopup::onPreviousPage)
        ))
            .id("previous-page-button"_spr)
            .scaleToFit(ARROW_SIZE)
            .pos(-ARROW_DISTANCE, HEIGHT / 2)
            .visible(Settings::General::showPageArrows.get())
            .parent(m_buttonMenu);

        m_closeBtn->setPosition(WIDTH, HEIGHT);

        goToPage(Settings::General::saveSettingPage.get() ? Mod::get()->getSavedValue<int>("general-settings-page") : 0);

        return true;
    }

    CCMenu* SettingsPopup::createPage(Category* pCategory) {  
        const auto page = m_pages.size();

        auto pageContainer = ui::node(Setup(CCNode::create())
            .id("page-container-{}"_spr, page)
            .pos(WIDTH / 2, HEIGHT / 2)
            .parent(m_mainLayer)
        );

        m_pages.push_back(pageContainer);
        
        Setup(ui::buttonSprite(
            Settings::General::useLogosForDots.get() ? pCategory->logo() : "smallDot.png", 
            this, menu_selector(SettingsPopup::onPageDot)
        ))
            .id("page-{}-dot"_spr, page)
            .scaleToFit(DOT_MENU_HEIGHT)
            .tag(page)
            .parent(m_pageDotMenu);

        auto menu = ui::node(Setup(ui::menu(ui::row(
            AxisAlignment::Start, SETTING_BUTTON_GAP, AxisAlignment::End
        )
            .autoScale()
            .crossOverflow(false)
        ))
            .id("settings_menu"_spr)
            .size(
                SettingButtonBase::SETTING_BUTTON_SIZE.width * 3 + SETTING_BUTTON_GAP * 2, 
                SettingButtonBase::SETTING_BUTTON_SIZE.height * 4 + SETTING_BUTTON_GAP * 2
            )
            .pos(0.0f, -HEIGHT / 2 + EDGE_PADDING + DOT_MENU_HEIGHT + PADDING + SettingButtonBase::SETTING_BUTTON_SIZE.height * 2 + SETTING_BUTTON_GAP)
            .parent(pageContainer)
        );

        auto logo = ui::node(Setup(CCSprite::create(pCategory->logo().c_str()))
            .id("logo"_spr)
            .pos(-WIDTH / 2 + EDGE_PADDING + LOGO_SIZE / 2, HEIGHT / 2 - EDGE_PADDING - LOGO_SIZE / 2)
            .scaleToFit(LOGO_SIZE)
            .parent(pageContainer)
        );

        Setup(ui::label(pCategory->name()))
            .id("category-label"_spr)
            .anchor(LEFT_CENTER_ANCHOR)
            .scale(0.5f)
            .pos(logo->getPositionX() + LOGO_SIZE / 2 + PADDING, logo->getPositionY())
            .parent(pageContainer);

        if (pCategory->name() == "Keybinds") {
            Setup(Button::createWithNode(ButtonSprite::create("Open Keybinds"), [this] (Button*) {
                geode::openSettingsPopup(Mod::get(), true);
            }))
                .id("keybinds_button"_spr)
                .scaleToFit(KEYBINDS_BUTTON_SIZE)
                .pos(CCPointZero)
                .parent(pageContainer);
        }

        return menu;
    }

    void SettingsPopup::goToPage(int pPage) {
        m_currentPage = pPage < 0 ? m_pages.size() - 1 : pPage % m_pages.size();

        for (auto page : m_pages) {
            page->setVisible(false);
        }

        m_pages[m_currentPage]->setVisible(true);

        for (auto dot : CCArrayExt<CCMenuItemSpriteExtra*>(m_pageDotMenu->getChildren())) {
            dot->setColor(ccGRAY);
        }

        m_pageDotMenu->getChildByType<CCMenuItemSpriteExtra*>(m_currentPage)->setColor(ccWHITE);

        Mod::get()->setSavedValue<int>("general-settings-page", m_currentPage);
    }

    void SettingsPopup::onPageDot(CCObject* pSender) {
        goToPage(pSender->getTag());
    }
    void SettingsPopup::onNextPage(CCObject* pSender) {
        goToPage(m_currentPage + 1);
    }
    void SettingsPopup::onPreviousPage(CCObject* pSender) {
        goToPage(m_currentPage - 1);
    }

    void SettingsPopup::onClose(CCObject* pSender) {
        if (!Settings::General::showReloadWarnings.get() || m_reloadSettingsActivated.empty()) {
            return Popup::onClose(pSender);
        }

        std::string str;

        if (const auto count = std::ranges::count(m_reloadSettingsActivated, SettingReload::Editor)) {
            str.append(fmt::format("{} settings that require editor reload, ", count));
        }
        if (const auto count = std::ranges::count(m_reloadSettingsActivated, SettingReload::Game)) {
            str.append(fmt::format("{} settings that require game reload, ", count));
        }
        if (const auto count = std::ranges::count(m_reloadSettingsActivated, SettingReload::Pause)) {
            str.append(fmt::format("{} settings that require editor pause menu reload, ", count));
        }
        if (const auto count = std::ranges::count(m_reloadSettingsActivated, SettingReload::Popup)) {
            str.append(fmt::format("{} settings that require settings popup reload, ", count));
        }

        str.pop_back();
        str.pop_back();

        if (const auto i = str.find_last_of(','); i != std::string::npos) {
            str.replace(str.find_last_of(','), 1, ", and");
        }
        
        str.append(" have been changed");
        
        Popup::onClose(pSender);
        
        FLAlertLayer::create(
            "BTW",
            str,
            "OK"
        )->show();
    }

    SettingsPopup* SettingsPopup::create() {
        auto ret = new SettingsPopup;

        if (!ret->init()) {
            delete ret;
            
            return nullptr;
        }

        ret->autorelease();

        return ret;
    }
    void SettingsPopup::settingChanged(GenericSetting* pSetting, SettingReload pReload) {
        if (pReload != SettingReload::None && !m_settingsChanged.contains(pSetting)) {
            m_reloadSettingsActivated.push_back(pReload);
        }

        m_settingsChanged.insert(pSetting);
    }
}