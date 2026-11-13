#pragma once

#include "setting-base.hpp"

namespace sillyedit {
    namespace features {
        class FeatureEnum;
    }

    namespace settings {
        class SettingsManager final {
        protected:
            struct Category {
                bool initialized = false;
                int order = 0;
                std::string name;
                std::string logo;
                std::string description;
                std::vector<GenericSetting*> settings;
            };

            std::vector<Category> m_categories;

        public:
            static auto get() {
                static SettingsManager inst;
                return &inst;
            }

            std::string registerCategory(features::FeatureEnum category, std::string logo, std::string description);
            void registerSetting(geode::ZStringView category, GenericSetting* setting);
            void unregisterSetting(GenericSetting* setting);
            const std::vector<Category>& categories() const;
        };
    }
}