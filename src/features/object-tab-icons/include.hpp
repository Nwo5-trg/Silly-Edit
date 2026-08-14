#pragma once

#include <Geode/modify/EditorUI.hpp>
#include <feature/include.hpp>

namespace ObjectTabIcons {
    class $feature(ObjectTabIcons) {
        void onEditor() override;
    } feature;

    class $feature_modify(EditorUI) {};

    class $setting_category("object-tab-icons-logo.png"_spr);

    inline SillySetting<std::string> blockTabMode{
        "Block Tab Mode", feature, "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor
    };
    inline SillySetting<std::string> outlineTabMode{
        "Outline Tab Mode", feature, "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor
    };
    inline SillySetting<std::string> slopeTabMode{
        "Slope Tab Mode", feature, "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor
    };
    inline SillySetting<std::string> hazardTabMode{
        "Hazard Tab Mode", feature, "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor
    };
    inline SillySetting<std::string> threedTabMode{
        "3D Tab Mode", feature, "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor, "has inbuilt alternate texture"
    };
    inline SillySetting<std::string> portalTabMode{
        "Portal Tab Mode", feature, "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor
    };
    inline SillySetting<std::string> monsterTabMode{
        "Monster Tab Mode", feature, "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor
    };
    inline SillySetting<std::string> pixelTabMode{
        "Pixel Tab Mode", feature, "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor, "has inbuilt alternate texture"
    };
    inline SillySetting<std::string> collectibleTabMode{
        "Collectible Tab Mode", feature, "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor
    };
    inline SillySetting<std::string> iconTabMode{
        "Icon Tab Mode", feature, "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor
    };
    inline SillySetting<std::string> decoTabMode{
        "Deco Tab Mode", feature, "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor
    };
    inline SillySetting<std::string> sawbladeTabMode{
        "Sawblade Tab Mode", feature, "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor
    };
    inline SillySetting<std::string> triggerTabMode{
        "Trigger Tab Mode", feature, "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor
    };
    inline SillySetting<std::string> customTabMode{
        "Custom Tab Mode", feature, "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor, "has inbuilt alternate texture"
    };
}