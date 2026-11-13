#pragma once

#include <Geode/modify/EditorUI.hpp>
#include <feature/include.hpp>

namespace ObjectTabIcons {
    class $feature(ObjectTabIcons) {
        void setup() override {
            m_data.reload = SettingReload::Editor;
        }

        void onEditor() override;
    } feature;

    class $feature_modify(EditorUI) {};

    class $setting_category("object-tab-icons-logo.png"_spr, "Retexture object tab icons cuz wat was rob actually doing");

    inline Setting<std::string> blockTabMode{
        "Block Tab Mode", feature, "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor
    };
    inline Setting<std::string> outlineTabMode{
        "Outline Tab Mode", feature, "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor
    };
    inline Setting<std::string> slopeTabMode{
        "Slope Tab Mode", feature, "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor
    };
    inline Setting<std::string> hazardTabMode{
        "Hazard Tab Mode", feature, "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor
    };
    inline Setting<std::string> threedTabMode{
        "3D Tab Mode", feature, "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor, "has inbuilt alternate texture"
    };
    inline Setting<std::string> portalTabMode{
        "Portal Tab Mode", feature, "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor
    };
    inline Setting<std::string> monsterTabMode{
        "Monster Tab Mode", feature, "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor
    };
    inline Setting<std::string> pixelTabMode{
        "Pixel Tab Mode", feature, "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor, "has inbuilt alternate texture"
    };
    inline Setting<std::string> collectibleTabMode{
        "Collectible Tab Mode", feature, "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor
    };
    inline Setting<std::string> iconTabMode{
        "Icon Tab Mode", feature, "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor
    };
    inline Setting<std::string> decoTabMode{
        "Deco Tab Mode", feature, "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor
    };
    inline Setting<std::string> sawbladeTabMode{
        "Sawblade Tab Mode", feature, "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor
    };
    inline Setting<std::string> triggerTabMode{
        "Trigger Tab Mode", feature, "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor
    };
    inline Setting<std::string> customTabMode{
        "Custom Tab Mode", feature, "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor, "has inbuilt alternate texture"
    };
}