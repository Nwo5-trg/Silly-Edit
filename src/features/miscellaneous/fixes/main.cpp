#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;

namespace Fixes {
    void EditorUI::selectObject(GameObject* object, bool ignoreFilter) {
        GD::EditorUI::selectObject(object, ignoreFilter);

        if (Fixes::enabled() && Fixes::fixObjectInfoLabel) {
            this->updateObjectInfoLabel();
        }
    } 





    void GJBaseGameLayer::loadUpToPosition(float position, int order, int channel) {
        auto fields = m_fields.self();

        fields->loading = true;
        GD::GJBaseGameLayer::loadUpToPosition(position, order, channel);
        fields->loading = false;
    }

    void GJBaseGameLayer::processAreaEffects(gd::vector<EnterEffectInstance>* effects, GJAreaActionType type, float dt, bool visibleFrame) {
        if (Fixes::enabled() && Fixes::fixAreaCorruption && m_fields->loading) {
            return;
        }

        GD::GJBaseGameLayer::processAreaEffects(effects, type, dt, visibleFrame);
    }





    void PlayerObject::collidedWithSlopeInternal(float dt, GameObject* object, bool forced) {
        if (Fixes::enabled() && Fixes::fixIgnoreDamageWave && m_isDart && m_ignoreDamage && m_stateDartSlide <= 0) {
            return;
        }

        GD::PlayerObject::collidedWithSlopeInternal(dt, object, forced);
    }
}