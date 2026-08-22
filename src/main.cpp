#include <Geode/modify/LevelEditorLayer.hpp>
#include <utils/include.hpp>
#include <settings/include.hpp>

using namespace geode::prelude;

static void tryShowWarningPopup(LevelEditorLayer* pLayer) {
    static bool shown = false;

    if (shown || Settings::disableModWarningPopup) {
        return;
    }

    const auto text = Sillyedit::isBetterEditLoaded()
        ? "using <co>betteredit</c> is <cr>UNSUPPORTED</c> by sillyedit, might still work but no promises (read <cl>about</c> for more info)"
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