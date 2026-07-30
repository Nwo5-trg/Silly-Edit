#include <Geode/modify/EditorPauseLayer.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <internal/utils/utils.hpp>
#include "popup.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

static void tryShowWarningPopup(LevelEditorLayer* pLayer) {
    static bool shown = false;

    if (shown || Settings::General::disableModWarningPopup.get()) {
        return;
    }

    const auto text = Loader::get()->isModLoaded("hjfod.betteredit")
        ? "sillyedit is <cr>LARGELY INCOMPATIBLE</c> with <co>betteredit</c>, if you want more <co>betteredit</c> features, of which sillyedit already adds a couple, use <cf>tinker</c> along side sillyedit (read <cl>about</c> for more info)"
        : "sillyedit is in <cr>BETA</c> ! there prolly will be <cd>bugs</c> and or <cs>crashes</c> (you can disable this popup in <cl>settings</c>)";
    
    auto popup = FLAlertLayer::create("SillyEdit", text, "Ok !");
    popup->m_scene = pLayer;
    popup->show();

    shown = true;
}

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