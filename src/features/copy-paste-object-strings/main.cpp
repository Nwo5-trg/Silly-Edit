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
        !CopyPasteObjectStrings::enabled(), nwo5::utils::getTinker(), "EditorUI::doCopyObjects"
    );
    nwo5::utils::conditionallyEnableHook(
        !CopyPasteObjectStrings::enabled(), nwo5::utils::getTinker(), "EditorUI::doPasteObjects"
    );
    nwo5::utils::conditionallyEnableHook(
        !CopyPasteObjectStrings::enabled(), nwo5::utils::getBetterEdit(), "EditorUI::doCopyObjects"
    );
    nwo5::utils::conditionallyEnableHook(
        !CopyPasteObjectStrings::enabled(), nwo5::utils::getBetterEdit(), "EditorUI::doPasteObjects"
    );
}

void CopyPasteObjectStrings::EditorUI::doCopyObjects(bool withColor) {
    GD::EditorUI::doCopyObjects(withColor);

    if (CopyPasteObjectStrings::enabled() && CopyPasteObjectStrings::copy.get()) {
        std::string str{GameManager::get()->m_editorClipboard};

        if (str.ends_with(';')) {
            str.pop_back();
        }

        clipboard::write(str);

        if (CopyPasteObjectStrings::copyNotification.get()) {
            geode::Notification::create("Object String Copied To Clipboard", NotificationIcon::Info)->show();
        }
    }
}
void CopyPasteObjectStrings::EditorUI::doPasteObjects(bool withColor) {
    if (!CopyPasteObjectStrings::enabled() || !CopyPasteObjectStrings::paste.get()) {
        return GD::EditorUI::doPasteObjects(withColor);
    }

    const auto clipboard = clipboard::read();

    if (!isProbablierObjectString(clipboard)) {
        if (CopyPasteObjectStrings::fallbackEditor.get()) {
            GD::EditorUI::doPasteObjects(withColor);

            if (CopyPasteObjectStrings::pasteNotification.get()) {
                geode::Notification::create("Invalid Object String, Pasted Fallback", NotificationIcon::Warning)->show();
            }
        }
        else if (CopyPasteObjectStrings::pasteNotification.get()) {
            geode::Notification::create("Invalid Object String", NotificationIcon::Warning)->show();
        }

        return;
    }

    if (!CopyPasteObjectStrings::dontOverrideEditor.get()) {
        const std::string ret{GameManager::get()->m_editorClipboard};
        GameManager::get()->m_editorClipboard = clipboard::read();

        GD::EditorUI::doPasteObjects(withColor);

        GameManager::get()->m_editorClipboard = ret;
    }
    else {
        GameManager::get()->m_editorClipboard = clipboard::read();

        GD::EditorUI::doPasteObjects(withColor);
    }

    if (CopyPasteObjectStrings::pasteNotification.get()) {
        geode::Notification::create("Object String Pasted", NotificationIcon::Info)->show();
    }
}

void CopyPasteObjectStrings::Feature::onEditor() {
    auto ui = editor::ui<CopyPasteObjectStrings::EditorUI>();
    
    disableHooksCuzFuckYou();
}
void CopyPasteObjectStrings::Feature::onToggled(bool) {
    disableHooksCuzFuckYou();
}