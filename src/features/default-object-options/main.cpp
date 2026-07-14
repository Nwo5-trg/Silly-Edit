#include "settings.hpp"
#include "include.hpp"
#include <internal/utils/utils.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>

using namespace geode::prelude;

class $modify(EditorUI) {
    bool onCreate() {
        // to not break custom objects
        DefaultObjectOptions::shouldApplyObjectOptions() = m_selectedObjectIndex >= 1;

        auto ret = EditorUI::onCreate();

        DefaultObjectOptions::shouldApplyObjectOptions() = false;

        return ret;
    }
};

class $modify(LevelEditorLayer) {
    struct Fields {
        DefaultObjectOptions::ObjectOptions options;
    };

    void updateSimpleObjectsString() {
        m_fields->options.updateSimpleOptionsString(
            Settings::DefaultObjectOptions::dontFade.get(),
            Settings::DefaultObjectOptions::dontEnter.get(),
            Settings::DefaultObjectOptions::noGlow.get()
        );
    }

    bool init(GJGameLevel* level, bool noUI) {
        if (!LevelEditorLayer::init(level, noUI)) {
            return false;
        }

        listenForAllSavedSettingChanges(this, [this] (std::string_view pKey, GenericSetting* pSetting) {
            const auto name = pSetting->name();

            if (name == "Dont Fade" || name == "Dont Enter" || name == "No Glow") {
                this->updateSimpleObjectsString();
            }
            else if (name == "Use JSON") {
                DefaultObjectOptions::parseOptions(this->m_fields->options);
            }
        }, "Default Object Options");

        DefaultObjectOptions::parseOptions(m_fields->options);
        updateSimpleObjectsString();

        return true;
    }

    // please god remind me to actually get off my ass and reverse engineer this cuz this is diabolical
    GameObject* createObject(int key, CCPoint position, bool noUndo) {
        auto ret = LevelEditorLayer::createObject(key, position, noUndo);

        if (!ret || !Settings::DefaultObjectOptions::enabled.get() || noUndo || !DefaultObjectOptions::shouldApplyObjectOptions()) {
            return ret;
        }

        const auto& options = m_fields->options;

        auto objectString = fmt::format(
            "{}{}", ret->getSaveString(this), options.getSimpleOptionsString()
        );
        
        if (Settings::DefaultObjectOptions::useJSON.get()) {
            if (options.defaultExists()) {
                objectString.append(options.getDefaultOptionsString());
            }
            if (options.idHasOptions(key)) {
                objectString.append(options.getOptionsStringForID(key));
            }
        }

        if (auto i = objectString.find(';'); i != std::string::npos) {
            objectString = objectString.substr(0, i);
        }
        
        editor::object::remove(ret);

        return static_cast<GameObject*>(
            createObjectsFromString(objectString, noUndo, true)->firstObject()
        );
    }
};