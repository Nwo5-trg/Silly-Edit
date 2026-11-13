#pragma once

#include <Geode/modify/EditorUI.hpp>
#include <feature/include.hpp>
#include "utils.hpp"

namespace Ruler {
    class $feature(Ruler) {
        void onEditor() override;
        void onToggled(bool enabled) override;
        void onUpdate() override;
    } feature;

    class $feature_modify(EditorUI) {
        struct Fields {
            std::vector<Measurement> measurements;
        };

        MeasurementColor getMeasurementColor();
        std::string getMeasurementString(float measure);
        geode::Label* createMeasurementLabel(float measure);
        void createMeasurement();
        void deleteMeasurement(bool deleteAll);
    };

    class $setting_category("ruler-logo.png"_spr, "Measure an area with an overcomplicated overlay (now with rainbow)");

    inline Setting<bool> useGDUnits{
        "Use GD Units", feature, true, "one block = 10 gd units = 30 units, if enabled will show 45 units as '1, 5'"
    };
    inline Setting<bool> editorTabButton{
        "Editor Tab Button", feature, true, SettingReload::Editor
    };
    inline Setting<float> thickness{
        "Thickness", feature, 2.5f, {0.0f, std::nullopt}
    };
    inline Setting<bool> showCenter{
        "Show Center", feature, true
    };
    inline Setting<float> centerSize{
        "Center Size", feature, 5.0f, {0.0f, std::nullopt}
    };
    inline Setting<bool> scaleWithZoom{
        "Scale With Zoom", feature, false
    };
    inline Setting<float> padding{
        "Padding", feature, 0.0f
    };
    inline Setting<int> fillOpacity{
        "Fill Opacity", feature, 0, {0, 255}
    };
    inline Setting<float> labelSize{
        "Label Scale", feature, 1.25f, {0.0f, std::nullopt}
    };
    inline Setting<float> labelDistance{
        "Label Distance", feature, 2.5f
    };
    inline Setting<bool> dontRotateLabel{
        "Dont Rotate Label", feature, false
    };
    inline Setting<bool> labelOnRight{
        "Label On Right", feature, false
    };
    inline Setting<bool> labelOnBottom{
        "Label On Bottom", feature, false
    };
    inline Setting<bool> chroma{
        "Chroma", feature, false
    };
}