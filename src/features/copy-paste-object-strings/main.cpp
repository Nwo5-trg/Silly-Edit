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
    static void onModify(auto& pSelf) {
        (void)pSelf.setHookPriority("EditorUI::doCopyObjects", Priority::Early);
        (void)pSelf.setHookPriority("EditorUI::doPasteObjects", Priority::Early);
    }

    void doCopyObjects(bool withColor) {
        // would be too annoying to restore setting value if my cpos is disabled so wtv
        if (nwo5::utils::isBetterEditLoaded() && Settings::CopyPasteObjectStrings::enabled.get()) {
            nwo5::utils::setBetterEditSetting<bool>("copy-paste-from-clipboard", false);
        }

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

        EditorUI::doCopyObjects(withColor);
    }
    void doPasteObjects(bool withColor) {
        if (nwo5::utils::isBetterEditLoaded() && Settings::CopyPasteObjectStrings::enabled.get()) {
            nwo5::utils::setBetterEditSetting<bool>("copy-paste-from-clipboard", false);
        }

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