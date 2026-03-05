#include "settings.hpp"
#include "include.hpp"
#include <internal/utils/utils.hpp>

#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>

using namespace geode::prelude;

namespace DefaultObjectOptions {
    void ObjectOptions::addOption(int pID, std::string_view pCustomString) {
        m_options[pID].append(fmt::format(",{}", pCustomString));

        if (!pID) {
            m_defaultOption = true;
        }
    }

    bool ObjectOptions::idHasOptions(int pID) const {
        return m_options.find(pID) != m_options.end();
    }
    // starts with ,
    std::string ObjectOptions::getOptionsStringForID(int pID) const {
        if (const auto it = m_options.find(pID); it != m_options.end()) {
            return it->second;
        }

        return "";
    }

    bool ObjectOptions::defaultExists() const {
        return m_defaultOption;
    }
    std::string ObjectOptions::getDefaultOptionsString() const {
        return getOptionsStringForID(DEFAULT_OPTION_ID);
    }

    void ObjectOptions::updateSimpleOptionsString(bool pDontFade, bool pDontEnter, bool pNoGlow) {
        // 64 == dont fade prop, 67 == dont enter prop, 96 == no glow prop
        m_simpleOptionsString =  fmt::format(
            ",64,{},67,{},96,{}", 
            static_cast<int>(pDontFade), 
            static_cast<int>(pDontEnter), 
            static_cast<int>(pNoGlow)
        );
    }
    std::string ObjectOptions::getSimpleOptionsString() const {
        return m_simpleOptionsString;
    }

    void ObjectOptions::reset() {
        m_options.clear();
        m_simpleOptionsString.clear();
        m_defaultOption = false;
    }
}

class $modify(DefaultObjectOptionsEditorUI, EditorUI) {
    struct Fields {
        bool creatingObject = false;
    };
    
    GameObject* createObject(int objectID, CCPoint position) {
        auto& creatingObject = m_fields->creatingObject;
        creatingObject = true;

        GameObject* ret = EditorUI::createObject(objectID, position);

        creatingObject = false;  
        
        return ret;
    }
};

class $modify(LevelEditorLayer) {
    struct Fields {
        DefaultObjectOptions::ObjectOptions options;
    };

    bool init(GJGameLevel* level, bool noUI) {
        if (!LevelEditorLayer::init(level, noUI)) return false;
        
        if (Settings::DefaultObjectOptions::useJSON.get()) {
            DefaultObjectOptions::parseOptions(m_fields->options);
        }
        else {
            m_fields->options.updateSimpleOptionsString(
                Settings::DefaultObjectOptions::dontFade.get(),
                Settings::DefaultObjectOptions::dontEnter.get(),
                Settings::DefaultObjectOptions::noGlow.get()
            );
        }

        return true;
    }

    GameObject* createObject(int key, CCPoint position, bool noUndo) {
        if (!Settings::DefaultObjectOptions::enabled.get() || noUndo || !static_cast<DefaultObjectOptionsEditorUI*>(m_editorUI)->m_fields->creatingObject) {
            return LevelEditorLayer::createObject(key, position, noUndo);
        }

        // 1 == id prop, 2 == x pos prop, 3 == y pos prop
        auto objectString = fmt::format(
            "1,{},2,{},3,{}", key, 
            position.x + Editor::OBJECT_STRING_POSITION_OFFSET.x, 
            position.y + Editor::OBJECT_STRING_POSITION_OFFSET.y
        );

        const auto& options = m_fields->options;
        
        if (Settings::DefaultObjectOptions::useJSON.get()) {
            if (options.defaultExists()) {
                objectString.append(options.getDefaultOptionsString());
            }
            if (options.idHasOptions(key)) {
                objectString.append(options.getOptionsStringForID(key));
            }
        }
        else {
            objectString.append(options.getSimpleOptionsString());
        }

        if (auto i = objectString.find(';'); i != std::string::npos) {
            objectString = objectString.substr(0, i);
        }
        
        return static_cast<GameObject*>(createObjectsFromString(objectString, noUndo, true)->firstObject());
    }
};