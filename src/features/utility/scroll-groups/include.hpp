#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <feature/include.hpp>

namespace ScrollGroups {
    class $feature(ScrollGroups, SettingCondition::DesktopOnly) {
        void onEditor() override;
    } feature;

    constexpr float DEFAULT_SCROLL_DISTANCE = 2.5f;

    class $feature_modify(EditorUI) {
        struct Fields {
            asp::time::Instant lastScroll = asp::time::Instant::now();
            
            float scrollingDistance = 0.0f;

            bool modifierDown = false;
            bool secondaryGroupDown = false;
        };

        void scrollGroup(GameObject* pObj, bool pUp, bool pSecondaryGroup);
    };

    class $setting_category("scroll-groups-logo.png"_spr, "Scroll groups while holding a modifier, configurable in keybinds");

    inline SillySetting<float> scrollSensitivity{
        "Scroll Sensitivity", feature, 1.0f, {0.1f, 10.0f}, "to scroll a group, you must scroll 10 / [scroll sensitivity] points"
    };
    inline SillySetting<bool> reverseScroll{
        "Reverse Scroll", feature, false
    };
    inline SillySetting<int> scrollTimeout{
        "Scroll Timeout", feature, 250, {10, 10000}, "reset scrolling distance if mouse not scrolled for this many milliseconds, u prolly wont need to touch this"
    };
}