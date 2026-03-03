#include "utils-nodes.hpp"

using namespace geode::prelude;

namespace Utils {
    TextInput* createTextInput(float pWidth, float pHeight, const std::string& pPlaceholder, const std::string& pFont, TextInputCallback pCallback) {
        const float scale = pHeight / DEFAULT_TEXT_INPUT_HEIGHT;

        auto input = TextInput::create(pWidth / scale, pPlaceholder, pFont);
        input->setCallback(std::move(pCallback));
        input->setScale(scale);

        return input;
    }
    TextInput* createTextInput(float pWidth, float pHeight, const std::string& pPlaceholder, TextInputCallback pCallback) {
        return createTextInput(pWidth, pHeight, pPlaceholder, "bigFont.fnt", std::move(pCallback));
    }
    TextInput* createTextInput(float pWidth, const std::string& pPlaceholder, const std::string& pFont, TextInputCallback pCallback) {
        return createTextInput(pWidth, DEFAULT_TEXT_INPUT_HEIGHT, pPlaceholder, pFont, std::move(pCallback));
    }
    TextInput* createTextInput(float pWidth, const std::string& pPlaceholder, TextInputCallback pCallback) {
        return createTextInput(pWidth, DEFAULT_TEXT_INPUT_HEIGHT, pPlaceholder, std::move(pCallback));
    }

    CCMenuItemSpriteExtra* createCircleButton(const std::string& pTexture, CircleBaseColor pColor, CircleBaseSize pSize, CCObject* pTarget, SEL_MenuHandler pCallback, float pScale) {
        return CCMenuItemSpriteExtra::create(
            CircleButtonSprite::createWithSprite(pTexture.c_str(), pScale, pColor, pSize), pTarget, pCallback
        );
    }
    CCMenuItemSpriteExtra* createCircleButton(const std::string& pTexture, CircleBaseColor pColor, CCObject* pTarget, SEL_MenuHandler pCallback, float pScale) {
        return createCircleButton(pTexture, pColor, CircleBaseSize::Medium, pTarget, pCallback, pScale);
    }
    CCMenuItemSpriteExtra* createCircleButtonFrame(const std::string& pTexture, CircleBaseColor pColor, CircleBaseSize pSize, CCObject* pTarget, SEL_MenuHandler pCallback, float pScale) {
        return CCMenuItemSpriteExtra::create(
            CircleButtonSprite::createWithSpriteFrameName(pTexture.c_str(), pScale, pColor, pSize), pTarget, pCallback
        );
    }
    CCMenuItemSpriteExtra* createCircleButtonFrame(const std::string& pTexture, CircleBaseColor pColor, CCObject* pTarget, SEL_MenuHandler pCallback, float pScale) {
        return createCircleButtonFrame(pTexture, pColor, CircleBaseSize::Medium, pTarget, pCallback, pScale);
    }
    CCMenuItemSpriteExtra* createButton(const std::string& pTexture, cocos2d::CCObject* pTarget, cocos2d::SEL_MenuHandler pCallback, float pScale) {
        return CCMenuItemSpriteExtra::create(
            CCSprite::create(pTexture.c_str()), pTarget, pCallback
        );
    }
    CCMenuItemSpriteExtra* createButtonFrame(const std::string& pTexture, cocos2d::CCObject* pTarget, cocos2d::SEL_MenuHandler pCallback, float pScale) {
        return CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName(pTexture.c_str()), pTarget, pCallback
        );
    }
}