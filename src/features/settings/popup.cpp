#include <internal/utils/utils.hpp>
#include<Geode/ui/GeodeUI.hpp>
#include <Geode/ui/Button.hpp>
#include "popup.hpp"
#include "setting-button.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;
using namespace nwo5::utils::setup;

namespace Settings {
    bool SettingsPopup::init() {
        if (!Popup::init(WIDTH, HEIGHT)) {
            return false;
        }

        m_pageDotMenu = nwo5::utils::setupNode(
            CCMenu::create(),

            SetNodeID{"dot-menu"_spr},
            SetNodeSize{WIDTH - EDGE_PADDING * 2, DOT_MENU_HEIGHT},
            SetNodePosition{WIDTH / 2, EDGE_PADDING + DOT_MENU_HEIGHT / 2},
            SetNodeParent{m_mainLayer}
        );

        for (auto& category : SettingsManager::get()->getCategories()) {
            const auto size = category->getSettings().size();

            auto menu = createPage(category);

            for (int i = 0; i < size; i += SETTINGS_PER_PAGE) {
                for (int j = 0; j < SETTINGS_PER_PAGE && (i + j) < size; j++) {
                    auto button = createSettingButton(category->getSettings()[i + j], this);
                    
                    menu->addChild(button);
                    m_settings.push_back(button);
                }

                menu->updateLayout();
            }
        }

        m_pageDotMenu->setLayout(AxisLayout::create()
            ->setGap(DOT_MENU_GAP)
            ->setGrowCrossAxis(false)
            ->setAutoScale(false)
        );
        m_pageDotMenu->updateLayout();

        auto next = nwo5::utils::setupNode(
            nwo5::utils::createButtonFrame("GJ_arrow_01_001.png", this, menu_selector(SettingsPopup::onNextPage)),

            SetNodeID{"next-page-button"_spr},
            SetNodeScaleWithSize{ARROW_SIZE},
            SetNodePosition{WIDTH + ARROW_DISTANCE, HEIGHT / 2},
            SetNodeVisibility{Settings::General::showPageArrows.get()},
            SetNodeParent{m_buttonMenu}
        );

        next->setRotationY(180.0f);

        auto prev = nwo5::utils::setupNode(
            nwo5::utils::createButtonFrame("GJ_arrow_01_001.png", this, menu_selector(SettingsPopup::onPreviousPage)),

            SetNodeID{"previous-page-button"_spr},
            SetNodeScaleWithSize{ARROW_SIZE},
            SetNodePosition{-ARROW_DISTANCE, HEIGHT / 2},
            SetNodeVisibility{Settings::General::showPageArrows.get()},
            SetNodeParent{m_buttonMenu}
        );

        m_closeBtn->setPosition(WIDTH, HEIGHT);

        goToPage(Settings::General::saveSettingPage.get() ? Mod::get()->getSavedValue<int>("general-settings-page") : 0);

        return true;
    }

    CCMenu* SettingsPopup::createPage(Category* pCategory) {  
        const auto page = m_pages.size();

        auto pageContainer = nwo5::utils::setupNode(
            CCNode::create(),

            SetNodeID{"page-container-{}"_spr, page},
            SetNodePosition{WIDTH / 2, HEIGHT / 2},
            SetNodeParent{m_mainLayer}
        );

        m_pages.push_back(pageContainer);
        
        nwo5::utils::setupNode(
            nwo5::utils::createButton(
                Settings::General::useLogosForDots.get() ? pCategory->logo() : "smallDot.png", 
                this, menu_selector(SettingsPopup::onPageDot)
            ),

            SetNodeID{"page-{}-dot"_spr, page},
            SetNodeScaleWithSize{DOT_MENU_HEIGHT},
            SetNodeTag{static_cast<int>(page)},
            SetNodeParent{m_pageDotMenu}
        );

        auto menu = nwo5::utils::setupNode(
            CCMenu::create(),

            SetNodeID{"settings_menu"_spr},
            SetNodeSize{
                SettingButtonBase::SETTING_BUTTON_SIZE.width * 3 + SETTING_BUTTON_GAP * 2, 
                SettingButtonBase::SETTING_BUTTON_SIZE.height * 4 + SETTING_BUTTON_GAP * 2
            },
            SetNodePosition{0.0f, -HEIGHT / 2 + EDGE_PADDING + DOT_MENU_HEIGHT + PADDING + SettingButtonBase::SETTING_BUTTON_SIZE.height * 2 + SETTING_BUTTON_GAP},
            SetNodeParent{pageContainer}
        );

        menu->setLayout(AxisLayout::create(Axis::Row)
            ->setAxisAlignment(AxisAlignment::Start)
            ->setCrossAxisAlignment(geode::AxisAlignment::End)
            ->setGap(SETTING_BUTTON_GAP)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(false)
        );

        auto logo = nwo5::utils::setupNode(
            CCSprite::create(pCategory->logo().c_str()),

            SetNodeID{"logo"_spr},
            SetNodePosition{-WIDTH / 2 + EDGE_PADDING + LOGO_SIZE / 2, HEIGHT / 2 - EDGE_PADDING - LOGO_SIZE / 2},
            SetNodeScaleWithSize{LOGO_SIZE},
            SetNodeParent{pageContainer}
        );

        auto label = CCLabelBMFont::create(pCategory->name().c_str(), "bigFont.fnt");

        nwo5::utils::setupNode(
            label,

            SetNodeID{"category-label"_spr},
            SetNodeAnchor{LEFT_CENTER_ANCHOR},
            SetNodeScale{0.5f},
            SetNodePosition{logo->getPositionX() + LOGO_SIZE / 2 + PADDING, logo->getPositionY()},
            SetNodeParent{pageContainer}
        );

        if (pCategory->name() == "Keybinds") {
            setupKeybindsMenu(pageContainer);
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

    void SettingsPopup::setupKeybindsMenu(cocos2d::CCNode* pContainer) {
        nwo5::utils::setupNode(
            Button::createWithNode(ButtonSprite::create("Open Keybinds"), [this] (Button*) {
                geode::openSettingsPopup(Mod::get(), true);
            }),

            SetNodeID{"keybinds_button"_spr},
            SetNodeScaleWithSize{KEYBINDS_BUTTON_SIZE},
            SetNodePosition{CCPointZero},
            SetNodeParent{pContainer}
        );
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