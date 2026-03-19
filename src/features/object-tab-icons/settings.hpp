#pragma once

#include <internal/settings.hpp>

namespace Settings::ObjectTabIcons {
    SCAPI_INLINE_CATEGORY("Object Tab Icons", std::nullopt, "object-tab-icons-logo.png"_spr, SettingCategory::ObjectTabIcons)
    inline SavedSetting<bool> enabled{"Enabled", "Object Tab Icons", true};
    
    inline SavedSetting<int> blockTabMode{"Block Tab Mode", "Object Tab Icons", 1, "set to 0 for normal gd icons, 1 for custom textures, and 2 for alternate textures"};
    inline SavedSetting<int> outlineTabMode{"Outline Tab Mode", "Object Tab Icons", 1, "set to 0 for normal gd icons, 1 for custom textures, and 2 for alternate textures"};
    inline SavedSetting<int> slopeTabMode{"Slope Tab Mode", "Object Tab Icons", 1, "set to 0 for normal gd icons, 1 for custom textures, and 2 for alternate textures"};
    inline SavedSetting<int> hazardTabMode{"Hazard Tab Mode", "Object Tab Icons", 1, "set to 0 for normal gd icons, 1 for custom textures, and 2 for alternate textures"};
    inline SavedSetting<int> threedTabMode{"3D Tab Mode", "Object Tab Icons", 1, "has inbuilt alternate texture | set to 0 for normal gd icons, 1 for custom textures, and 2 for alternate textures"};
    inline SavedSetting<int> portalTabMode{"Portal Tab Mode", "Object Tab Icons", 1, "set to 0 for normal gd icons, 1 for custom textures, and 2 for alternate textures"};
    inline SavedSetting<int> monsterTabMode{"Monster Tab Mode", "Object Tab Icons", 1, "set to 0 for normal gd icons, 1 for custom textures, and 2 for alternate textures"};
    inline SavedSetting<int> pixelTabMode{"Pixel Tab Mode", "Object Tab Icons", 1, "has inbuilt alternate texture | set to 0 for normal gd icons, 1 for custom textures, and 2 for alternate textures"};
    inline SavedSetting<int> collectibleTabMode{"Collectible Tab Mode", "Object Tab Icons", 1, "set to 0 for normal gd icons, 1 for custom textures, and 2 for alternate textures"};
    inline SavedSetting<int> iconTabMode{"Icon Tab Mode", "Object Tab Icons", 1, "set to 0 for normal gd icons, 1 for custom textures, and 2 for alternate textures"};
    inline SavedSetting<int> decoTabMode{"Deco Tab Mode", "Object Tab Icons", 1, "set to 0 for normal gd icons, 1 for custom textures, and 2 for alternate textures"};
    inline SavedSetting<int> sawbladeTabMode{"Sawblade Tab Mode", "Object Tab Icons", 1, "set to 0 for normal gd icons, 1 for custom textures, and 2 for alternate textures"};
    inline SavedSetting<int> triggerTabMode{"Trigger Tab Mode", "Object Tab Icons", 1, "set to 0 for normal gd icons, 1 for custom textures, and 2 for alternate textures"};
    inline SavedSetting<int> customTabMode{"Custom Tab Mode", "Object Tab Icons", 1, "has inbuilt alternate texture | set to 0 for normal gd icons, 1 for custom textures, and 2 for alternate textures"};
}