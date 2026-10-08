#pragma once

#include <alphalaneous.alphas-ui-pack/include/API.hpp>

namespace BetterEditMenu {
    class EditMenu : public cocos2d::CCNode {
    protected:
        cocos2d::CCMenu* m_moveMenu = nullptr;
        cocos2d::CCMenu* m_moveShortcutsMenu = nullptr;
        geode::TextInput* m_moveAmountInput = nullptr;

        cocos2d::CCMenu* m_rotationMenu = nullptr;
        cocos2d::CCMenu* m_rotationShortcutsMenu = nullptr;
        geode::TextInput* m_rotationAmountInput = nullptr;
        CCMenuItemToggler* m_rotationLock = nullptr;

        cocos2d::CCMenu* m_buttonsMenu = nullptr;
        CCMenuItemSpriteExtra* m_leftArrow = nullptr;
        CCMenuItemSpriteExtra* m_rightArrow = nullptr;
        alpha::ui::AdvancedScrollLayer* m_buttonsScroll = nullptr;

        static constexpr float PADDING = 5.0f;
        static constexpr float EDGE_PADDING = 5.0f;
        static constexpr float MAIN_MENU_HEIGHT = 51.0f;
        
        static constexpr float MOVE_SHORTCUT_SIZE = 20.0f;
        static constexpr float MOVE_SHORTCUT_GAP = 2.5f;
        static constexpr float MOVE_BUTTON_SIZE = 25.0f;
        static constexpr float MOVE_BUTTON_GAP = 1.0f;
        static constexpr float MOVE_INPUT_MENU_GAP = 2.5f;
        static constexpr float MOVE_INPUT_EXTRA_SIZE = 20.0f;
        static constexpr cocos2d::CCSize MOVE_AMOUNT_INPUT_SIZE{MOVE_INPUT_EXTRA_SIZE * 2 + MOVE_INPUT_MENU_GAP, MOVE_INPUT_EXTRA_SIZE};

        static constexpr float ROTATION_SHORTCUT_GAP = 2.5f;
        static constexpr float ROTATION_SHORTCUT_SIZE = 20.0f;
        static constexpr float ROTATION_BUTTON_SIZE = 25.0f;
        static constexpr float ROTATION_BUTTON_GAP = 1.0f;
        static constexpr float ROTATION_LOCK_SIZE = 15.0f;
        static constexpr cocos2d::CCSize ROTATION_INPUT_SIZE{ROTATION_SHORTCUT_SIZE * 2 + ROTATION_SHORTCUT_GAP, ROTATION_SHORTCUT_SIZE};

        static constexpr float BUTTONS_SCROLL_MAX_WIDTH_PERCENT= 0.75f;
        static constexpr float BUTTONS_SCROLL_BUTTON_SCALE = 0.5f;
        static constexpr float BUTTONS_SCROLL_OVERSHOOT = 10.0f;
        static constexpr float BUTTONS_SCROLL_ARROW_SIZE = 20.0f;
        static constexpr float BUTTONS_SCROLL_ARROW_GAP = 7.5f;

        enum MoveButton {
            Up,
            Down,
            Left,
            Right
        };
        enum RotationButton {
            CW,
            CCW
        };
        enum FlipButton {
            X,
            Y
        };

        bool init();

        void updateArrowButtons();
        
        void onMoveArrow(cocos2d::CCObject* pSender);
        void onMoveShortcut(cocos2d::CCObject* pSender);
        void onRotationArrow(cocos2d::CCObject* pSender);
        void onRotationShortcut(cocos2d::CCObject* pSender);
        void onFlip(cocos2d::CCObject* pSender);
        void onButtonArrow(cocos2d::CCObject* pSender);

    public:
        void createMoveShortcuts();
        void createRotationShortcuts();

        void createButtons(cocos2d::CCArray* pButtonArray);
        void updateButtonSprite(int pTag);
        
        void position();

        static EditMenu* create();
    };
}