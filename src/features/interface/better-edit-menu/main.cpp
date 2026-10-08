#include <alphalaneous.editortab_api/include/EditorTabAPI.hpp>
#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;
using namespace alpha::prelude;

namespace BetterEditMenu {
    void EditorUI::updateEditButtonColor(int tag, ccColor3B color) {
        GD::EditorUI::updateEditButtonColor(tag, color);

        if (auto editMenu = editor::ui<BetterEditMenu::EditorUI>()->m_fields->editMenu) {
            editMenu->updateButtonSprite(tag);
        }
    }

    void EditorUI::toggleSpecialEditButtons() {
        GD::EditorUI::toggleSpecialEditButtons();

        if (auto editMenu = editor::ui<BetterEditMenu::EditorUI>()->m_fields->editMenu) {
            editMenu->updateButtonSprite(25);
            editMenu->updateButtonSprite(26);
            editMenu->updateButtonSprite(27);
            editMenu->updateButtonSprite(28);
        }
    }




    
    void EditButtonBar::loadFromItems(CCArray* objects, int rows, int columns, bool keepPage) {
        GD::EditButtonBar::loadFromItems(objects, rows, columns, keepPage);

        if (!BetterEditMenu::enabled() || this->getID() != "edit-tab-bar") {
            return;
        }

        if (auto editMenu = editor::ui<BetterEditMenu::EditorUI>()->m_fields->editMenu) {
            auto out = CCArray::create();

            for (auto button : CCArrayExt<CCNode*>(m_buttonArray)) {
                if (BetterEditMenu::noDefaultMoveButtons) {
                    static std::unordered_set<std::string> map{
                        "move-up-small-button", 
                        "move-down-small-button", 
                        "move-left-small-button", 
                        "move-right-small-button", 
                        "move-up-button", 
                        "move-down-button", 
                        "move-left-button", 
                        "move-right-button", 
                        "move-up-big-button", 
                        "move-down-big-button", 
                        "move-left-big-button", 
                        "move-right-big-button", 
                        "move-up-tiny-button", 
                        "move-down-tiny-button", 
                        "move-left-tiny-button", 
                        "move-right-tiny-button", 
                        "move-up-half-button", 
                        "move-down-half-button", 
                        "move-left-half-button", 
                        "move-right-half-button"
                    };

                    if (map.contains(button->getID())) {
                        continue;
                    }
                }

                if (BetterEditMenu::noDefaultRotateButtons) {
                    static std::unordered_set<std::string> map{
                        "rotate-cw-button", 
                        "rotate-ccw-button", 
                        "rotate-cw-button", 
                        "rotate-ccw-button", 
                        "rotate-cw-45-button", 
                        "rotate-ccw-45-button"
                    };

                    if (map.contains(button->getID())) {
                        continue;
                    }
                }

                if (BetterEditMenu::noDefaultFlipButtons) {
                    static std::unordered_set<std::string> map{
                        "flip-x-button", 
                        "flip-y-button"
                    };

                    if (map.contains(button->getID())) {
                        continue;
                    }
                }

                out->addObject(button);
            }

            editMenu->createButtons(out);
        }
    }





    void Feature::onEditor() {
        auto self = editor::ui<BetterEditMenu::EditorUI>();
        auto fields = self->m_fields.self();

        alpha::editor_tabs::addTabSwitchCallback([self] (ZStringView pTab) {
            if (auto editMenu = self->m_fields->editMenu) {
                editMenu->setVisible(pTab == "edit");
            }
        });
    }

    void Feature::onUpdate() {
        if (BetterEditMenu::enabled()) {
            editor::ui()->m_editButtonBar->setVisible(false);
        }
    }

    void Feature::onToggled(bool pEnabled) {
        auto self = editor::ui<BetterEditMenu::EditorUI>();
        auto fields = self->m_fields.self();

        if (pEnabled) {
            fields->editMenu = Setup(EditMenu::create())
                .id("edit-menu"_spr)
                .visible(alpha::editor_tabs::getCurrentTab().unwrapOrDefault() == "edit")
                .order(self->m_editButtonBar)
                .parent(self)
                .addTo(self->m_uiItems);
        }
        else if (fields->editMenu) {
            fields->editMenu->removeMeAndCleanup();
            fields->editMenu = nullptr;
            self->m_editButtonBar->setVisible(alpha::editor_tabs::getCurrentTab().unwrapOrDefault() == "edit");
        }
        
        editor::updateEditorTabButtons();
    }

    void Feature::onUIUpdated(float) {
        auto self = editor::ui<BetterEditMenu::EditorUI>();
        auto fields = self->m_fields.self();

        if (fields->editMenu) {
            fields->editMenu->position();
        }
    }

    void Feature::onSettingChanged(std::string pName, GenericSetting*) {
        auto self = editor::ui<BetterEditMenu::EditorUI>();
        auto fields = self->m_fields.self();

        editor::updateEditorTabButtons();

        if (auto editMenu = self->m_fields->editMenu) {            
            if (pName.contains("Move")) {
                editMenu->createMoveShortcuts();
            }
            else if (pName.contains("Rotate")) {
                editMenu->createRotationShortcuts();
            }
        }
    }
}