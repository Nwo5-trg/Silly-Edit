#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/EffectGameObject.hpp>
#include <feature/include.hpp>
#include "utils.hpp"

namespace GroupLabelShenanigans {
    class $feature(GroupLabelShenanigans) {
        void onEditor() override;
        void onSettingChanged(std::string pName, GenericSetting*) override;
    } feature;

    class $feature_modify(LevelEditorLayer) {
        struct Fields {
            std::unique_ptr<LabelOptions> options;
            int lastObjectCount = -1;

            ~Fields();
        };

        void updateLabelsInSection(bool pPositionsOnly);

        static void updateObjectLabel(GameObject* object);
        void updateDebugDraw();
    };

    class $feature_modify(EffectGameObject) {
        struct Fields {
            geode::Label* label = nullptr;
            cocos2d::CCSprite* sprite = nullptr;

            GroupLabelShenanigans::EffectGameObject* obj = nullptr;

            ~Fields();
        };

        void removeShenanigans();
        void updateShenaniganPositions(geode::Label* pLabel, cocos2d::CCSprite* pSprite);
        void updateShenanigans(LabelOptions* pOptions);
        void addShenanigans(LabelOptions* pOptions);
        
        void customSetup();
    };

    class $setting_category("group-label-shenanigans-logo.png"_spr, "Data oriented group labels");

    inline SillySetting<bool> optimize{
        "Optimize", feature, true, "disable if labels dont update properly, this setting updates label for object whenever robtop updates label for object instead of every frame"
    };
    inline SillySetting<bool> extras{
        "Extras", feature, true, "dots on the top right of some triggers if some condition is true"
    };
    inline SillySetting<std::string> mode{
        "Mode", feature, "More Info", {"More Info", "Custom", "Vanilla"}, "custom lets you define group labels in json, for an example check examples/group-label-shenanigans.jsonc in the github repo"
    };
    inline SillySetting<std::string> customPath{
        "Custom Path", feature, "group-labels.json", "if mode is set to custom, ur path will be configdir/PATH"
    };
    inline SillySetting<bool> dontRotateLabel{
        "Dont Rotate\nLabel", feature, false, "technically a little slow lol"
    };
}