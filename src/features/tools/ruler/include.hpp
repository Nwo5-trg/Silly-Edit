#pragma once

#include <Geode/modify/EditorUI.hpp>
#include <feature/include.hpp>
#include "utils.hpp"

namespace Ruler {
    class $feature(Ruler) {
        void onEditor() override;
        void onToggled(bool pEnabled) override;
        void onUpdate() override;
    } feature;

    class $feature_modify(EditorUI) {
        struct Fields {
            std::vector<Measurement> measurements;
        };

        MeasurementColor getMeasurementColor();
        std::string getMeasurementString(float pMeasure);
        geode::Label* createMeasurementLabel(float pMeasure);
        void createMeasurement();
        void deleteMeasurement(bool pDeleteAll);
    };

    class $setting_category("ruler-logo.png"_spr, "Measure an area with an overcomplicated overlay (now with rainbow)");

    inline SillySetting<bool> useGDUnits{
        "Use GD Units", feature, true, "one block = 10 gd units = 30 units, if enabled will show 45 units as '1, 5'"
    };
    inline SillySetting<bool> editorTabButton{
        "Editor Tab Button", feature, true, SettingReload::Editor
    };
    inline SillySetting<float> thickness{
        "Thickness", feature, 2.5f, {0.0f, std::nullopt}
    };
    inline SillySetting<bool> showCenter{
        "Show Center", feature, true
    };
    inline SillySetting<float> centerSize{
        "Center Size", feature, 5.0f, {0.0f, std::nullopt}
    };
    inline SillySetting<bool> scaleWithZoom{
        "Scale With Zoom", feature, false
    };
    inline SillySetting<float> padding{
        "Padding", feature, 0.0f
    };
    inline SillySetting<int> fillOpacity{
        "Fill Opacity", feature, 0, {0, 255}
    };
    inline SillySetting<float> labelSize{
        "Label Scale", feature, 1.25f, {0.0f, std::nullopt}
    };
    inline SillySetting<float> labelDistance{
        "Label Distance", feature, 2.5f
    };
    inline SillySetting<bool> dontRotateLabel{
        "Dont Rotate Label", feature, false
    };
    inline SillySetting<bool> labelOnRight{
        "Label On Right", feature, false
    };
    inline SillySetting<bool> labelOnBottom{
        "Label On Bottom", feature, false
    };
    inline SillySetting<bool> chroma{
        "Chroma", feature, false
    };
}