#pragma once

namespace Editor {
    EditorUI* ui();
    LevelEditorLayer* layer();
    
    bool loaded();

    void update(bool pUpdateControls = true);

    float zoom();
    cocos2d::CCPoint center();

    constexpr float GRID_SIZE = 30.0f;
    constexpr float GRID_SIZE_OBJECT = 10.0f;
    /// ok so apparently robtop just subtracts 90 from the position when making object strings cuz fuck u and
    /// doesnt use m_toolbarHeight or smth so (this took me like an hour of reverse engineering to figure out im kms)
    constexpr cocos2d::CCPoint OBJECT_STRING_POSITION_OFFSET{0.0f, -90.0f};

    constexpr cocos2d::CCPoint AUTO_CENTER{std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};

    namespace Impl {
        /// param in bindings actually js named wrong i think (according to my reverse engineering findings anyway)
        void createUndoObject(UndoCommand pCommand, bool pAddToList);
    }
}