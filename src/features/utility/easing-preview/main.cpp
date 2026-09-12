#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

static Button* setupEasingPopupButton(CCLabelBMFont* pLabel, SetupTriggerPopup* pPopup, EasingPreview::PreviewType pType, int pProp) {
    return Setup(Button::create())
        .size(cocos::getLabelSize("Exponential In Out", Font::Default) * pLabel->getScale())
        .pos(pLabel)
        .callback([pLabel, pPopup, pType, pProp] (Button*) {
            if (pLabel->isVisible()) {
                EasingPreview::SelectEasingPopup::create(pPopup, pType, pProp)->show();
            }
        })
        .callbackSelect([pLabel, scale = pLabel->getScale()] (Button*) {
            pLabel->stopAllActions();
            pLabel->runAction(CCEaseBounceOut::create(
                CCScaleTo::create(0.4f, scale * 1.26f)
            ));
        })
        .callbackUnselect([pLabel, scale = pLabel->getScale()] (Button*) {
            pLabel->stopAllActions();
            pLabel->runAction(CCEaseBounceOut::create(
                CCScaleTo::create(0.4f, scale)
            ));
        })
        .parent(pPopup->m_mainLayer);
}

namespace EasingPreview {
    void SetupTriggerPopup::onClose(CCObject* sender) {
        GD::SetupTriggerPopup::onClose(sender);

        if (auto popup = reinterpret_cast<SelectEasingPopup*>(CCDirector::get()->getRunningScene()->getChildByID("select-easing-popup"_spr))) {
            popup->onClose(nullptr);
        }
    }

    void SetupTriggerPopup::createEasingControls(cocos2d::CCPoint position, float scale, int page, int group) {
        GD::SetupTriggerPopup::createEasingControls(position, scale, page, group);

        auto obj = m_gameObject;

        if (!obj) {
            obj = static_cast<EffectGameObject*>(m_gameObjects->firstObject());
        }

        if (!obj) {
            return;
        }

        switch (obj->m_objectID) {
            case trigger::MOVE_TRIGGER: {
                setupEasingPopupButton(m_easingLabel, this, PreviewType::Move, 30);
            break; }
            case trigger::ROTATE_TRIGGER: {
                setupEasingPopupButton(m_easingLabel, this, PreviewType::Rotate, 30);
            break; }
            case trigger::SCALE_TRIGGER: {
                setupEasingPopupButton(m_easingLabel, this, PreviewType::Scale, 30);
            break; }
            default: {
                setupEasingPopupButton(m_easingLabel, this, PreviewType::None, 30);
            break; }
        }
    }

    void SetupTriggerPopup::createCustomEasingControls(gd::string text, cocos2d::CCPoint position, float scale, int typeProperty, int rateProperty, int page, int group) {
        GD::SetupTriggerPopup::createCustomEasingControls(text, position, scale, typeProperty, rateProperty, page, group);

        auto obj = m_gameObject;

        if (!obj) {
            obj = static_cast<EffectGameObject*>(m_gameObjects->firstObject());
        }

        if (!obj) {
            return;
        }

        geode::Button* button = nullptr;

        switch (obj->m_objectID) {
            case trigger::AREA_MOVE_TRIGGER: [[fallthrough]];
            case trigger::EDIT_AREA_MOVE_TRIGGER: {
                button = setupEasingPopupButton(static_cast<CCLabelBMFont*>(m_customEasingLabels->objectForKey(typeProperty)), this, PreviewType::Move, typeProperty);
            break; }
            case trigger::AREA_ROTATE_TRIGGER: [[fallthrough]];
            case trigger::EDIT_AREA_ROTATE_TRIGGER: {
                button = setupEasingPopupButton(static_cast<CCLabelBMFont*>(m_customEasingLabels->objectForKey(typeProperty)), this, PreviewType::Rotate, typeProperty);
            break; }
            case trigger::AREA_SCALE_TRIGGER: [[fallthrough]];
            case trigger::EDIT_AREA_SCALE_TRIGGER: {
                button = setupEasingPopupButton(static_cast<CCLabelBMFont*>(m_customEasingLabels->objectForKey(typeProperty)), this, PreviewType::Scale, typeProperty);
            break; }
            default: {
                button = setupEasingPopupButton(static_cast<CCLabelBMFont*>(m_customEasingLabels->objectForKey(typeProperty)), this, PreviewType::None, typeProperty);
            break; }
        }

        if (typeProperty == 248) {
            button->setID("select-easing-button-2"_spr);
        }
    }
}