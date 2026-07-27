#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>
#include <internal/utils/utils.hpp>
#include "settings.hpp"

using namespace geode::prelude;

class $modify(EditorUI) {
    static void onModify(auto& pSelf) {
        (void)pSelf.setHookPriorityAfterPost("EditorUI::init", nwo5::utils::TINKER_EDIT_ID);
    }

    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) {
            return false;
        }

        auto undoMenu = this->getChildByID("undo-menu");

        if (!undoMenu) {
            return true;
        }

        // if (auto button = static_cast<CCMenuItemToggler*>(undoMenu->getChildByID(nwo5::utils::TINKER_EDIT_ID + "/hide-ui"))) {
        if (auto button = static_cast<CCMenuItemToggler*>(undoMenu->getChildByID(nwo5::utils::TINKER_EDIT_ID + "/hide-ui-toggle"))) {
            log::error("nothing");
            if (Settings::Compat::tinkerBEUIButton.get()) {
                auto unhideSpr = CircleButtonSprite::createWithSprite(
                    "be-eye-off.png"_spr, 1.0f, CircleBaseColor::Gray, CircleBaseSize::Tiny
                );
                unhideSpr->setOpacity(105);
                static_cast<CCSprite*>(unhideSpr->getTopNode())->setOpacity(105);
                
                auto hideSpr = CircleButtonSprite::createWithSprite(
                    "be-eye-on.png"_spr, 1.0f, CircleBaseColor::Green, CircleBaseSize::Tiny
                );

                button->m_offButton->setNormalImage(hideSpr);
                button->m_onButton->setNormalImage(unhideSpr);

                undoMenu->updateLayout();
            }
        }
        
        return true;
    }
};

class $modify(EditorPauseLayer) {
    static void onModify(auto& pSelf) {
        (void)pSelf.setHookPriorityAfterPost("EditorPauseLayer::init", nwo5::utils::TINKER_EDIT_ID);
    }

    bool init(LevelEditorLayer* layer) {
        if (!EditorPauseLayer::init(layer)) {
            return false;
        }

        auto guidelinesMenu = this->getChildByID("guidelines-menu");

        if (!guidelinesMenu) {
            return true;
        }

        if (auto button = static_cast<CCMenuItemSpriteExtra*>(guidelinesMenu->getChildByID(nwo5::utils::TINKER_EDIT_ID + "/tinker-settings"))) {
            if (Settings::Compat::tinkerBEButton.get()) {
                button->setNormalImage(CCSprite::create("be-button.png"_spr));

                guidelinesMenu->updateLayout();
            }

            if (Settings::Compat::disableTinkerButton.get()) {
                button->setVisible(false);

                guidelinesMenu->updateLayout();
            }
        }

        return true;
    }
};