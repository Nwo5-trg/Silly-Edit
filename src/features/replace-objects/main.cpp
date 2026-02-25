#include "include.hpp"
#include "settings.hpp"
#include <internal/utils/utils.hpp>

#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;
using namespace Utils::Aliases;

class $modify(ReplaceObjectsEditorUI, EditorUI) {
    struct Fields {
        ReplaceObjects::FindAndReplaceMenu* findAndReplaceMenu = nullptr;
        bool canDestroyUndo = false;
    };
    

    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) {
            return false;
        }

        auto menu = Utils::setupNode(
            ReplaceObjects::FindAndReplaceMenu::create(),

            SetNodeID{"find-and-replace-menu"_spr},
            SetNodeParent{m_deleteMenu}
        );

        m_fields->findAndReplaceMenu = menu;

        auto filterMenu = m_deleteMenu->getChildByID("delete-filter-menu");

        constexpr auto MENU_GAP = 5.0f;
        
        //  i stg i remember there being an always invisible delete tab bar but i cant find it (unless its due to edit tab api) so for now ill ignore
        menu->setPosition(
            filterMenu->getPositionX() + filterMenu->getScaledContentWidth() / 2 - menu->getScaledContentWidth(), // cuz i made my ui shitily anchor doesnt work so we manually do that :3c (i hate ui)
            filterMenu->getPositionY() + filterMenu->getScaledContentHeight() / 2 + MENU_GAP * 2
        );

        auto helpButton = m_deleteMenu->getChildByID("delete-help-icon");

        auto toggle = Utils::setupNode(
            CCMenuItemToggler::create(
                CircleButtonSprite::createWithSpriteFrameName("edit_findBtn_001.png", 1.0f, CircleBaseColor::Gray),
                CircleButtonSprite::createWithSpriteFrameName("edit_findBtn_001.png", 1.0f, CircleBaseColor::Blue),
                this, menu_selector(ReplaceObjectsEditorUI::onToggleFindAndReplaceVisibility)
            ),

            SetNodeID{"toggle_find_and_replace_button"_spr},
            SetNodePosition{helpButton->getPositionX() + helpButton->getScaledContentWidth() + MENU_GAP / 2, helpButton->getPositionY()},
            SetNodeScaleWithSize{helpButton->getScaledContentWidth()},
            SetNodeParent{m_deleteMenu}
        );

        const auto show = Settings::ReplaceObjects::saveSettings 
                ? Mod::get()->getSavedValue<bool>("replace-object-show-find-and-replace-menu-toggle") 
                : false;

        toggle->toggle(show);
        menu->setVisible(show);

        return true;
    }

    void onToggleFindAndReplaceVisibility(CCObject* pSender) {
        m_fields->findAndReplaceMenu->setVisible(Utils::isToggled(pSender));

        Mod::get()->setSavedValue<bool>("replace-object-show-find-and-replace-menu-toggle", Utils::isToggled(pSender));
    }

    // so when clicking on an unselectable object the undo queue doesnt get filled up
    void createUndoSelectObject(bool redo) {
        EditorUI::createUndoSelectObject(redo);

        m_fields->canDestroyUndo = true;
    }

    void selectObject(GameObject* object, bool ignoreFilter) {
        if (!Settings::ReplaceObjects::enabled.get()) {
            return EditorUI::selectObject(object, ignoreFilter);
        }
    
        // printing ignorefilter outputed false and checking if(ignoreFilter) evaluated to true so i have no idea man this seems to work tho </3
        // im not kidding when i say i debugged this for a week i dont fucking get it :sob:
        if (ignoreFilter == false && !ignoreFilter) {
            return EditorUI::selectObject(object, ignoreFilter);
        }

        const auto menu = m_fields->findAndReplaceMenu;

        if (!menu || !menu->shouldSelectFilter()) {
            return EditorUI::selectObject(object, ignoreFilter);
        }

        if (menu->evaluateObject(object)) {
            EditorUI::selectObject(object, ignoreFilter);
        }
        else if (m_fields->canDestroyUndo) {
            m_editorLayer->m_undoObjects->removeLastObject(false);
        }
    }

    void selectObjects(CCArray* objects, bool ignoreFilter) {
        if (!Settings::ReplaceObjects::enabled.get()) {
            return EditorUI::selectObjects(objects, ignoreFilter);
        }

        if (ignoreFilter) {
            return EditorUI::selectObjects(objects, ignoreFilter);
        }

        const auto menu = m_fields->findAndReplaceMenu;

        if (!menu || !menu->shouldSelectFilter()) {
            return EditorUI::selectObjects(objects, ignoreFilter);
        }

        auto validObjs = CCArray::create();

        for (auto obj : CCArrayExt<GameObject*>(objects)) {
            if (menu->evaluateObject(obj)) {
                validObjs->addObject(obj);
            }
        }

        if (validObjs->count()) {
            EditorUI::selectObjects(validObjs, ignoreFilter);
        }
        else if (m_fields->canDestroyUndo) {
            m_editorLayer->m_undoObjects->removeLastObject(false);
        }
    }
};