#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;

static bool isProbablierObjectString(std::string_view pStr) {
    if (pStr.find_first_of("1234567890") != 0) {
        return false;
    }

    if (pStr.ends_with(',') || pStr.ends_with('.') || nwo5::utils::stringCount(pStr, ',') < 5) {
        return false;
    }

    return true;
}

static void disableHooksCuzFuckYou() {
    nwo5::utils::conditionallyEnableHook(
        !CopyObjectStrings::enabled(), nwo5::utils::getTinker(), "EditorUI::doCopyObjects"
    );
    nwo5::utils::conditionallyEnableHook(
        !CopyObjectStrings::enabled(), nwo5::utils::getTinker(), "EditorUI::doPasteObjects"
    );
    nwo5::utils::conditionallyEnableHook(
        !CopyObjectStrings::enabled(), nwo5::utils::getBetterEdit(), "EditorUI::doCopyObjects"
    );
    nwo5::utils::conditionallyEnableHook(
        !CopyObjectStrings::enabled(), nwo5::utils::getBetterEdit(), "EditorUI::doPasteObjects"
    );
}

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

        if (!isProbablierObjectString(clipboard)) {
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





    void Feature::onEditor() {
        disableHooksCuzFuckYou();
    }
    void Feature::onToggled(bool) {
        disableHooksCuzFuckYou();
    }
}