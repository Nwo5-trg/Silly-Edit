#pragma once

#include <settings/silly-setting.hpp>

// in my defense, i need an enum for TITLE order, but also this is a very silly method of defining a feature as a title :sob:

// ------------------------------------------

#define SILLYEDIT_FEATURE_LIST \
    TITLE(Core) \
        FEATURE(General) \
    TITLE(Tools) \
        FEATURE(Ruler) \
        FEATURE(FloodFill) \
        FEATURE(ZoomInput) \
        FEATURE(BetterSelectAll) \
    TITLE(Interface) \
        FEATURE(BetterScale) \
        FEATURE(BetterLayers) \
        FEATURE(ObjectTabIcons) \
        FEATURE(HideUIToggle) \
        FEATURE(UI) \
    TITLE(Utility) \
        FEATURE(DefaultObjectOptions) \
        FEATURE(SelectionUtils) \
        FEATURE(SetupStartpos) \
        FEATURE(TextObjectUtils) \
    TITLE(Miscellaneous) \
        FEATURE(SillyKeybinds) \
        FEATURE(Fixes) \
        FEATURE(HideWithPlaytest) \
        FEATURE(PlaceObjectPreview) \
        FEATURE(CopyObjectStrings) \
        FEATURE(Template) \
    
// ------------------------------------------

// actual stuff

namespace Features {
    enum class FeatureEnum {
        #define FEATURE(pFeature) pFeature ,
        #define TITLE(pTitle) GEODE_CONCAT(pTitle , Title),
            SILLYEDIT_FEATURE_LIST
        #undef TITLE
        #undef FEATURE
    };
    
    inline std::string formatID(std::string_view pStr) {
        if (pStr.empty()) {
            return {};
        }

        std::string out{pStr.front()};

        for (const auto c : pStr.substr(1)) {
            if (std::isupper(c) && !std::isupper(out.back())) {
                out.push_back(' ');
            }
            
            out.push_back(c);
        }

        return out;
    }

    inline geode::ZStringView getID(FeatureEnum pEnum) {
        static const std::string array[] = {
            #define FEATURE(pFeature) formatID(#pFeature) ,
            #define TITLE(pTitle) fmt::format("{}Title", formatID ( #pTitle )),
                SILLYEDIT_FEATURE_LIST
            #undef TITLE
            #undef FEATURE
        };

        return array[static_cast<size_t>(pEnum)];
    }
    inline std::optional<FeatureEnum> getEnum(std::string_view pID) {
        #define FEATURE(pFeature) static const auto pFeature = formatID(#pFeature);
        #define TITLE(pTitle) static const auto GEODE_CONCAT( pTitle , Title) = fmt::format("{}Title", formatID( #pTitle ));
            SILLYEDIT_FEATURE_LIST
        #undef TITLE
        #undef FEATURE

        #define FEATURE(pFeature) if (pID == pFeature ) return FeatureEnum:: pFeature ;
        #define TITLE(pTitle) if (pID == GEODE_CONCAT( pTitle , Title) ) return FeatureEnum:: GEODE_CONCAT( pTitle , Title) ;
            SILLYEDIT_FEATURE_LIST
        #undef TITLE
        #undef FEATURE
        
        return std::nullopt;
    }

    #define FEATURE(pFeature)
    #define TITLE(pTitle) SILLY_API_INLINE_CATEGORY(fmt::format("{}Title", formatID( #pTitle )), std::nullopt, "", FeatureEnum :: GEODE_CONCAT( pTitle , Title));
        SILLYEDIT_FEATURE_LIST
    #undef TITLE
    #undef FEATURE
};

#undef SILLYEDIT_FEATURE_LIST