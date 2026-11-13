#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/EffectGameObject.hpp>
#include <feature/include.hpp>
#include "utils.hpp"

namespace GroupLabelShenanigans {
    class $feature(GroupLabelShenanigans) {
        void onEditor() override;
        void onUpdate() override;
        void onSettingChanged(std::string name, GenericSetting*) override;
    } feature;

    class $feature_modify(LevelEditorLayer) {
        struct Fields {
            std::unique_ptr<LabelOptions> options;

            ~Fields();
        };

        static void onModify(auto& self) {
            (void)self.setHookPriority("LevelEditorLayer::updateObjectLabel", geode::Priority::Replace);
        }

        void updateLabelsInSection(bool positionsOnly = false);

        static void updateObjectLabel(GameObject* object);
    };

    class $feature_modify(EffectGameObject) {
        struct Fields {
            geode::Label* label = nullptr;
            cocos2d::CCSprite* sprite = nullptr;

            GroupLabelShenanigans::EffectGameObject* obj = nullptr;

            ~Fields();
        };

        void removeShenanigans();
        void updateShenaniganPositions(geode::Label* label, cocos2d::CCSprite* sprite);
        void updateShenanigans(LabelOptions* options);
        void addShenanigans(LabelOptions* options);
        
        void customSetup();
    };

    class $setting_category("group-label-shenanigans-logo.png"_spr, "Data oriented group labels");

    inline Setting<bool> extras{
        "Extras", feature, true, "dots on the top right of some triggers if some condition is true"
    };
    inline Setting<std::string> mode{
        "Mode", feature, "More Info", {"More Info", "Custom", "Vanilla"}, "custom lets you define group labels in json, for an example check examples/group-label-shenanigans.jsonc in the github repo"
    };
    inline Setting<std::string> customPath{
        "Custom Path", feature, "group-labels.json", "if mode is set to custom, ur path will be configdir/PATH"
    };
    inline Setting<bool> dontRotateLabel{
        "Dont Rotate Label", feature, false, "technically a little slow lol"
    };
}