#include "include.hpp"

using namespace geode::prelude;

namespace DefaultObjectOptions {
    GameObject* LevelEditorLayer::createObject(int key, CCPoint position, bool noUndo) {
        if (!DefaultObjectOptions::enabled() || !sillyedit::utils::shouldApplyCustomPlacedObjectOptions()) {
            return GD::LevelEditorLayer::createObject(key, position, noUndo);
        }

        const auto& options = m_fields->options;

        auto objectString = fmt::format(
            "1,{},2,{},3,{}{}", key, position.x, position.y + editor::constants::OBJECT_STRING_POSITION_OFFSET.y, options.getSimpleOptionsString()
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

        auto obj = static_cast<GameObject*>(this->createObjectsFromString(objectString, noUndo, true)->firstObject());

        if (!obj) {
            return nullptr;
        }

        // for some reason some of the effectgameobject setup stuff that is set in init isnt being set with createobjectsfromstring
        // (but also some other stuff in init like classtype is being set ???? im so fucking confused)
        // meaning triggers dont get place with a default m_duration = 0.5f and stuff, so im just gonna reimplement that here

        if (obj->m_classType == GameObjectClassType::Effect) {
            auto trigger = static_cast<EffectGameObject*>(obj);

            trigger->m_bUnkBool2 = false;
            trigger->m_triggerTargetColor = ccColor3B{255, 255, 255};
            trigger->m_duration = 0.5f;
            trigger->m_legacyHSV = true;
        }
        
        return obj;
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