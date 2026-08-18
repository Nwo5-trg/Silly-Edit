#include <Geode/modify/EditorUI.hpp>
#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui;

namespace ObjectTabIcons {
    void Feature::onEditor() {
        auto self = editor::ui<ObjectTabIcons::EditorUI>();

        if (!ObjectTabIcons::enabled()) {
            return;
        }

        const std::pair<const char*, std::string> tabs[] {
            {"block-tab", ObjectTabIcons::blockTabMode},  {"outline-tab", ObjectTabIcons::outlineTabMode}, {"slope-tab", ObjectTabIcons::slopeTabMode},
            {"hazard-tab", ObjectTabIcons::hazardTabMode}, {"3d-tab", ObjectTabIcons::threedTabMode}, {"portal-tab", ObjectTabIcons::portalTabMode},
            {"monster-tab", ObjectTabIcons::monsterTabMode}, {"pixel-tab", ObjectTabIcons::pixelTabMode}, {"collectible-tab", ObjectTabIcons::collectibleTabMode},
            {"icon-tab", ObjectTabIcons::iconTabMode}, {"deco-tab", ObjectTabIcons::decoTabMode}, {"sawblade-tab", ObjectTabIcons::sawbladeTabMode},
            {"trigger-tab", ObjectTabIcons::triggerTabMode}, {"custom-tab", ObjectTabIcons::customTabMode}
        };

        for (const auto& [name, mode] : tabs) {
            if (mode == "Default") {
                continue;
            }

            // we're js gonna trust u ery pr from a year ago
            for (int i = 0; i < 2; i++) {
                auto tab = nwo5::utils::getNestedChildSafe<CCSprite*>(
                    self->m_tabsMenu,

                    GetChildQuery{name},
                    GetChildQuery<CCMenuItemSpriteExtra>{i},
                    GetChildQuery<CCSprite>{}
                );
            
                if (!tab) {
                    continue;
                }

                const auto texture = fmt::format("{}{}.png"_spr, name, mode == "Alt" ? "-alt" : "");
                
                auto spr = CCSprite::create(texture.c_str());

                if (!spr || !cocos::isSpriteName(spr, texture.c_str())) {
                    continue;
                }
                
                auto originalIcon = tab->getChildByType<CCNodeRGBA>(0);

                if (!originalIcon) {
                    continue;
                }

                originalIcon->setVisible(false);

                Setup(spr)
                    .id("{}"_spr, name)
                    .scale(0.5f)
                    .opacity(150)
                    .pos(originalIcon)
                    .parent(tab);
            }
        }
    };
}