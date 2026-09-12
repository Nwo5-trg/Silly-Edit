#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;

namespace CopyObjectStrings {
    void EditorUI::doCopyObjects(bool withColor) {
        GD::EditorUI::doCopyObjects(withColor);

        if (CopyObjectStrings::enabled() && CopyObjectStrings::copy) {
            std::string str{GameManager::get()->m_editorClipboard};

            if (str.ends_with(';')) {
                str.pop_back();
            }

            clipboard::write(str);

            if (CopyObjectStrings::copyNotification) {
                geode::Notification::create("Object String Copied To Clipboard", NotificationIcon::Info)->show();
            }
        }
    }

    void EditorUI::doPasteObjects(bool withColor) {
        if (!CopyObjectStrings::enabled() || !CopyObjectStrings::paste) {
            return GD::EditorUI::doPasteObjects(withColor);
        }

        const auto clipboard = clipboard::read();

        if (!sillyedit::utils::isProbablierObjectString(clipboard)) {
            if (CopyObjectStrings::fallbackEditor) {
                GD::EditorUI::doPasteObjects(withColor);

                if (CopyObjectStrings::pasteNotification) {
                    geode::Notification::create("Invalid Object String, Pasted Fallback", NotificationIcon::Warning)->show();
                }
            }
            else if (CopyObjectStrings::pasteNotification) {
                geode::Notification::create("Invalid Object String", NotificationIcon::Warning)->show();
            }

            return;
        }

        if (!CopyObjectStrings::dontOverrideEditor) {
            const std::string ret{GameManager::get()->m_editorClipboard};
            GameManager::get()->m_editorClipboard = clipboard::read();

            GD::EditorUI::doPasteObjects(withColor);

            GameManager::get()->m_editorClipboard = ret;
        }
        else {
            GameManager::get()->m_editorClipboard = clipboard::read();

            GD::EditorUI::doPasteObjects(withColor);
        }

        if (CopyObjectStrings::pasteNotification) {
            geode::Notification::create("Object String Pasted", NotificationIcon::Info)->show();
        }
    }





    void Feature::onToggled(bool pEnabled) {
        sillyedit::utils::conditionallyEnableHook(
            !pEnabled, sillyedit::utils::getTinker(), "EditorUI::doCopyObjects"
        );
        sillyedit::utils::conditionallyEnableHook(
            !pEnabled, sillyedit::utils::getTinker(), "EditorUI::doPasteObjects"
        );
        sillyedit::utils::conditionallyEnableHook(
            !pEnabled, sillyedit::utils::getBetterEdit(), "EditorUI::doCopyObjects"
        );
        sillyedit::utils::conditionallyEnableHook(
            !pEnabled, sillyedit::utils::getBetterEdit(), "EditorUI::doPasteObjects"
        );
    }
}