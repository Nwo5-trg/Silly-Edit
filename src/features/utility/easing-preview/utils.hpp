#pragma once

#include <nwo5.silly-api/utils/include.hpp>

namespace EasingPreview {
    enum class PreviewType {
        None,
        Move,
        Rotate,
        Scale
    };
    
    class EasingButton final : public CCMenuItemSpriteExtra {
    public:
        static constexpr cocos2d::CCSize SIZE{60.0f, 40.0f};
    protected:
        cocos2d::CCSprite* m_sprite = nullptr;
        cocos2d::CCSprite* m_dot = nullptr;
        nwo5::utils::SillyDrawNode* m_drawnode = nullptr;
        EasingType m_easing = EasingType::None;
        float m_exponent = 1.0f;

        asp::Instant m_start = asp::Instant::now();
        
        static constexpr float SPRITE_SIZE = 15.0f;
        static constexpr float DOT_SIZE = 2.5f;
        static constexpr float PADDING = 10.0f;

        bool init(EasingType easing, PreviewType type, float exponent, geode::CopyableFunction<void(EasingType)> callback);
        void update(float);
    public:
        void setGraphMode(bool enabled);

        static EasingButton* create(EasingType easing, PreviewType type, float exponent, geode::CopyableFunction<void(EasingType)> callback);
    };

    class SelectEasingPopup final : public geode::Popup {
    protected:
        std::vector<EasingButton*> m_buttons;

        static constexpr float PADDING = 20.0f;

        static constexpr float LABEL_HEIGHT = 10.0f;
        static constexpr float MENU_GAP = 5.0f;
        static constexpr cocos2d::CCSize MENU_SIZE{
            EasingButton::SIZE.width * 5 + MENU_GAP * 4,
            EasingButton::SIZE.height * 4 + LABEL_HEIGHT * 4 + MENU_GAP * 7
        };

        static constexpr float GRAPH_MODE_TOGGLE_SIZE = 30.0f;

        static constexpr cocos2d::CCSize SIZE = MENU_SIZE + cocos2d::CCSize{PADDING, PADDING + 30.0F};

        bool init(SetupTriggerPopup* popup, PreviewType type, int prop);

        void onGraphModeToggle(cocos2d::CCObject* sender);
    public:
        static SelectEasingPopup* create(SetupTriggerPopup* popup, PreviewType type, int prop);

        void onClose(cocos2d::CCObject* sender);
    };
}