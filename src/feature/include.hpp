#pragma once

#include <settings/include.hpp>
#include "feature-manager.hpp"

using sillyedit::features::FeatureEnum;
using sillyedit::features::FeatureManager;

#define $feature(feature) \
    featureDummy##__COUNTER__ ; \
    inline struct Feature final : public sillyedit::features::FeatureTemplate<FeatureEnum:: feature >

#define $feature_modify(class) \
    class##Hook##__COUNTER__; \
    namespace GD { \
        using class = :: class ;\
    } \
    struct class : geode::Modify< class , :: class>

#define SETTING_CATEGORY_IMPL_1(logo) \
    $execute{ settings::SettingsManager::get()->registerCategory(FeatureEnum::fromString(feature), std::nullopt, logo ); }
#define SETTING_CATEGORY_IMPL_2(logo, description) \
    $execute{ settings::SettingsManager::get()->registerCategory(FeatureEnum::fromString(feature), description , logo ); }
#define $setting_category(...) \
    categoryDummy##__COUNTER__ ; /* cool syntax to have class infront of setting category lol its completely unneccesary*/\
    inline bool enabled() { \
        return feature.enabled(); \
    } \
    GEODE_INVOKE(GEODE_CONCAT(SETTING_CATEGORY_IMPL_, GEODE_NUMBER_OF_ARGS(__VA_ARGS__)), __VA_ARGS__)