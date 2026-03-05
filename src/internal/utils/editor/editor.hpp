#pragma once

namespace Editor {
    EditorUI* ui();
    LevelEditorLayer* layer();
    
    bool loaded();

    void update(bool pUpdateControls = true, bool pOtherwiseDeactivateControls = false);

    float zoom();
    cocos2d::CCPoint center();

    void activateRotationControl(bool pRefresh = true);
    void activateScaleControl(bool pXY, bool pRefresh = true);
    void activateTransformControl(bool pRefresh = true);

    int currentLayer();
    bool layerSelectable(int pLayer, bool pIgnoreLocked = false);
    bool layerLocked(int pLayer);

    constexpr float GRID_SIZE = 30.0f;
    constexpr float GRID_SIZE_OBJECT = 10.0f;

    /// ok so apparently robtop just subtracts 90 from the position when making object strings cuz fuck u and
    /// doesnt use m_toolbarHeight or smth so (this took me like an hour of reverse engineering to figure out im kms)
    constexpr cocos2d::CCPoint OBJECT_STRING_POSITION_OFFSET{0.0f, -90.0f};
    constexpr size_t MAX_LAYERS = 9999;

    constexpr float MIN_ZOOM = 0.1f;
    constexpr float MAX_ZOOM = 4.0f;

    constexpr cocos2d::CCPoint AUTO_CENTER{std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};

    namespace Impl {
        void createUndoObject(UndoCommand pCommand);
    }
}