#include <internal/utils/utils.hpp>
#include "include.hpp"

using namespace geode::prelude;

namespace UI {
    PopupInfo::PopupInfo(FLAlertLayer* pPopup) {
        title = pPopup->m_mainLayer->getChildByType<CCLabelBMFont>(0);
        if (!title) {
            pPopup->getChildByType<CCLabelBMFont>(0);
        }

        bg = pPopup->m_mainLayer->getChildByType<CCScale9Sprite>(0);

        okButton = pPopup->m_buttonMenu->getChildByType<CCMenuItemSpriteExtra>(0);

        infoButton = pPopup->m_buttonMenu->getChildByType<InfoAlertButton>(0);

        rect.size = bg->getContentSize();
        rect.origin = bg->getPosition() - rect.size / 2;
    }

    PopupInfo setupRobtopPopup(FLAlertLayer* pPopup) {
        PopupInfo info{pPopup};

        const auto layerOffset = (CCDirector::get()->getWinSize() - info.rect.size) / 2;
        pPopup->m_mainLayer->setPosition(layerOffset);
        pPopup->m_mainLayer->setContentSize(info.rect.size);
        for (auto obj : pPopup->m_mainLayer->getChildrenExt()) {
            obj->setPosition(obj->getPosition() - layerOffset);
        }

        auto menuOffset = pPopup->m_buttonMenu->getPosition();
        pPopup->m_buttonMenu->setPosition(0.0f, 0.0f);
        for (auto obj : pPopup->m_buttonMenu->getChildrenExt()) {
            obj->setPosition(obj->getPosition() + menuOffset);
        }

        return info;
    }

    PropInput::PropInput(int pProp, SetupTriggerPopup* pPopup) {
        input = static_cast<CCTextInputNode*>(pPopup->m_inputNodes->objectForKey(pProp));
        for (auto node : pPopup->m_mainLayer->getChildrenExt()) {
            if (node->getPosition() == input->getPosition()) {
                if (auto bgNode = typeinfo_cast<CCScale9Sprite*>(node)) {
                    bg = bgNode;

                    break;
                }
            }
        }
        label = static_cast<CCLabelBMFont*>(pPopup->m_inputLabels->objectForKey(pProp));
        for (auto node : pPopup->m_buttonMenu->getChildrenExt()) {
            if (node->getTag() == pProp) {
                if (auto button = typeinfo_cast<CCMenuItemSpriteExtra*>(node)) {
                    if (!previousArrow) {
                        previousArrow = button;
                    }
                    else if (!nextArrow) {
                        nextArrow = button;
                    }
                    else {
                        break;
                    }
                }
            }
        }
    }
    void moveTextInput(const PropInput& pInput, CCPoint pPos) {
        const CCPoint delta = pPos - pInput.input->getPosition();

        pInput.bg->setPosition(pPos);
        pInput.input->setPosition(pPos);
        pInput.label->setPosition(pInput.label->getPosition() + delta);
        pInput.nextArrow->setPosition(pInput.nextArrow->getPosition() + delta);
        pInput.previousArrow->setPosition(pInput.previousArrow->getPosition() + delta);
    }

    PropToggle::PropToggle(int pProp, ZStringView pLabel, SetupTriggerPopup* pPopup) {
        for (auto node : pPopup->m_buttonMenu->getChildrenExt()) {
            if (node->getTag() == pProp) {
                if (auto togglerNode = typeinfo_cast<CCMenuItemToggler*>(node)) {
                    toggler = togglerNode;

                    break;
                }
            }
        }
        for (auto node : pPopup->m_mainLayer->getChildrenExt()) {
            if (auto labelNode = typeinfo_cast<CCLabelBMFont*>(node)) {
                if (labelNode->getString() == pLabel.c_str()) {
                    label = labelNode;
                    
                    break;
                }
            }
        }
    }
    void moveToggler(const PropToggle& pToggler, CCPoint pPos) {
        const CCPoint delta = pPos - pToggler.toggler->getPosition();

        pToggler.toggler->setPosition(pPos);
        pToggler.label->setPosition(pToggler.label->getPosition() + delta);
    }

    void crushTriggerTypeButtons(SetupTriggerPopup* pPopup) {
        const CCPoint triggerTypePositions[] { {20.0f, 50.0f}, {20.0f, 20.0f}, {105.0f, 20.0f} };
        for (int i = 0; i < 3; i++) {
            pPopup->m_buttonMenu->getChildByType<CCMenuItemToggler>(i)->setPosition(triggerTypePositions[i]);
            pPopup->m_mainLayer->getChildByType<CCLabelBMFont>(i + 1)->setPosition(triggerTypePositions[i].x + 20, triggerTypePositions[i].y);
        }
    }
}