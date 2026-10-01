#pragma once

#include <Geode/modify/LevelEditorLayer.hpp>
#include <feature/include.hpp>
#include "utils.hpp"

namespace TriggerIndicators {
    class $feature(TriggerIndicators) {
        void onEditor() override;
        void onSettingChanged(std::string pName, GenericSetting*) override;
    } feature;

    class $feature_modify(LevelEditorLayer) {
        struct Fields {
            std::unique_ptr<Drawer> m_drawer = nullptr;
        };

        void updateDebugDraw();
    };

    class $setting_category("trigger-indicators-logo.png"_spr, "Draw lines to trigger targets");

    inline SillySetting<float> maxDistance{
        "Max Distance", feature, 300.0f, {0.0f, 10000.0f}, "max distance between objects to draw indicators (without selecting), set to 0 for no max distance"
    };
    inline SillySetting<bool> onlySelected{
        "Only Selected", feature, false, "only show indicators for selected triggers/objs"
    };
    inline SillySetting<bool> onlyTriggers{
        "Only Triggers", feature, false, "only show trigger to trigger connections"
    };
    inline SillySetting<bool> onlySpawn{
        "Only Spawn", feature, true, "only triggers only show spawn connected triggers instead of all triggers with that id"
    };
    inline SillySetting<bool> selectOverride{
        "Select Override", feature, false, "selecting an object/trigger overrides only trgger/spawn restrictions"
    };
    inline SillySetting<float> thickness{
        "Thickness", feature, 1.0f, {0.0f, 50.0f}
    };
    inline SillySetting<bool> clusterObjects{
        "Cluster Objects", feature, true
    };
    inline SillySetting<float> maxObjectClusterDistance{
        "Max Object Cluster Distance", feature, 60.0f, {0.0f, 300.0f}, "when clustering, any neighbours further than this will form their own cluster instead of joining one"
    };
    inline SillySetting<int> objectClustersMaxThreshold{
        "Object Clusters Max Threshold", feature, 90, {1, 1000}, "maximum amount of objects for clustering, any more and fallback gets used (dont set to a high number 1k == 1 mil calls worse case sowwy not sowwy that my cluster implementation is on^2 anything more and its too much for me to understand :3c)"
    };
    inline SillySetting<bool> objectClusterLineFallback{
        "Object Cluster Line Fallback", feature, false, "if cluster threshold is exceeded fallback to indivudal lines to every object instead of one big rect"
    };
    inline SillySetting<bool> clusterTriggers{
        "Cluster Triggers", feature, true
    };
    inline SillySetting<float> maxTriggerClusterDistance{
        "Max Trigger Cluster Distance", feature, 30.0f, {0.0f, 300.0f}, "when clustering, any neighbours further than this will form their own cluster instead of joining one"
    };
    inline SillySetting<int> triggerClustersMaxThreshold{
        "Trigger Clusters Max Threshold", feature, 90, {1, 1000}, "maximum amount of triggers for clustering, any more and fallback gets used (dont set to a high number 1k == 1 mil calls worse case sowwy not sowwy that my cluster implementation is on^2 anything more and its too much for me to understand :3c)"
    };
    inline SillySetting<bool> triggerClusterLineFallback{
        "Trigger Cluster Line Fallback", feature, true, "if cluster threshold is exceeded fallback to indivudal lines to every trigger instead of one big rect"
    };
    inline SillySetting<bool> scaleWithZoom{
        "Scale With Zoom", feature, false
    };
    inline SillySetting<bool> dottedCenterLines{
        "Dotted Center Lines", feature, true
    };
    inline SillySetting<float> dottedSegmentSize{
        "Dotted Segment Size", feature, 7.5f, {0.0f, 50.0f}, "size of filled in part + empty space"
    };
    inline SillySetting<float> dottedDotSize{
        "Dotted Dot Size", feature, 0.5f, {0.0f, 1.0f}, "percent of dotted segment size the dot should cover"
    };
    inline SillySetting<bool> alwaysDrawExtras{
        "Always Draw Extras", feature, false, "always show input/output extras for all triggers, regardless of if theyre targeting anything"
    };
    inline SillySetting<cocos2d::ccColor3B> extrasFillCol{
        "Extras Fill Color", feature, cocos2d::ccColor3B{255, 255, 255}, "input/output indicators"
    };
    inline SillySetting<cocos2d::ccColor3B> extrasOutlineCol{
        "Extras Outline Color", feature, cocos2d::ccColor3B{0, 0, 0}, "input/output indicators"
    };
    inline SillySetting<bool> circleOutputExtras{
        "Circle Output Extras", feature, false
    };
    inline SillySetting<bool> drawExtrasBehind{
        "Draw Extras Behind", feature, false, "draw extras behind triggers"
    };
    inline SillySetting<float> extrasScale{
        "Extras Scale", feature, 1.0, {0.1f, 5.0f}
    };
    inline SillySetting<float> extrasOffset{
        "Extras Offset", feature, 12.5f, {0.0f, 50.0f}, "how far horizontally should extras be from trigger center"
    };
    inline SillySetting<std::string> groupBlacklist{
        "Group Blacklist", feature, "", "you can input more groups like \"1,2,3,4,5\""
    };
    inline SillySetting<float> cullMultiplier{
        "Cull Multiplier", feature, 2.5f, {0.1f, 500.0f}, "higher number means slower, but could be useful for not making really long connections disappear"
    };
    inline SillySetting<float> cullMultiplierSelectionMod{
        "Cull Multiplier Selection Mod", feature, 5.0f, {0.1f, 500.0f}, "cull multiplier but specifically for whenever any objects with groups are selected (so u can see incoming connections from further away)"
    };
    inline SillySetting<float> minimumCullingSize{
        "Minimum Culling Size", feature, 2500.0f, {0.0f, 100000.0f}, "clamp size of viewport to be atleast this, this is so stuff doesnt clip out of existence at really high zooms"
    };
    inline SillySetting<bool> noCulling{
        "No Culling", feature, false, "good luck !"
    };
    inline SillySetting<std::string> triggerBlacklist{
        "Trigger Blacklist", feature, "", "you can input more trigger ids like \"1,2,3,4,5\""
    };
    inline SillySetting<bool> drawLines{
        "Draw Lines", feature, false, "draw lines instead of \"segments\", objects with mutliple connections might look a bit off at the point where connections meet, but might b a little faster"
    };
    inline SillySetting<bool> chroma{
        "Chroma", feature, false, "ooooo gay rgbtq lights :3c"
    };
    inline SillySetting<bool> chromaByObject{
        "Chroma By Object", feature, false, "determine chroma offset by individual trigger object instead of by id"
    };
}