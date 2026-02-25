#include "settings.hpp"
#include <internal/utils/utils.hpp>

#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;
using namespace Utils::Aliases;

class $modify(EditorUI) {
	bool init(LevelEditorLayer* editorLayer) {	
		if (!EditorUI::init(editorLayer)) {
            return false;
        }

        if (!Settings::ObjectTabIcons::enabled.get()) {
            return true;
        }

        const std::pair<const char*, int> tabs[] {
            {"block-tab", Settings::ObjectTabIcons::blockTabMode.get()},  {"outline-tab", Settings::ObjectTabIcons::outlineTabMode.get()}, {"slope-tab", Settings::ObjectTabIcons::slopeTabMode.get()},
            {"hazard-tab", Settings::ObjectTabIcons::hazardTabMode.get()}, {"3d-tab", Settings::ObjectTabIcons::threedTabMode.get()}, {"portal-tab", Settings::ObjectTabIcons::portalTabMode.get()},
            {"monster-tab", Settings::ObjectTabIcons::monsterTabMode.get()}, {"pixel-tab", Settings::ObjectTabIcons::pixelTabMode.get()}, {"collectible-tab", Settings::ObjectTabIcons::collectibleTabMode.get()},
            {"icon-tab", Settings::ObjectTabIcons::iconTabMode.get()}, {"deco-tab", Settings::ObjectTabIcons::decoTabMode.get()}, {"sawblade-tab", Settings::ObjectTabIcons::sawbladeTabMode.get()},
            {"trigger-tab", Settings::ObjectTabIcons::triggerTabMode.get()}, {"custom-tab", Settings::ObjectTabIcons::customTabMode.get()}
        };

        for (const auto [name, mode] : tabs) {
            if (!mode) {
                continue;
            }

            // we're js gonna trust u ery pr from a year ago
            for (int i = 0; i < 2; i++) {
                auto tab = Utils::getNestedChildSafe<CCSprite*>(
                    m_tabsMenu,

                    GetChildQuery{name},
                    GetChildQuery<CCMenuItemSpriteExtra>{i},
                    GetChildQuery<CCSprite>{}
                );
         
                if (!tab) {
                    continue;
                }

                constexpr int ALT_TEXTURE_MODE = 2;

                const auto texture = fmt::format("{}{}.png"_spr, name, mode == ALT_TEXTURE_MODE ? "-alt" : "");
                
                auto spr = CCSprite::create(texture.c_str());

                if (!spr || !cocos::isSpriteName(spr, texture.c_str())) {
                    continue;
                }
                
                auto originalIcon = tab->getChildByType<CCSprite>(0);

                if (!originalIcon) {
                    continue;
                }

                originalIcon->setVisible(false);

                Utils::setupNode(
                    spr,  
                    
                    SetNodeID{"{}"_spr, name},
                    SetNodeScale{0.5f},
                    SetNodeOpacity{150},
                    SetNodePosition{originalIcon},
                    SetNodeParent{tab}
                );
            }
        }
        
		return true;
	}
};