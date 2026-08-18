#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;

namespace DefaultObjectOptions {
    void LevelEditorLayer::onModify(auto& pSelf) {
        (void)pSelf.setHookPriorityPost("LevelEditorLayer::createObject", Priority::Replace);
    }



    GameObject* LevelEditorLayer::createObject(int key, CCPoint position, bool noUndo) {
        auto ret = GD::LevelEditorLayer::createObject(key, position, noUndo);

        if (!DefaultObjectOptions::enabled() || !Shared::shouldApplyCustomPlacedObjectOptions()) {
            return ret;
        }

        const auto& options = m_fields->options;

        auto objectString = fmt::format(
            "{}{}", ret->getSaveString(this), options.getSimpleOptionsString()
        );
        
        if (DefaultObjectOptions::useJSON) {
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
            this->createObjectsFromString(objectString, noUndo, true)->firstObject()
        );
    }

    



    void Feature::onEditor() {
        auto self = editor::layer<DefaultObjectOptions::LevelEditorLayer>();
        auto fields = self->m_fields.self();
        
        DefaultObjectOptions::parseOptions(fields->options);

        fields->options.updateSimpleOptionsString(
            DefaultObjectOptions::dontFade,
            DefaultObjectOptions::dontEnter,
            DefaultObjectOptions::noGlow
        );
    }

    void Feature::onSettingChanged(std::string pName, GenericSetting*) {
        auto self = editor::layer<DefaultObjectOptions::LevelEditorLayer>();

        if (pName == "Dont Fade" || pName == "Dont Enter" || pName == "No Glow") {
            self->m_fields->options.updateSimpleOptionsString(
                DefaultObjectOptions::dontFade,
                DefaultObjectOptions::dontEnter,
                DefaultObjectOptions::noGlow
            );
        }
        else if (pName == "Use JSON") {
            DefaultObjectOptions::parseOptions(self->m_fields->options);
        }
    }
}