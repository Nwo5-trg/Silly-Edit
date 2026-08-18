#pragma once

#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <feature/include.hpp>

namespace PlaceObjectPreview {
    class $feature(PlaceObjectPreview, SettingCondition::DesktopAndAndroidDisable) {
        void onUpdate() override;
    } feature;

    class $feature_modify(EditorUI) {
        struct Fields {
            GameObject* previewObject = nullptr;
            bool wasSelectedObject = false;
        };

        void updatePreviewObject();

        void onPause(cocos2d::CCObject* sender);
        bool onCreate();
        bool canSelectObject(GameObject* object);
        void selectObject(GameObject* object, bool ignoreFilter);
        void transformObjectCall(EditCommand command);
    };

    class $feature_modify(LevelEditorLayer) {
        static void onModify(auto& pSelf);

        void addSpecial(GameObject* object);
        void onPlaytest();
        gd::string getLevelString();
        bool typeExistsAtPosition(int objectID, cocos2d::CCPoint position, bool flipX, bool flipY, float rotation);
        void updateVisibility(float dt);
    };

    class $setting_category("place-object-preview-logo.png"_spr, "Show a ghost of the object ur about to place !");

    inline SillySetting<int> opacity{
        "Preview Opacity", feature, 75, {0, 255}
    };
}