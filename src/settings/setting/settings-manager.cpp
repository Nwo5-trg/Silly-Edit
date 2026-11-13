#include <features/registry.hpp>

using namespace geode::prelude;

namespace sillyedit::settings {
    std::string SettingsManager::registerCategory(features::FeatureEnum category, std::string logo, std::string description) {
        auto it = std::ranges::find_if(m_categories, [&] (const auto& c) { return c.name == category.toString(); });

        if (it != m_categories.end()) {
            if (it->initialized) {
                return "";
            }

            it->initialized = true;
            it->order = category;
            it->name = category;
            it->logo = logo;
            it->description = description;
        }
        else {
            m_categories.emplace_back(true, category, category, logo, description);
        }

        std::ranges::sort(m_categories, [] (const auto& a, const auto& b) { return a.order < b.order; });

        return category;
    };
    void SettingsManager::registerSetting(geode::ZStringView category, GenericSetting* setting) {
        auto it = std::ranges::find_if(m_categories, [&] (const auto& c) { return c.name == category; });

        if (it != m_categories.end()) {
            it->settings.push_back(setting);
        }
        else {
            m_categories.emplace_back().settings.push_back(setting);

            std::ranges::sort(m_categories, [] (const auto& a, const auto& b) { return a.order < b.order; });
        }
    }
    void SettingsManager::unregisterSetting(GenericSetting* setting) {
        for (auto& category : m_categories) {
            if (std::erase(category.settings, setting)) {
                break;
            }
        }
    }
    const std::vector<SettingsManager::Category>& SettingsManager::categories() const {
        return m_categories;
    }
}