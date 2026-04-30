#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <internal/utils/utils.hpp>
#include <features/compat.hpp>
#include "popup.hpp"

using namespace geode::prelude;
using namespace nwo5::utils::setup;

static void tryShowWarningPopup(LevelEditorLayer* pLayer) {
    static bool shown = false;

    if (shown || Settings::General::disableModWarningPopup.get()) {
        return;
    }

    auto popup = FLAlertLayer::create("SillyEdit", "sillyedit is in <cr>TESTING</c> ! there prolly will be <cd>bugs</c> and or <cs>crahes</c>", "Ok !");
    popup->m_scene = pLayer;
    popup->show();

    shown = true;
}

class $modify(EditorUI) {
    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) {
            return false;
        }

        nwo5::utils::setupKeybind(this, "general-open-settings", [this] (const Keybind&, bool pDown, bool pRepeat, double) {
            if (pDown && !pRepeat && !CCDirector::get()->getRunningScene()->getChildByType<Settings::SettingsPopup>(0)) {
                Settings::SettingsPopup::create()->show();
            }
        });

        return true;
    }
};

class $modify(LevelEditorLayer) {
    bool init(GJGameLevel* level, bool noUI) {
        if (!LevelEditorLayer::init(level, noUI)) {
            return false;
        }

        // if better edit does 2 i should prolly do 3
        Loader::get()->queueInMainThread([this] {
            Loader::get()->queueInMainThread([this] {
                Loader::get()->queueInMainThread([this] {
                    tryShowWarningPopup(this);
                });
            });
        });

        return true;
    }
};

class $modify(SettingsEditorPauseLayer, EditorPauseLayer) {
    bool init(LevelEditorLayer* layer) {
        if (!EditorPauseLayer::init(layer)) {
            return false;
        }

        if (!Settings::General::showSettingsButton.get()) {
            return true;
        }

        auto menu = getChildByID("guidelines-menu");

        if (!menu) {
            return true;
        }

        auto spr = CCSprite::create(
            fmt::format(
                "settings-button-{}.png"_spr, 
                string::toLower(Settings::General::settingsButtonTexture.get())
            ).c_str()
        );
        spr->setScale(0.85f),

        nwo5::utils::setupNode(
            CCMenuItemSpriteExtra::create(
                spr, this, menu_selector(SettingsEditorPauseLayer::onSESettings)
            ),

            SetNodeID{"se-settings-button"_spr},
            SetNodeParent{menu}
        );

        menu->updateLayout();

        return true;
    }

    void onSESettings(CCObject* sender) {
        Settings::SettingsPopup::create()->show();    
    }
};