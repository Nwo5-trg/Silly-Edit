#include "settings.hpp"
#include "include.hpp"
#include <internal/utils/utils.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>

using namespace geode::prelude;

class $modify(DefaultObjectOptionsEditorUI, EditorUI) {
    struct Fields {
        bool creatingObject = false;
    };
    
    bool onCreate() {
        auto& creatingObject = m_fields->creatingObject;
        creatingObject = true;

        auto ret = EditorUI::onCreate();

        creatingObject = false;

        return ret;
    }
};

class $modify(LevelEditorLayer) {
    struct Fields {
        DefaultObjectOptions::ObjectOptions options;
    };

    bool init(GJGameLevel* level, bool noUI) {
        if (!LevelEditorLayer::init(level, noUI)) {
            return false;
        }
        
        DefaultObjectOptions::parseOptions(m_fields->options);

        m_fields->options.updateSimpleOptionsString(
            Settings::DefaultObjectOptions::dontFade.get(),
            Settings::DefaultObjectOptions::dontEnter.get(),
            Settings::DefaultObjectOptions::noGlow.get()
        );

        return true;
    }
    GameObject* createObject(int key, CCPoint position, bool noUndo) {
        auto ret = LevelEditorLayer::createObject(key, position, noUndo);

        if (!ret || !Settings::DefaultObjectOptions::enabled.get() || noUndo || !reinterpret_cast<DefaultObjectOptionsEditorUI*>(m_editorUI)->m_fields->creatingObject) {
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