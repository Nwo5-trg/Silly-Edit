#pragma once

namespace nwo5::sillyedit {
    /// toggle a feature
    /// @param pFeature feature id, features listed in src/feature/registry.hpp (e.g. FloodFill has id "Flood Fill")
    /// @param pEnable whether to enable or disable the feature
    /// @param pMod what mod is disabling the feature, you can just leave this alone
    void toggleFeature(geode::ZStringView pFeature, bool pEnable, geode::Mod* pMod = geode::Mod::get());
    
    /// check if feature is enabled (accounts for enabled setting + mods disabling feature)
    /// @param pFeature feature id, features listed in src/feature/registry.hpp (e.g. FloodFill has id "Flood Fill")
    /// @returns if feature is enabled or false if feature doesnt exist
    bool featureEnabled(geode::ZStringView pFeature);
    // check if feature is force disabled (disabled by other mod(s))
    /// @param pFeature feature id, features listed in src/feature/registry.hpp (e.g. FloodFill has id "Flood Fill")
    /// @returns if feature is force disabled or false if feature doesnt exist
    bool featureForceDisabled(geode::ZStringView pFeature);
};