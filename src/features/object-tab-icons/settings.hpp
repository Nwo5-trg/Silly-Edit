#pragma once

#include <internal/settings.hpp>

namespace Settings::ObjectTabIcons {
    SE_CATEGORY("Object Tab Icons", "object-tab-icons-logo.png"_spr, SettingCategory::ObjectTabIcons)
    SE_SETTING("Object Tab Icons", "Enabled", bool, enabled, true)
    
    SE_SETTING("Object Tab Icons", "Block Tab Mode", int, blockTabMode, 1, "set to 0 for normal gd icons, 1 for custom textures, and 2 for alternate textures")
    SE_SETTING("Object Tab Icons", "Outline Tab Mode", int, outlineTabMode, 1, "set to 0 for normal gd icons, 1 for custom textures, and 2 for alternate textures")
    SE_SETTING("Object Tab Icons", "Slope Tab Mode", int, slopeTabMode, 1, "set to 0 for normal gd icons, 1 for custom textures, and 2 for alternate textures")
    SE_SETTING("Object Tab Icons", "Hazard Tab Mode", int, hazardTabMode, 1, "set to 0 for normal gd icons, 1 for custom textures, and 2 for alternate textures")
    SE_SETTING("Object Tab Icons", "3D Tab Mode", int, threedTabMode, 1, "has inbuilt alternate texture | set to 0 for normal gd icons, 1 for custom textures, and 2 for alternate textures")
    SE_SETTING("Object Tab Icons", "Portal Tab Mode", int, portalTabMode, 1, "set to 0 for normal gd icons, 1 for custom textures, and 2 for alternate textures")
    SE_SETTING("Object Tab Icons", "Monster Tab Mode", int, monsterTabMode, 1, "set to 0 for normal gd icons, 1 for custom textures, and 2 for alternate textures")
    SE_SETTING("Object Tab Icons", "Pixel Tab Mode", int, pixelTabMode, 1, "has inbuilt alternate texture | set to 0 for normal gd icons, 1 for custom textures, and 2 for alternate textures")
    SE_SETTING("Object Tab Icons", "Collectible Tab Mode", int, collectibleTabMode, 1, "set to 0 for normal gd icons, 1 for custom textures, and 2 for alternate textures")
    SE_SETTING("Object Tab Icons", "Icon Tab Mode", int, iconTabMode, 1, "set to 0 for normal gd icons, 1 for custom textures, and 2 for alternate textures")
    SE_SETTING("Object Tab Icons", "Deco Tab Mode", int, decoTabMode, 1, "set to 0 for normal gd icons, 1 for custom textures, and 2 for alternate textures")
    SE_SETTING("Object Tab Icons", "Sawblade Tab Mode", int, sawbladeTabMode, 1, "set to 0 for normal gd icons, 1 for custom textures, and 2 for alternate textures")
    SE_SETTING("Object Tab Icons", "Trigger Tab Mode", int, triggerTabMode, 1, "set to 0 for normal gd icons, 1 for custom textures, and 2 for alternate textures")
    SE_SETTING("Object Tab Icons", "Custom Tab Mode", int, customTabMode, 1, "has inbuilt alternate texture | set to 0 for normal gd icons, 1 for custom textures, and 2 for alternate textures")
}