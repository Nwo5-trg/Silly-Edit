#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

namespace HideUIToggle {
    void EditorUI::onHideUI(CCObject* pSender) {
        this->showUI(!nwo5::utils::isToggled(pSender));
    }



    void EditorUI::showUI(bool show) {
        GD::EditorUI::showUI(show);

        if (auto toggler = m_fields->hideUIToggle) {
            // toggler->toggle(!show);
            toggler->setVisible(!editor::isPlaytesting());
        }
    }




    void Feature::onEditor() {
        auto self = editor::ui<EditorUI>();

        if (!HideUIToggle::enabled()) {
            return;
        }

        if (auto undoMenu = self->getChildByID("undo-menu")) {
            auto off = CircleButtonSprite::createWithSprite(
                "eye-on.png"_spr, 1.0f, CircleBaseColor::Green, CircleBaseSize::Tiny
            );

            auto on = CircleButtonSprite::createWithSprite(
                "eye-off.png"_spr, 1.0f, CircleBaseColor::Gray, CircleBaseSize::Tiny
            );
            on->setOpacity(105);
            static_cast<CCSprite*>(on->getTopNode())->setOpacity(105);

            self->m_fields->hideUIToggle = ui::node(Setup(ui::toggler(
                off, on, self, menu_selector(HideUIToggle::EditorUI::onHideUI)
            ))
                .id("hide-ui-toggle"_spr)
                .parent(undoMenu)
            );
        }
    }
}