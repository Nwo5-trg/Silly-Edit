// smth smth optimization, if someone wants to pr a reimpl of updatevisibility this coudl b even faster
// but no way im reverse engineering it to avoid the extra hook

// btw looping thru every object again instead of hooking setopacity itself is actually moderately slower ! so

#include <features/better-layers/settings.hpp>
#include <features/better-layers/include.hpp>
#include <features/hide-with-playtest/settings.hpp>
#include <features/hide-with-playtest/include.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/GameObject.hpp>
#include <internal/utils/utils.hpp>

using namespace geode::prelude;

enum class SetOpacityHookState {
    ShouldntHook = 0,
    UpdatingVisibility = 1,
    Playtesting = 2
};

static auto& getSetOpacityHookState() {
    static SetOpacityHookState val = SetOpacityHookState::ShouldntHook;
    return val;
}

class $modify(LevelEditorLayer) {
    void updateVisibility(float dt) {
        getSetOpacityHookState() = editor::isPlaytesting() ? SetOpacityHookState::Playtesting : SetOpacityHookState::UpdatingVisibility;
        
        LevelEditorLayer::updateVisibility(dt);
        
        getSetOpacityHookState() = SetOpacityHookState::ShouldntHook;
    }
};

class $modify(GameObject) {
    void setOpacity(unsigned char opacity) {
        const auto state = getSetOpacityHookState();

        if (state == SetOpacityHookState::ShouldntHook) {
            return GameObject::setOpacity(opacity);
        }

        if (state == SetOpacityHookState::Playtesting && Settings::HideWithPlaytest::enabled.get()) {
            opacity = HideWithPlaytest::opacityForObject(opacity, this);
        }
        else if (Settings::BetterLayers::enabled.get()) {
            if (auto ptr = BetterLayers::getLayerSettingsPtr()) {
                opacity = ptr->opacityForObject(opacity, this);
            }
        }

        GameObject::setOpacity(opacity);
    }
};