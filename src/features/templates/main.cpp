#include <internal/utils/utils.hpp>
#include <hjfod.gmd-api/include/GMD.hpp>
#include <Geode/modify/LevelOptionsLayer.hpp>
#include <Geode/modify/GameLevelManager.hpp>
#include "settings.hpp"
#include <Geode/ui/Button.hpp>

using namespace geode::prelude;
using namespace nwo5::utils::setup;

static std::filesystem::path getTemplatePath() {
    return Mod::get()->getSaveDir() / "template.gmd";
}

class $modify(TemplatesLevelOptionsLayer, LevelOptionsLayer) {
    void setupOptions() {
        LevelOptionsLayer::setupOptions();

        if (!Settings::Templates::enabled.get()) {
            return;
        }

        auto bg = m_mainLayer->getChildByType<CCScale9Sprite>(0);

        if (!bg) {
            return;
        }

        auto button = nwo5::utils::setupNode(
            Button::createWithNode(ButtonSprite::create("Save\nTemplate"), [] (Button*) {
                editor::save();

                auto res = gmd::exportLevelAsGmd(editor::layer()->m_level, getTemplatePath());

                if (res.isErr()) {
                    return Notification::create("everything is crashing and burning and were all gonna die !", NotificationIcon::Error)->show();
                }

                Notification::create("template updated !", NotificationIcon::Info)->show();
            }),

            SetNodeID{"save-template-button"_spr},
            SetNodeScale{0.65f},
            SetNodeParent{m_mainLayer}
        );
        button->setPosition(
            bg->getPositionX() - bg->getScaledContentWidth() / 2 + button->getScaledContentWidth() / 2 + 10.0f,
            bg->getPositionY() - bg->getScaledContentHeight() / 2 + button->getScaledContentHeight() / 2 + 10.0f
        );
    }
};

class $modify(GameLevelManager) {
    GJGameLevel* createNewLevel() {
        auto ret = GameLevelManager::createNewLevel();

        if (!Settings::Templates::enabled.get()) {
            return ret;
        }

        const auto path = getTemplatePath();

        if (!std::filesystem::exists(path)) {
            return ret;
        }

        auto levelRes = gmd::importGmdAsLevel(path);

        if (levelRes.isErr()) {
            return ret;
        }

        const auto name = std::string{ret->m_levelName};

        ret->copyLevelInfo(levelRes.unwrap());

        ret->m_levelName = name;

        return ret;
    }
};