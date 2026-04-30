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
        if (!Settings::DefaultObjectOptions::enabled.get() || noUndo || !reinterpret_cast<DefaultObjectOptionsEditorUI*>(m_editorUI)->m_fields->creatingObject) {
            return LevelEditorLayer::createObject(key, position, noUndo);
        }

        const auto& options = m_fields->options;

        // 1 == id prop, 2 == x pos prop, 3 == y pos prop
        auto objectString = fmt::format(
            "1,{},2,{},3,{}{}", key, 
            position.x + editor::constants::OBJECT_STRING_POSITION_OFFSET.x, 
            position.y + editor::constants::OBJECT_STRING_POSITION_OFFSET.y,
            options.getSimpleOptionsString()
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
        
        return static_cast<GameObject*>(
            createObjectsFromString(objectString, noUndo, true)->firstObject()
        );
    }
};