#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

namespace HideUI {
    void EditorUI::onHideUI(CCObject* pSender) {
        this->showUI(!misc::isToggled(pSender));
    }



    void EditorUI::showUI(bool show) {
        GD::EditorUI::showUI(show);

        if (auto toggler = m_fields->hideUIToggle) {
            // toggler->toggle(!show);
            toggler->setVisible(!editor::isPlaytesting());
        }
    }

    void EditorUI::onPause(CCObject* sender) {
        if (auto toggler = m_fields->hideUIToggle) {
            toggler->toggle(false);
        }
        GD::EditorUI::onPause(sender);
    }




    void Feature::onEditor() {
        auto self = editor::ui<HideUI::EditorUI>();

        if (!HideUI::enabled()) {
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

            self->m_fields->hideUIToggle = ui::toggler(
                off, on, self, menu_selector(HideUI::EditorUI::onHideUI)
            )
                .id("hide-ui-toggle"_spr)
                .parent(undoMenu);
        }
    }
}