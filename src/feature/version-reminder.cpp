#include <Geode/modify/EditorUI.hpp>

// wont do anything until v1 lolll

using namespace geode::prelude;

class $modify(EditorUI) {
    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) {
            return false;
        }

        const auto version = Mod::get()->getVersion();

        if (!Mod::get()->hasSavedValue("version-reminder-version")) {
            Mod::get()->setSavedValue("version-reminder-version", version.toVString(false));

            return true;
        }

        const auto savedVersion = VersionInfo::parse(
            Mod::get()->getSavedValue<std::string>("version-reminder-version")
        ).unwrapOrDefault();

        if (version > savedVersion) {
            if (version.getMinor() > savedVersion.getMinor()) {
                Notification::create(
                    "sillyedit has been updated ! this update adds new features so check those out :3 (or disable them if u want)",
                    NotificationIcon::Info
                )->show();
            }
            else if (version.getPatch() > savedVersion.getPatch()) {
                Notification::create(
                    "sillyedit has been updated ! this update's just bugfixes so expect nothing to change on ur end :3",
                    NotificationIcon::Info
                )->show();
            }

            Mod::get()->setSavedValue("version-reminder-version", version.toVString(false));
        }
        
        return true;
    }
};