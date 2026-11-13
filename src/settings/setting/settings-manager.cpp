#include "settings-manager.hpp"

using namespace geode::prelude;

namespace sillyedit::settings {
    bool SettingsManager::registerCategory(std::string name, std::string logo, std::string description) {
        auto& category = m_categories[name];
        if (category.initialized) {
            return false;
        }

        category.initialized = true;
        category.key = generateKey(name);
        category.name = name;
        category.logo = logo;
        category.description = description;

        return true;
    };
    void SettingsManager::registerSetting(geode::ZStringView category, GenericSetting* setting) {
        m_categories[category].settings.push_back(setting);
    }
    void SettingsManager::unregisterSetting(GenericSetting* setting) {
        for (auto& [_, category] : m_categories) {
            auto it = std::ranges::find(category.settings, setting);

            if (it != category.settings.end()) {
                category.settings.erase(it);

                break;
            }
        }
    }
    const geode::utils::StringMap<SettingsManager::Category>& SettingsManager::categories() const {
        return m_categories;
    }
}