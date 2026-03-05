#include <internal/utils/utils.hpp>
#include<Geode/ui/GeodeUI.hpp>
#include <Geode/ui/Button.hpp>
#include "popup.hpp"
#include "settings.hpp"
#include "setting-button.hpp"

using namespace geode::prelude;
using namespace Utils::Aliases;

namespace Settings {
    bool SettingsPopup::init() {
        if (!Popup::init(WIDTH, HEIGHT)) {
            return false;
        }

        m_pageDotMenu = Utils::setupNode(
            CCMenu::create(),

            SetNodeID{"dot-menu"_spr},
            SetNodeSize{WIDTH - EDGE_PADDING * 2, DOT_MENU_HEIGHT},
            SetNodePosition{WIDTH / 2, EDGE_PADDING + DOT_MENU_HEIGHT / 2},
            SetNodeParent{m_mainLayer}
        );

        for (auto& category : SettingsManager::get()->getCategories()) {
            const auto size = category.getSettings().size();

            auto menu = createPage(category);

            for (int i = 0; i < size; i += SETTINGS_PER_PAGE) {
                for (int j = 0; j < SETTINGS_PER_PAGE && (i + j) < size; j++) {
                    auto button = createSettingButton(category.getSettings()[i + j]);
                    
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

        auto next = Utils::setupNode(
            Utils::createButtonFrame("GJ_arrow_01_001.png", this, menu_selector(SettingsPopup::onNextPage)),

            SetNodeID{"next-page-button"_spr},
            SetNodeScaleWithSize{ARROW_SIZE},
            SetNodePosition{WIDTH + ARROW_DISTANCE, HEIGHT / 2},
            SetNodeVisibility{Settings::General::showPageArrows.get()},
            SetNodeParent{m_buttonMenu}
        );

        next->setRotationY(180.0f);

        auto prev = Utils::setupNode(
            Utils::createButtonFrame("GJ_arrow_01_001.png", this, menu_selector(SettingsPopup::onPreviousPage)),

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

    CCMenu* SettingsPopup::createPage(const Category& pCategory) {  
        const auto page = m_pages.size();

        auto pageContainer = Utils::setupNode(
            CCNode::create(),

            SetNodeID{"page-container-{}"_spr, page},
            SetNodePosition{WIDTH / 2, HEIGHT / 2},
            SetNodeParent{m_mainLayer}
        );

        m_pages.push_back(pageContainer);
        
        Utils::setupNode(
            Utils::createButton(
                Settings::General::useLogosForDots.get() ? pCategory.getLogoPath() : "smallDot.png", 
                this, menu_selector(SettingsPopup::onPageDot)
            ),

            SetNodeID{"page-{}-dot"_spr, page},
            SetNodeScaleWithSize{DOT_MENU_HEIGHT},
            SetNodeTag{static_cast<int>(page)},
            SetNodeParent{m_pageDotMenu}
        );

        auto menu = Utils::setupNode(
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

        auto logo = Utils::setupNode(
            CCSprite::create(pCategory.getLogoPath().c_str()),

            SetNodeID{"logo"_spr},
            SetNodePosition{-WIDTH / 2 + EDGE_PADDING + LOGO_SIZE / 2, HEIGHT / 2 - EDGE_PADDING - LOGO_SIZE / 2},
            SetNodeScaleWithSize{LOGO_SIZE},
            SetNodeParent{pageContainer}
        );

        auto label = CCLabelBMFont::create(pCategory.getName().c_str(), "bigFont.fnt");

        Utils::setupNode(
            label,

            SetNodeID{"category-label"_spr},
            SetNodeAnchor{LEFT_CENTER_ANCHOR},
            SetNodeScale{0.5f},
            SetNodePosition{logo->getPositionX() + LOGO_SIZE / 2 + PADDING, logo->getPositionY()},
            SetNodeParent{pageContainer}
        );

        if (pCategory.getName() == "Keybinds") {
            setupKeybindsMenu(pageContainer);
        }

        return menu;
    }

    void SettingsPopup::goToPage(int pPage) {
        m_currentPage = pPage % m_pages.size();

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
        Utils::setupNode(
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

    SettingsPopup* SettingsPopup::create() {
        auto ret = new SettingsPopup;

        if (!ret->init()) {
            delete ret;
            return nullptr;
        }

        ret->autorelease();

        return ret;
    }
}