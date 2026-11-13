#pragma once

namespace nwo5::sillyedit {
    /// toggle a feature
    /// @param feature feature id, features listed in src/feature/registry.hpp (e.g. FloodFill has id "Flood Fill")
    /// @param enable whether to enable or disable the feature
    /// @param mod what mod is disabling the feature, you can just leave this alone
    void toggleFeature(geode::ZStringView feature, bool enable, geode::Mod* mod = geode::Mod::get());
    
    /// check if feature is enabled (accounts for enabled setting + mods disabling feature)
    /// @param feature feature id, features listed in src/feature/registry.hpp (e.g. FloodFill has id "Flood Fill")
    /// @returns if feature is enabled or false if feature doesnt exist
    bool featureEnabled(geode::ZStringView feature);
    // check if feature is force disabled (disabled by other mod(s))
    /// @param feature feature id, features listed in src/feature/registry.hpp (e.g. FloodFill has id "Flood Fill")
    /// @returns if feature is force disabled or false if feature doesnt exist
    bool featureForceDisabled(geode::ZStringView feature);
};