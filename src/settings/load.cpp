#include "include.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {    
    SettingsManager::get()->load();
}