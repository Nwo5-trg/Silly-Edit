#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>
#include <internal/utils/utils.hpp>
#include <features/compat.hpp>

using namespace geode::prelude;

class $modify(EditorUI) {
    bool init(LevelEditorLayer* editorLayer) {
        if (nwo5::utils::isBetterEditLoaded()) {
            editor::unregisterEditTabButton(nwo5::utils::BETTER_EDIT_ID + "/move-up-quarter-button", !Settings::Compat::removeBEMoveButtons.get());
            editor::unregisterEditTabButton(nwo5::utils::BETTER_EDIT_ID + "/move-down-quarter-button", !Settings::Compat::removeBEMoveButtons.get());
            editor::unregisterEditTabButton(nwo5::utils::BETTER_EDIT_ID + "/move-left-quarter-button", !Settings::Compat::removeBEMoveButtons.get());
            editor::unregisterEditTabButton(nwo5::utils::BETTER_EDIT_ID + "/move-right-quarter-button", !Settings::Compat::removeBEMoveButtons.get());
            editor::unregisterEditTabButton(nwo5::utils::BETTER_EDIT_ID + "/move-up-eighth-button", !Settings::Compat::removeBEMoveButtons.get());
            editor::unregisterEditTabButton(nwo5::utils::BETTER_EDIT_ID + "/move-down-eighth-button", !Settings::Compat::removeBEMoveButtons.get());
            editor::unregisterEditTabButton(nwo5::utils::BETTER_EDIT_ID + "/move-left-eighth-button", !Settings::Compat::removeBEMoveButtons.get());
            editor::unregisterEditTabButton(nwo5::utils::BETTER_EDIT_ID + "/move-right-eighth-button", !Settings::Compat::removeBEMoveButtons.get());
            editor::unregisterEditTabButton(nwo5::utils::BETTER_EDIT_ID + "/move-up-unit-button", !Settings::Compat::removeBEMoveButtons.get());
            editor::unregisterEditTabButton(nwo5::utils::BETTER_EDIT_ID + "/move-down-unit-button", !Settings::Compat::removeBEMoveButtons.get());
            editor::unregisterEditTabButton(nwo5::utils::BETTER_EDIT_ID + "/move-left-unit-button", !Settings::Compat::removeBEMoveButtons.get());
            editor::unregisterEditTabButton(nwo5::utils::BETTER_EDIT_ID + "/move-right-unit-button", !Settings::Compat::removeBEMoveButtons.get());
        }

        return EditorUI::init(editorLayer);
    }
};

class $modify(EditorPauseLayer) {
    static void onModify(auto& pSelf) {
        (void)pSelf.setHookPriority("EditorPauseLayer::init", Priority::LatePost);
    }

    bool init(LevelEditorLayer* layer) {
        if (!EditorPauseLayer::init(layer)) {
            return false;
        }

        auto menu = getChildByID("guidelines-menu");

        if (!menu) {
            return true;
        }

        if (auto button = menu->getChildByID(nwo5::utils::TINKER_EDIT_ID + "/tinker-settings"); button && Settings::Compat::disableTinkerButton.get()) {
            button->setVisible(false);

            menu->updateLayout();
        }
        if (auto button = menu->getChildByID(nwo5::utils::BETTER_EDIT_ID + "/about"); button && Settings::Compat::disableBetterEditButton.get()) {
            button->setVisible(false);

            menu->updateLayout();
        }

        return true;
    }
};