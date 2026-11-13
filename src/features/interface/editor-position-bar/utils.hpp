#pragma once

namespace EditorPositionBar {
    class EditorPositionButton : public geode::Button {
    protected:
        static constexpr cocos2d::CCSize SIZE{30.0f, 20.0f};
        static constexpr float LABEL_HEIGHT = 5.0f;

        static cocos2d::CCSprite* createEditorSnapshot(cocos2d::CCPoint pos, cocos2d::CCSize viewSize);

        bool init(int index, cocos2d::CCPoint pos);
    public:
        static EditorPositionButton* create(int index, cocos2d::CCPoint pos);
    };
}