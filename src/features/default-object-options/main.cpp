#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <internal/utils/utils.hpp>
#include <features/shared.hpp>
#include "settings.hpp"
#include "include.hpp"

using namespace geode::prelude;

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

    static void onModify(auto& pSelf) {
        (void)pSelf.setHookPriorityPost("LevelEditorLayer::createObject", Priority::VeryEarly);
    }

    GameObject* createObject(int key, CCPoint position, bool noUndo) {
        auto ret = LevelEditorLayer::createObject(key, position, noUndo);

        if (!Settings::DefaultObjectOptions::enabled.get() || !Shared::shouldApplyCustomPlacedObjectOptions()) {
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
        
        // our father
        // who art in heaven
        // hallowed be thy name
        // thy kingdom come
        // thy will be done
        // on earth as it is in heaven
        // give us this day or daily bread
        // and forgive us our trespasses
        // as we forgive those who trespass against us
        // and lead us not into temptation
        // but deliver us from evil
        // lord forgive me for i have sinned
        editor::object::remove(ret);

        return static_cast<GameObject*>(
            createObjectsFromString(objectString, noUndo, true)->firstObject()
        );
    }
};