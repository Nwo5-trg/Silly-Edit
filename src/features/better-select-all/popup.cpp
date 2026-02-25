#include <internal/utils/utils.hpp>
#include <Geode/ui/Button.hpp>
#include "include.hpp"
#include "settings.hpp"

using namespace geode::prelude;
using namespace Utils::Aliases;

namespace BetterSelectAll {
    bool BetterSelectAllPopup::init() {
        if (!Popup::init(WIDTH, HEIGHT)) {
            return false;
        }

        setTitle("Select All");

        auto menu = Utils::setupNode(
            CCMenu::create(),

            SetNodeID{"button-menu"_spr},
            SetNodeSize{SELECT_BUTTON_SIZE * 3 + SELECT_BUTTON_GAP * 2, SELECT_BUTTON_SIZE * 3 + SELECT_BUTTON_GAP * 2},
            SetNodePosition{WIDTH / 2, HEIGHT / 2},
            SetNodeParent{m_mainLayer}
        );

        for (int i = 0; i < 9; i++) {
            auto button = Utils::setupNode(
                CCMenuItemSpriteExtra::create(
                    EditorButtonSprite::createWithSprite(
                        fmt::format("direction-{}.png"_spr, i).c_str(), 1.0f, EditorBaseColor::Gray
                    ), this, menu_selector(BetterSelectAllPopup::onSelectButton)
                ),

                SetNodeID{"select-button-{}", i},
                SetNodeScaleWithSize{SELECT_BUTTON_SIZE},
                SetNodeTag{i},
                SetNodeParent{menu}
            );
        }

        auto selectedObjectModeToggle = Utils::setupNode(
            CCMenuItemToggler::create(
                CircleButtonSprite::createWithSpriteFrameName("edit_areaModeBtn05_001.png", 0.75f, CircleBaseColor::Blue),
                CircleButtonSprite::createWithSpriteFrameName("square_01_001.png", 1.0f, CircleBaseColor::Blue),
                this, menu_selector(BetterSelectAllPopup::onToggleSelectedObjectMode)
            ),

            SetNodeID{"selected-object-mode-toggle"_spr},
            SetNodeScaleWithSize{BUTTON_SIZE},
            SetNodePosition{(WIDTH - BACKGROUND_BUFFER) - BUTTON_SIZE / 2, BACKGROUND_BUFFER + BUTTON_SIZE / 2},
            SetNodeParent{m_buttonMenu}
        );
        m_selectedObjectMode = Settings::BetterSelectAll::saveState ? Mod::get()->getSavedValue<bool>("better-select-all-selected-object-mode") : false;
        selectedObjectModeToggle->toggle(m_selectedObjectMode);

        auto bypassAllToggle = Utils::setupNode(
            CCMenuItemToggler::create(
                CircleButtonSprite::createWithSpriteFrameName("GJ_filterIcon_001.png", 1.0f, CircleBaseColor::Green),
                CircleButtonSprite::createWithSpriteFrameName("deleteFilter_none_001.png", 1.0f, CircleBaseColor::Gray),
                this, menu_selector(BetterSelectAllPopup::onToggleBypassAll)
            ),

            SetNodeID{"bypass-all-toggle"_spr},
            SetNodeScaleWithSize{BUTTON_SIZE},
            SetNodePosition{selectedObjectModeToggle->getPositionX(), selectedObjectModeToggle->getPositionY() + BUTTON_SIZE + PADDING},
            SetNodeParent{m_buttonMenu}
        );

        m_bypassAll = Settings::BetterSelectAll::saveState ? Mod::get()->getSavedValue<bool>("better-select-all-bypass-all") : false;
        bypassAllToggle->toggle(m_bypassAll);

        menu->setLayout(AxisLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
            ->setCrossAxisAlignment(geode::AxisAlignment::End)
            ->setGap(SELECT_BUTTON_GAP)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(false)
            ->setAutoScale(false)
        );

        return true;
    }

    void BetterSelectAllPopup::onSelectButton(cocos2d::CCObject* pSender) {
        Editor::Selection::add(getObjectsWithDirection(static_cast<SelectDirection>(pSender->getTag()), m_selectedObjectMode), true, !m_bypassAll);

        onClose(nullptr);
    }
    void BetterSelectAllPopup::onToggleBypassAll(cocos2d::CCObject* pSender) {
        m_bypassAll = Utils::isToggled(pSender);

        Mod::get()->setSavedValue<bool>("better-select-all-bypass-all", m_bypassAll);
    }
    void BetterSelectAllPopup::onToggleSelectedObjectMode(cocos2d::CCObject* pSender) {
        m_selectedObjectMode = Utils::isToggled(pSender);

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