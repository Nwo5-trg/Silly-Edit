#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <internal/utils/utils.hpp>
#include "settings.hpp"

using namespace geode::prelude;

class $modify(EditorUI) {
    void selectObject(GameObject* object, bool ignoreFilter) {
        EditorUI::selectObject(object, ignoreFilter);

        if (Settings::Miscellaneous::fixes.get()) {
            updateObjectInfoLabel();
        }
    } 
};

class $modify(GJBaseGameLayer) {
    struct Fields {
        bool loading = false;
    };
    
    void loadUpToPosition(float position, int order, int channel) {
        auto fields = m_fields.self();

        fields->loading = true;
        GJBaseGameLayer::loadUpToPosition(position, order, channel);
        fields->loading = false;
    }

    void processAreaEffects(gd::vector<EnterEffectInstance>* effects, GJAreaActionType type, float dt, bool visibleFrame) {
        if (Settings::Miscellaneous::fixes.get() && m_fields->loading) {
            return;
        }

        GJBaseGameLayer::processAreaEffects(effects, type, dt, visibleFrame);
    }
};

class $modify(PlayerObject) {
    void collidedWithSlopeInternal(float dt, GameObject* object, bool forced) {
        if (Settings::Miscellaneous::fixes.get() && m_isDart && m_ignoreDamage && m_stateDartSlide <= 0) {
            return;
        }

        PlayerObject::collidedWithSlopeInternal(dt, object, forced);
    }
};