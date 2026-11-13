#pragma once

#include "setting-base.hpp"

namespace sillyedit::settings {
    class SettingsManager final {
    protected:
        struct Category {
            bool initialized = false;
            std::string key;
            std::string name;
            std::string logo;
            std::string description;
            std::vector<GenericSetting*> settings;
        };

        geode::utils::StringMap<Category> m_categories;

    public:
        static auto get() {
            static SettingsManager inst;
            return &inst;
        }

        bool registerCategory(std::string name, std::string logo, std::string description);
        void registerSetting(geode::ZStringView category, GenericSetting* setting);
        void unregisterSetting(GenericSetting* setting);
        const geode::utils::StringMap<Category>& categories() const;
    };
}