#pragma once

// in my defense, i need an enum for category order

// ------------------------------------------

#define SILLYEDIT_FEATURE_LIST \
    X(General) \
    X(Keybinds) \
    X(DefaultObjectOptions) \
    X(Ruler) \
    X(FloodFill) \
    X(SetupStartpos) \
    X(BetterScale) \
    X(ReplaceObjects) \
    X(SelectionUtils) \
    X(BetterLayers) \
    X(HideWithPlaytest) \
    X(TextObjectUtils) \
    X(ZoomInput) \
    X(CopyPasteObjectStrings) \
    X(BetterSelectAll) \
    X(ObjectTabIcons) \
    X(UI) \
    X(Miscellaneous) \
    
// ------------------------------------------

// actual stuff

namespace Features {
    enum class FeatureEnum {
        #define X(pFeature) pFeature,
            SILLYEDIT_FEATURE_LIST
        #undef X
    };
    

    inline geode::ZStringView getID(FeatureEnum pEnum) {
        static const geode::ZStringView array[] = {
            #define X(pFeature) #pFeature ,
                SILLYEDIT_FEATURE_LIST
            #undef X
        };

        return array[static_cast<size_t>(pEnum)];
    }
    inline std::optional<FeatureEnum> getEnum(std::string_view pID) {
        const auto getID = [] (std::string_view pStr) -> std::string {
            if (pStr.empty()) {
                return {};
            }

            std::string out{pStr.front()};

            for (const auto c : pStr.substr(1)) {
                if (std::isalpha(c) && !std::isalpha(out.back())) {
                    out.push_back(' ');
                }
                
                out.push_back(c);
            }

            return out;
        };

        #define X(pFeature) static const auto pFeature = getID(#pFeature);
            SILLYEDIT_FEATURE_LIST
        #undef X

        #define X(pFeature) if (pID == pFeature ) return FeatureEnum:: pFeature ;
            SILLYEDIT_FEATURE_LIST
        #undef X
        
        return std::nullopt;
    }
};

#undef SILLYEDIT_FEATURE_LIST