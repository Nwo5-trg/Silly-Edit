#include <Geode/modify/EditorPauseLayer.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <utils/include.hpp>
#include "popup.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

class $modify(LevelEditorLayer) {
    bool init(GJGameLevel* level, bool noUI) {
        if (!LevelEditorLayer::init(level, noUI)) {
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

class $modify(SettingsEditorPauseLayer, EditorPauseLayer) {
    bool init(LevelEditorLayer* layer) {
        if (!EditorPauseLayer::init(layer)) {
            return false;
        }

        if (!Settings::General::showSettingsButton.get()) {
            return true;
        }

        auto menu = this->getChildByID("guidelines-menu");

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

        Setup(ui::buttonSprite(
            fmt::format(
                "settings-button-{}.png"_spr, 
                string::toLower(Settings::General::settingsButtonTexture.get())
            ), this, menu_selector(SettingsEditorPauseLayer::onSESettings), 0.85f
        ))
            .id("se-settings-button"_spr)
            .parent(menu);

        return true;
    }

    void onSESettings(CCObject* sender) {
        Settings::SettingsPopup::create()->show();    
    }
};