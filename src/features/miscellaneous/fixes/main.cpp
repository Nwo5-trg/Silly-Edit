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

    void EditorUI::keyDown(cocos2d::enumKeyCodes key, double timestamp) {
        if (Fixes::allowShiftChangeModes && CCKeyboardDispatcher::get()->getShiftKeyPressed()) {
            if (key == enumKeyCodes::KEY_One) {
                toggleMode(m_buildModeBtn);
                return;
            }
            if (key == enumKeyCodes::KEY_Two) {
                toggleMode(m_editModeBtn);
                return;
            }
            if (key == enumKeyCodes::KEY_Three) {
                toggleMode(m_deleteModeBtn);
                return;
            }
        }

        GD::EditorUI::keyDown(key, timestamp);
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