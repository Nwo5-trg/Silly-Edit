#include <Geode/modify/EditorUI.hpp>
#include <features/shared.hpp>
#include <internal/utils/utils.hpp>
#include "settings.hpp"

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

class $modify(EditorUI) {
    static void disableTinkerStuffs() {
        nwo5::utils::conditionallyEnableHook(
            !Settings::CopyPasteObjectStrings::enabled.get(), nwo5::utils::getTinker(), "EditorUI::doCopyObjects"
        );
        nwo5::utils::conditionallyEnableHook(
            !Settings::CopyPasteObjectStrings::enabled.get(), nwo5::utils::getTinker(), "EditorUI::doPasteObjects"
        );
    }

    bool init(LevelEditorLayer* editorLayer) {
        listenForSavedSettingChanges<SillySetting<bool>>(this, "copy-paste-object-strings-enabled", [] (SillySetting<bool>*) {
            disableTinkerStuffs();
        });
        disableTinkerStuffs();
        
        return EditorUI::init(editorLayer);
    }

    void doCopyObjects(bool withColor) {
        EditorUI::doCopyObjects(withColor);

        if (Settings::CopyPasteObjectStrings::enabled.get() && Settings::CopyPasteObjectStrings::copy.get()) {
            std::string str{GameManager::get()->m_editorClipboard};

            if (str.ends_with(';')) {
                str.pop_back();
            }

            clipboard::write(str);

            if (Settings::CopyPasteObjectStrings::copyNotification.get()) {
                geode::Notification::create("Object String Copied To Clipboard", NotificationIcon::Info)->show();
            }
        }
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
                    geode::Notification::create("Invalid Object String, Pasted Fallback", NotificationIcon::Warning)->show();
                }
            }
            else if (Settings::CopyPasteObjectStrings::pasteNotification.get()) {
                geode::Notification::create("Invalid Object String", NotificationIcon::Warning)->show();
            }

            return;
        }

        if (!Settings::CopyPasteObjectStrings::dontOverrideEditor.get()) {
            const std::string ret{GameManager::get()->m_editorClipboard};
            GameManager::get()->m_editorClipboard = clipboard::read();

            EditorUI::doPasteObjects(withColor);

            GameManager::get()->m_editorClipboard = ret;
        }
        else {
            GameManager::get()->m_editorClipboard = clipboard::read();

            EditorUI::doPasteObjects(withColor);
        }

        if (Settings::CopyPasteObjectStrings::pasteNotification.get()) {
            geode::Notification::create("Object String Pasted", NotificationIcon::Info)->show();
        }
    }
};