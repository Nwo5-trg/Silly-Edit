#include <Geode/modify/EditorPauseLayer.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <utils/include.hpp>
#include "popup.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

class $modify(LevelEditorLayer) {
    bool init(GJGameLevel* level, bool noUI) {
        if (!LevelEditorLayer::init(level, noUI)) {
            return false;
        }

        nwo5::utils::setupKeybind(this, "General-open-settings", [this] (const Keybind&, bool pDown, bool pRepeat, double) {
            if (pDown && !pRepeat) {
                if (auto popup = static_cast<Settings::SettingsPopup*>(CCDirector::get()->getRunningScene()->getChildByID("settings-popup"_spr))) {
                    popup->onClose(nullptr);
                }
                else {
                    Settings::SettingsPopup::create()->show();
                }
            }
        });

        this->addEventListener(ScrollWheelEvent(), [] (double, double) {
            if (CCDirector::get()->getRunningScene()->getChildByID("settings-popup"_spr)) {
                Sillyedit::shouldBlockScrolling() = true;
            }
        }, Priority::Early);

        return true;
    }
};

class $modify(SettingsEditorPauseLayer, EditorPauseLayer) {
    bool init(LevelEditorLayer* layer) {
        if (!EditorPauseLayer::init(layer)) {
            return false;
        }

        if (!Settings::showSettingsButton) {
            return true;
        }

        auto menu = this->getChildByID("guidelines-menu");

        if (!menu) {
            return true;
        }

        auto spr = CCSprite::create(
            fmt::format(
                "settings-button-{}.png"_spr, 
                string::toLower(Settings::settingsButtonTexture)
            ).c_str()
        );
        spr->setScale(0.85f),

        Setup(ui::buttonSprite(
            fmt::format(
                "settings-button-{}.png"_spr, 
                string::toLower(Settings::settingsButtonTexture)
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