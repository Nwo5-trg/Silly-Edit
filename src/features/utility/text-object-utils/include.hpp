#pragma once

#include <Geode/modify/CustomizeObjectLayer.hpp>
#include <feature/include.hpp>

namespace TextObjectUtils {
    class $feature(TextObjectUtils) {} feature;

    constexpr float VERTICAL_OFFSET = -20.0f;

    constexpr float SIDE_BUTTON_DISTANCE = 120.0f;
    constexpr float SIDE_BUTTON_SIZE = 25.0f;
    constexpr float SIDE_BUTTON_GAP = 5.0f;

    class $feature_modify(CustomizeObjectLayer) {
        struct Fields {
            std::vector<TextGameObject*> textObjects;

            geode::TextInput* kerningInput = nullptr;
            cocos2d::CCMenu* textObjectUtilsMenu = nullptr;
        };

        void onCopyText(CCObject*);
        void onPasteText(CCObject*);
        void onClearText(CCObject*);
        void onNewline(CCObject*);
        void openTextMenu();

        bool init(GameObject* object, cocos2d::CCArray* objects);
        void textChanged(CCTextInputNode* node);
        void sliderChanged(CCObject* sender);
        void onClose(CCObject* sender);
    };

    class $setting_category("text-object-utils-logo.png"_spr, "Newlines in your text and some other stuff");

    inline SillySetting<std::string> newlineShortcut{
        "Newline Shortcut", "Text Object Utils", "\\n"
    };
    inline SillySetting<bool> swapCopyPaste{
        "Swap\nCopy Paste", "Text Object Utils", false
    };
}