#include <internal/utils/utils.hpp>
#include <Geode/ui/Button.hpp>
#include "include.hpp"
#include "settings.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

namespace BetterSelectAll {
    bool BetterSelectAllPopup::init() {
        if (!Popup::init(WIDTH, HEIGHT)) {
            return false;
        }

        setTitle("Select All");

        auto menu = ui::node(
            Setup(ui::menu(AxisLayout::create()
                ->setAxisAlignment(AxisAlignment::Start)
                ->setCrossAxisAlignment(geode::AxisAlignment::End)
                ->setGap(SELECT_BUTTON_GAP)
                ->setGrowCrossAxis(true)
                ->setCrossAxisOverflow(false)
                ->setAutoScale(false)
            ))
                .id("button-menu"_spr)
                .size(SELECT_BUTTON_SIZE * 3 + SELECT_BUTTON_GAP * 2, SELECT_BUTTON_SIZE * 3 + SELECT_BUTTON_GAP * 2)
                .pos(WIDTH / 2, HEIGHT / 2)
                .parent(m_mainLayer)
        );

        for (int i = 0; i < 9; i++) {
            ui::node(
                Setup(ui::button(
                    EditorButtonSprite::createWithSprite(
                        fmt::format("direction-{}.png"_spr, i).c_str(), 1.0f, EditorBaseColor::Gray
                    ), this, menu_selector(BetterSelectAllPopup::onSelectButton)
                ))
                    .id(fmt::format("select-button-{}", i))
                    .scaleToFit(SELECT_BUTTON_SIZE)
                    .tag(i)
                    .parent(menu)
            );
        }

        // im prolly never gonna rememebr to redo this unless i come back and redo the ui but in new silly api versions
        // this isnt necessary
        menu->updateLayout();

        auto selectedObjectModeToggle = ui::node(
            Setup(ui::toggler(
                CircleButtonSprite::createWithSpriteFrameName("edit_areaModeBtn05_001.png", 0.75f, CircleBaseColor::Blue),
                CircleButtonSprite::createWithSpriteFrameName("square_01_001.png", 0.75f, CircleBaseColor::Blue),
                this, menu_selector(BetterSelectAllPopup::onToggleSelectedObjectMode)
            ))
                .id("selected-object-mode-toggle"_spr)
                .scaleToFit(BUTTON_SIZE)
                .pos((WIDTH - BACKGROUND_BUFFER) - BUTTON_SIZE / 2, BACKGROUND_BUFFER + BUTTON_SIZE / 2)
                .parent(m_buttonMenu)
        );
        
        m_selectedObjectMode = Settings::BetterSelectAll::saveState.get() ? Mod::get()->getSavedValue<bool>("better-select-all-selected-object-mode") : false;
        selectedObjectModeToggle->toggle(m_selectedObjectMode);

        auto bypassAllToggle = ui::node(
            Setup(ui::toggler(
                CircleButtonSprite::createWithSpriteFrameName("GJ_filterIcon_001.png", 1.0f, CircleBaseColor::Green),
                CircleButtonSprite::createWithSpriteFrameName("deleteFilter_none_001.png", 1.0f, CircleBaseColor::Gray),
                this, menu_selector(BetterSelectAllPopup::onToggleBypassAll)
            ))
                .id("bypass-all-toggle"_spr)
                .scaleToFit(BUTTON_SIZE)
                .pos(selectedObjectModeToggle->getPositionX(), selectedObjectModeToggle->getPositionY() + BUTTON_SIZE + PADDING)
                .parent(m_buttonMenu)
        );

        m_bypassAll = Settings::BetterSelectAll::saveState.get() ? Mod::get()->getSavedValue<bool>("better-select-all-bypass-all") : false;
        bypassAllToggle->toggle(m_bypassAll);

        return true;
    }

    void BetterSelectAllPopup::onSelectButton(cocos2d::CCObject* pSender) {
        editor::selection::add(
            getObjectsWithDirection(static_cast<SelectDirection>(pSender->getTag()), m_selectedObjectMode), 
            true, !m_bypassAll
        );

        editor::update(false, true);

        onClose(nullptr);
    }
    void BetterSelectAllPopup::onToggleBypassAll(cocos2d::CCObject* pSender) {
        m_bypassAll = nwo5::utils::isToggled(pSender);

        Mod::get()->setSavedValue<bool>("better-select-all-bypass-all", m_bypassAll);
    }
    void BetterSelectAllPopup::onToggleSelectedObjectMode(cocos2d::CCObject* pSender) {
        m_selectedObjectMode = nwo5::utils::isToggled(pSender);

        Mod::get()->setSavedValue<bool>("better-select-all-selected-object-mode", m_selectedObjectMode);
    }

    BetterSelectAllPopup* BetterSelectAllPopup::create() {
        auto ret = new BetterSelectAllPopup;

        if (!ret->init()) {
            delete ret;

            return nullptr;
        }

        ret->autorelease();

        return ret;
    } 
}