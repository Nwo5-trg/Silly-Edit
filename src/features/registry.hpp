#pragma once

#include <settings/setting/include.hpp>

// in my defense, i need an enum for TITLE order, but also this is a very silly method of defining a feature as a title :sob:

// ------------------------------------------

#define SILLYEDIT_FEATURE_LIST \
    TITLE(Core) \
        FEATURE(General) \
        FEATURE(Popup) \
    TITLE(Tools) \
        FEATURE(Ruler) \
        FEATURE(FloodFill) \
        FEATURE(ZoomInput) \
        FEATURE(ContextMenu) \
        FEATURE(BetterSelectAll) \
    TITLE(Interface) \
        FEATURE(BetterScale) \
        FEATURE(EditorPositionBar) \
        FEATURE(BetterLayers) \
        FEATURE(ObjectTabIcons) \
        FEATURE(BetterEditMenu) \
        FEATURE(HideUI) \
    TITLE(Utility) \
        FEATURE(DefaultObjectOptions) \
        FEATURE(SelectionUtils) \
        FEATURE(EasingPreview) \
        FEATURE(SetupStartpos) \
        FEATURE(TextObjectUtils) \
        FEATURE(ScrollGroups) \
    TITLE(Overlay) \
        FEATURE(TriggerIndicators) \
        FEATURE(GroupLabelShenanigans) \
        FEATURE(TriggerTypeBoxes) \
        FEATURE(PlaceObjectPreview) \
    TITLE(Miscellaneous) \
        FEATURE(SillyKeybinds) \
        FEATURE(EditorTime) \
        FEATURE(Fixes) \
        FEATURE(HideWithPlaytest) \
        FEATURE(CopyObjectStrings) \
        FEATURE(Template) \
    
// ------------------------------------------

// actual stuff

namespace sillyedit::features {
    struct FeatureEnum {
    protected:
        static std::string formatID(std::string_view str) {
            if (str.empty()) {
                return "";
            }

            std::string out{str.front()};

            for (const auto c : str.substr(1)) {
                if (std::isupper(c) && !std::isupper(out.back())) {
                    out.push_back(' ');
                }
                
                out.push_back(c);
            }

            return out;
        }

        
    public:
        enum class Order : size_t {
            #define FEATURE(feature) feature ,
            #define TITLE(title) GEODE_CONCAT(title , Title),
                SILLYEDIT_FEATURE_LIST
            #undef TITLE
            #undef FEATURE
        };

        int order;

        constexpr FeatureEnum(Order val)
            : order(std::to_underlying(val)) {}

        #define FEATURE(feature) static const FeatureEnum feature;
        #define TITLE(title) static const FeatureEnum GEODE_CONCAT(title , Title);
            SILLYEDIT_FEATURE_LIST
        #undef TITLE
        #undef FEATURE

        static const std::string& toString(FeatureEnum val) {
            static const std::string array[] = {
                #define FEATURE(feature) formatID(#feature) ,
                #define TITLE(title) fmt::format("{}Title", formatID ( #title )),
                    SILLYEDIT_FEATURE_LIST
                #undef TITLE
                #undef FEATURE
            };

            return array[val.order];
        }
        const std::string& toString() const {
            return toString(*this);
        }

        static std::optional<FeatureEnum> fromString(std::string_view iD) {
            #define FEATURE(feature) if (iD == feature ) return FeatureEnum:: feature ;
            #define TITLE(title) if (iD == GEODE_CONCAT( title , Title) ) return FeatureEnum:: GEODE_CONCAT( title , Title) ;
                SILLYEDIT_FEATURE_LIST
            #undef TITLE
            #undef FEATURE
            
            return std::nullopt;
        }

        operator int() const {
            return order;
        }
        operator std::string_view() const {
            return toString();
        }
        operator const std::string&() const {
            return toString();
        }
    };

    #define FEATURE(feature) inline constexpr FeatureEnum feature = FeatureEnum::Order:: feature ;
    #define TITLE(title) inline constexpr FeatureEnum GEODE_CONCAT(title , Title) = FeatureEnum::Order:: GEODE_CONCAT(title , Title);
        SILLYEDIT_FEATURE_LIST
    #undef TITLE
    #undef FEATURE

    $execute {
        #define FEATURE(feature)
        #define TITLE(title) settings::SettingsManager::get()->registerCategory(FeatureEnum:: GEODE_CONCAT( title , Title), "", "");
            SILLYEDIT_FEATURE_LIST
        #undef TITLE
        #undef FEATURE
    }
};

#undef SILLYEDIT_FEATURE_LIST