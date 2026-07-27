#pragma once

namespace UI {
    struct PopupInfo{
        cocos2d::CCRect rect{};
        cocos2d::extension::CCScale9Sprite* bg = nullptr;
        InfoAlertButton* infoButton = nullptr;
        CCMenuItemSpriteExtra* okButton = nullptr;
        cocos2d::CCLabelBMFont* title = nullptr;

        PopupInfo(FLAlertLayer* pPopup);
    };

    PopupInfo setupRobtopPopup(FLAlertLayer* pPopup);

    struct PropInput {
        cocos2d::extension::CCScale9Sprite* bg = nullptr;
        CCTextInputNode* input = nullptr;
        cocos2d::CCLabelBMFont* label = nullptr;
        CCMenuItemSpriteExtra* nextArrow = nullptr;
        CCMenuItemSpriteExtra* previousArrow = nullptr;

        PropInput(int pProp, SetupTriggerPopup* pPopup);
    };
    void moveTextInput(const PropInput& pInput, cocos2d::CCPoint pPos);

    struct PropToggle {
        CCMenuItemToggler* toggler = nullptr;
        cocos2d::CCLabelBMFont* label = nullptr;

        PropToggle(int pProp, geode::ZStringView pLabel, SetupTriggerPopup* pPopup);
    };
    void moveToggler(const PropToggle& pToggler, cocos2d::CCPoint pPos);

    void crushTriggerTypeButtons(SetupTriggerPopup* pPopup);
}