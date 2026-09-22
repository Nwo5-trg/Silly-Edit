#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <feature/include.hpp>

namespace ScrollGroups {
    class $feature(ScrollGroups, SettingCondition::DesktopOnly) {
        void onEditor() override;
    } feature;

    class $feature_modify(EditorUI) {
        struct Fields {
            asp::time::Instant lastScroll = asp::time::Instant::now();
            
            float scrollingDistance = 0.0f;
        };

        void scrollGroup(GameObject* pObj, bool pUp, bool pSecondaryGroup);
    };

    class $setting_category("scroll-groups-logo.png"_spr, "Scroll groups while holding a modifier (shift by default), configurable in keybinds");

    inline SillySetting<sillyedit::settings::Modifier> modifier{
        "Modifier", feature, "Shift", sillyedit::settings::modifierSettingOptions()
    };
    inline SillySetting<float> scrollSensitivity{
        "Scroll Sensitivity", feature, 1.0f, {0.1f, 10.0f}, "to scroll a group, you must scroll 2.5 / [scroll sensitivity] units"
    };
    inline SillySetting<bool> reverseScroll{
        "Reverse Scroll", feature, false
    };
    inline SillySetting<sillyedit::settings::Modifier> secondaryScroll{
        "Secondary Scroll", feature, "Ctrl+Command", sillyedit::settings::modifierSettingOptions(), "hold to scroll center group instead"
    };
    inline SillySetting<int> scrollTimeout{
        "Scroll Timeout", feature, 250, {10, 10000}, "reset scrolling distance if mouse not scrolled for this many milliseconds, u prolly wont need to touch this"
    };
}