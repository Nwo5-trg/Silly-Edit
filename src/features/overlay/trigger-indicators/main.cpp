#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

namespace TriggerIndicators {
    void Feature::onEditor() {
        auto self = editor::layer<TriggerIndicators::LevelEditorLayer>();
        auto fields = self->m_fields.self();

        self->addEventListener(ObjectsChangedEvent(), [self] () {
            auto fields = self->m_fields.self();

            if (TriggerIndicators::enabled() && fields->m_drawer) {
                fields->m_drawer->updateTriggerList();
            }
        });
    }

    void Feature::onUpdate() {
        if (!TriggerIndicators::enabled()) {
            return;
        }

        auto self = editor::layer<TriggerIndicators::LevelEditorLayer>();
        auto fields = self->m_fields.self();

        if (fields->m_drawer) {
            fields->m_drawer->draw();
        }
    }

    void Feature::onToggled(bool pEnabled) {
        auto self = editor::layer<TriggerIndicators::LevelEditorLayer>();
        auto fields = self->m_fields.self();
        
        if (!pEnabled) {
            fields->m_drawer = nullptr;

            return;
        }
        
        fields->m_drawer = std::make_unique<Drawer>();
        fields->m_drawer->updateBlacklist();
        fields->m_drawer->updateTriggerList();
    }

    void Feature::onSettingChanged(std::string pName, GenericSetting*) {
        auto self = editor::layer<TriggerIndicators::LevelEditorLayer>();
        auto fields = self->m_fields.self();

        if (fields->m_drawer) {
            fields->m_drawer->updateBlacklist();
            fields->m_drawer->updateTriggerList();
        }
    }
}