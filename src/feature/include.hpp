#pragma once

#include <settings/include.hpp>
#include "feature-manager.hpp"

using sillyedit::features::FeatureEnum;
using sillyedit::features::FeatureManager;

#define FEATURE_IMPL_1(pFeature) \
    inline struct Feature final : public sillyedit::features::FeatureTemplate<FeatureEnum:: pFeature >
#define FEATURE_IMPL_2(pFeature, pCondition) \
    inline struct Feature final : public sillyedit::features::FeatureTemplate<FeatureEnum:: pFeature , pCondition >
#define FEATURE_IMPL_3(pFeature, pCondition, pReload) \
    inline struct Feature final : public sillyedit::features::FeatureTemplate<FeatureEnum:: pFeature , pCondition , pReload  >
#define FEATURE_IMPL_4(pFeature, pCondition, pReload, pDefaultEnabled) \
    inline struct Feature final : public sillyedit::features::FeatureTemplate<FeatureEnum:: pFeature , pCondition , pReload , pDefaultEnabled >
#define $feature(...) \
    featureDummy##__COUNTER__ ; \
    GEODE_INVOKE(GEODE_CONCAT(FEATURE_IMPL_, GEODE_NUMBER_OF_ARGS(__VA_ARGS__)), __VA_ARGS__)

#define $feature_modify(pClass) \
    pClass##Hook##__COUNTER__; \
    namespace GD { \
        using pClass = :: pClass ;\
    } \
    struct pClass : geode::Modify< pClass , :: pClass>

#define SETTING_CATEGORY_IMPL_1(pLogo) \
    SILLY_API_INLINE_CATEGORY(feature, std::nullopt, pLogo , sillyedit::features::getEnum(feature).value())
#define SETTING_CATEGORY_IMPL_2(pLogo, pDescription) \
    SILLY_API_INLINE_CATEGORY(feature, pDescription , pLogo , sillyedit::features::getEnum(feature).value())
#define $setting_category(...) \
    categoryDummy##__COUNTER__ ; /* cool syntax to have class infront of setting category lol its completely unneccesary*/\
    inline bool enabled() { \
        return feature.enabled(); \
    } \
    GEODE_INVOKE(GEODE_CONCAT(SETTING_CATEGORY_IMPL_, GEODE_NUMBER_OF_ARGS(__VA_ARGS__)), __VA_ARGS__)