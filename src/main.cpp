#include <Geode/modify/LevelEditorLayer.hpp>
#include <utils/include.hpp>
#include <feature/include.hpp>
#include <settings/include.hpp>
// i dont wanna have to rebuild the whole project if i change a popup setting so im js including it in what i need
#include <settings/core/popup.hpp>

using namespace geode::prelude;

static void tryShowWarningPopup(LevelEditorLayer* pLayer) {
    static bool shown = false;

    if (shown || sillyedit::settings::disableModWarningPopup) {
        return;
    }

    const auto text = sillyedit::utils::isBetterEditLoaded()
        ? "using <co>betteredit</c> is <cr>UNSUPPORTED</c> by sillyedit, might still work but no promises, mayb use tinker instead (read <cl>about</c> for more info)"
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

$on_mod(Loaded) {  
    FeatureManager::get()->load();
    SettingsManager::get()->load();
}