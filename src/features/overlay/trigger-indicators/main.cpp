#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

namespace TriggerIndicators {
    void LevelEditorLayer::updateDebugDraw() {
        GD::LevelEditorLayer::updateDebugDraw();

        auto fields = m_fields.self();

        if (TriggerIndicators::enabled() && fields->m_drawer) {
            fields->m_drawer->draw();
        }
    }





    void Feature::onEditor() {
        auto self = editor::layer<TriggerIndicators::LevelEditorLayer>();
        auto fields = self->m_fields.self();

        fields->m_drawer = std::make_unique<Drawer>();
        fields->m_drawer->updateBlacklist();
    }

    void Feature::onSettingChanged(std::string pName, GenericSetting*) {
        auto self = editor::layer<TriggerIndicators::LevelEditorLayer>();
        auto fields = self->m_fields.self();

        if (fields->m_drawer) {
            fields->m_drawer->updateBlacklist();
        }
    }
}