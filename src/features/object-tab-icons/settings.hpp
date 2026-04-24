#pragma once

#include <internal/settings.hpp>

namespace Settings::ObjectTabIcons {
    SILLY_API_INLINE_CATEGORY("Object Tab Icons", std::nullopt, "object-tab-icons-logo.png"_spr, SettingCategory::ObjectTabIcons)
    inline SillySetting<bool> enabled{"Enabled", "Object Tab Icons", true, SettingReload::Editor};
    
    inline SillySetting<std::string> blockTabMode{"Block Tab Mode", "Object Tab Icons", "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor};
    inline SillySetting<std::string> outlineTabMode{"Outline Tab Mode", "Object Tab Icons", "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor};
    inline SillySetting<std::string> slopeTabMode{"Slope Tab Mode", "Object Tab Icons", "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor};
    inline SillySetting<std::string> hazardTabMode{"Hazard Tab Mode", "Object Tab Icons", "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor};
    inline SillySetting<std::string> threedTabMode{"3D Tab Mode", "Object Tab Icons", "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor, "has inbuilt alternate texture"};
    inline SillySetting<std::string> portalTabMode{"Portal Tab Mode", "Object Tab Icons", "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor};
    inline SillySetting<std::string> monsterTabMode{"Monster Tab Mode", "Object Tab Icons", "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor};
    inline SillySetting<std::string> pixelTabMode{"Pixel Tab Mode", "Object Tab Icons", "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor, "has inbuilt alternate texture"};
    inline SillySetting<std::string> collectibleTabMode{"Collectible Tab Mode", "Object Tab Icons", "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor};
    inline SillySetting<std::string> iconTabMode{"Icon Tab Mode", "Object Tab Icons", "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor};
    inline SillySetting<std::string> decoTabMode{"Deco Tab Mode", "Object Tab Icons", "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor};
    inline SillySetting<std::string> sawbladeTabMode{"Sawblade Tab Mode", "Object Tab Icons", "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor};
    inline SillySetting<std::string> triggerTabMode{"Trigger Tab Mode", "Object Tab Icons", "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor};
    inline SillySetting<std::string> customTabMode{"Custom Tab Mode", "Object Tab Icons", "Custom", {"Default", "Custom", "Alt"}, SettingReload::Editor, "has inbuilt alternate texture"};
}