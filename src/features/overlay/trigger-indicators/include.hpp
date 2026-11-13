#pragma once

#include <Geode/modify/LevelEditorLayer.hpp>
#include <feature/include.hpp>
#include "utils.hpp"

namespace TriggerIndicators {
    class $feature(TriggerIndicators) {
        void onEditor() override;
        void onUpdate() override;
        void onToggled(bool enabled) override;
        void onSettingChanged(std::string name, GenericSetting*) override;
    } feature;

    class $feature_modify(LevelEditorLayer) {
        struct Fields {
            std::unique_ptr<Drawer> m_drawer = nullptr;
        };
    };

    class $setting_category("trigger-indicators-logo.png"_spr, "Draw lines to trigger targets");

    inline Setting<float> maxDistance{
        "Max Distance", feature, 300.0f, {0.0f, 10000.0f}, "max distance between objects to draw indicators (without selecting), set to 0 for no max distance"
    };
    inline Setting<bool> onlySelected{
        "Only Selected", feature, false, "only show indicators for selected triggers/objs"
    };
    inline Setting<bool> onlyTriggers{
        "Only Triggers", feature, false, "only show trigger to trigger connections"
    };
    inline Setting<bool> onlySpawn{
        "Only Spawn", feature, true, "only triggers only show spawn connected triggers instead of all triggers with that id"
    };
    inline Setting<bool> selectOverride{
        "Select Override", feature, false, "selecting an object/trigger overrides only trgger/spawn restrictions"
    };
    inline Setting<float> thickness{
        "Thickness", feature, 1.0f, {0.0f, 50.0f}
    };
    inline Setting<bool> clusterObjects{
        "Cluster Objects", feature, true
    };
    inline Setting<float> maxObjectClusterDistance{
        "Max Object Cluster Distance", feature, 60.0f, {0.0f, 300.0f}, "when clustering, any neighbours further than this will form their own cluster instead of joining one"
    };
    inline Setting<int> objectClustersMaxThreshold{
        "Object Clusters Max Threshold", feature, 90, {1, 1000}, "maximum amount of objects for clustering, any more and fallback gets used (dont set to a high number 1k == 1 mil calls worse case sowwy not sowwy that my cluster implementation is on^2 anything more and its too much for me to understand :3c)"
    };
    inline Setting<bool> objectClusterLineFallback{
        "Object Cluster Line Fallback", feature, false, "if cluster threshold is exceeded fallback to indivudal lines to every object instead of one big rect"
    };
    inline Setting<bool> clusterTriggers{
        "Cluster Triggers", feature, true
    };
    inline Setting<float> maxTriggerClusterDistance{
        "Max Trigger Cluster Distance", feature, 30.0f, {0.0f, 300.0f}, "when clustering, any neighbours further than this will form their own cluster instead of joining one"
    };
    inline Setting<int> triggerClustersMaxThreshold{
        "Trigger Clusters Max Threshold", feature, 90, {1, 1000}, "maximum amount of triggers for clustering, any more and fallback gets used (dont set to a high number 1k == 1 mil calls worse case sowwy not sowwy that my cluster implementation is on^2 anything more and its too much for me to understand :3c)"
    };
    inline Setting<bool> triggerClusterLineFallback{
        "Trigger Cluster Line Fallback", feature, true, "if cluster threshold is exceeded fallback to indivudal lines to every trigger instead of one big rect"
    };
    inline Setting<bool> scaleWithZoom{
        "Scale With Zoom", feature, false
    };
    inline Setting<bool> dottedCenterLines{
        "Dotted Center Lines", feature, true
    };
    inline Setting<float> dottedSegmentSize{
        "Dotted Segment Size", feature, 7.5f, {0.0f, 50.0f}, "size of filled in part + empty space"
    };
    inline Setting<float> dottedDotSize{
        "Dotted Dot Size", feature, 0.5f, {0.0f, 1.0f}, "percent of dotted segment size the dot should cover"
    };
    inline Setting<bool> alwaysDrawExtras{
        "Always Draw Extras", feature, false, "always show input/output extras for all triggers, regardless of if theyre targeting anything"
    };
    inline Setting<cocos2d::ccColor3B> extrasFillCol{
        "Extras Fill Color", feature, cocos2d::ccColor3B{255, 255, 255}, "input/output indicators"
    };
    inline Setting<cocos2d::ccColor3B> extrasOutlineCol{
        "Extras Outline Color", feature, cocos2d::ccColor3B{0, 0, 0}, "input/output indicators"
    };
    inline Setting<bool> circleOutputExtras{
        "Circle Output Extras", feature, false
    };
    inline Setting<bool> drawExtrasBehind{
        "Draw Extras Behind", feature, false, "draw extras behind triggers"
    };
    inline Setting<float> extrasScale{
        "Extras Scale", feature, 1.0, {0.1f, 5.0f}
    };
    inline Setting<float> extrasOffset{
        "Extras Offset", feature, 12.5f, {0.0f, 50.0f}, "how far horizontally should extras be from trigger center"
    };
    inline Setting<std::string> groupBlacklist{
        "Group Blacklist", feature, "", "you can input more groups like \"1,2,3,4,5\""
    };
    inline Setting<float> cullMultiplier{
        "Cull Multiplier", feature, 2.5f, {0.1f, 500.0f}, "higher number means slower, but could be useful for not making really long connections disappear"
    };
    inline Setting<float> cullMultiplierSelectionMod{
        "Cull Multiplier Selection Mod", feature, 5.0f, {0.1f, 500.0f}, "cull multiplier but specifically for whenever any objects with groups are selected (so u can see incoming connections from further away)"
    };
    inline Setting<float> minimumCullingSize{
        "Minimum Culling Size", feature, 2500.0f, {0.0f, 100000.0f}, "clamp size of viewport to be atleast this, this is so stuff doesnt clip out of existence at really high zooms"
    };
    inline Setting<bool> noCulling{
        "No Culling", feature, false, "good luck !"
    };
    inline Setting<std::string> triggerBlacklist{
        "Trigger Blacklist", feature, "", "you can input more trigger ids like \"1,2,3,4,5\""
    };
    inline Setting<bool> drawLines{
        "Draw Lines", feature, false, "draw lines instead of \"segments\", objects with mutliple connections might look a bit off at the point where connections meet, but might b a little faster"
    };
    inline Setting<bool> chroma{
        "Chroma", feature, false, "ooooo gay rgbtq lights :3c"
    };
    inline Setting<bool> chromaByObject{
        "Chroma By Object", feature, false, "determine chroma offset by individual trigger object instead of by id"
    };
}