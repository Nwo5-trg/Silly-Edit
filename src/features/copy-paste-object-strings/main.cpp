#include "settings.hpp"
#include <internal/utils/utils.hpp>

#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

static bool isProbablierObjectString(std::string_view pStr) {
    if (pStr.find_first_of("1234567890") != 0) {
        return false;
    }

    if (pStr.ends_with(',') || pStr.ends_with('.')) {
        return false;
    }

    if (const auto count = Utils::stringCount(pStr, ','); count < 5 || !(count % 2)) {
        return false;
    }

    if ( // this is so extra lol
        const auto count = Utils::stringCount(pStr, "1,"); 
        count != Utils::stringCount(pStr, ";") + 1 
        || count != Utils::stringCount(pStr, "2,") 
        || count != Utils::stringCount(pStr, "3,")
    ) {
        return false;
    }

    return true;
}

class $modify(EditorUI) {
    void doCopyObjects(bool withColor) {
        if (Settings::CopyPasteObjectStrings::enabled.get() && Settings::CopyPasteObjectStrings::copy.get()) {
            clipboard::write(GameManager::get()->m_editorClipboard);

            if (Settings::CopyPasteObjectStrings::copyNotification.get()) {
                geode::Notification::create("Object String Copied To Clipboard", NotificationIcon::Info);
            }
        }
        EditorUI::doCopyObjects(withColor);
    }
    void doPasteObjects(bool withColor) {
        if (!Settings::CopyPasteObjectStrings::enabled.get() || !Settings::CopyPasteObjectStrings::paste.get()) {
            return EditorUI::doPasteObjects(withColor);
        }

        const auto clipboard = clipboard::read();

        if (!isProbablierObjectString(clipboard)) {
            if (Settings::CopyPasteObjectStrings::fallbackEditor.get()) {
                EditorUI::doPasteObjects(withColor);

                if (Settings::CopyPasteObjectStrings::pasteNotification.get()) {
                    geode::Notification::create("Invalid Object String, Pasted Fallback", NotificationIcon::Warning);
                }
            }
            else if (Settings::CopyPasteObjectStrings::pasteNotification.get()) {
                geode::Notification::create("Invalid Object String", NotificationIcon::Warning);
            }

            return;
        }

        if (!Settings::CopyPasteObjectStrings::dontOverrideEditor.get()) {
            const auto ret = GameManager::get()->m_editorClipboard;
            GameManager::get()->m_editorClipboard = clipboard::read();

            EditorUI::doPasteObjects(withColor);

            GameManager::get()->m_editorClipboard = ret;
        }
        else {
            GameManager::get()->m_editorClipboard = clipboard::read();

            EditorUI::doPasteObjects(withColor);
        }

        if (Settings::CopyPasteObjectStrings::pasteNotification.get()) {
            geode::Notification::create("Object String Pasted", NotificationIcon::Info);
        }
    }
};