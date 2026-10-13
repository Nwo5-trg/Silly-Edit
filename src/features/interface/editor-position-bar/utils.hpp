#pragma once

namespace EditorPositionBar {
    class EditorPositionButton : public geode::Button {
    protected:
        static constexpr cocos2d::CCSize SIZE{30.0f, 20.0f};
        static constexpr float LABEL_HEIGHT = 5.0f;

        static cocos2d::CCSprite* createEditorSnapshot(cocos2d::CCPoint pPos, cocos2d::CCSize pViewSize);

        bool init(int pIndex, cocos2d::CCPoint pPos);
    public:
        static EditorPositionButton* create(int pIndex, cocos2d::CCPoint pPos);
    };
}