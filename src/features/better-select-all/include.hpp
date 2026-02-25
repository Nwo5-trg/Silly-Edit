#pragma once

namespace BetterSelectAll {
    enum class SelectDirection {
        NorthWest,
        North,
        NorthEast,
        West,
        All,
        East,
        SouthWest,
        South,
        SouthEast
    };

    cocos2d::CCArray* getObjectsWithDirection(SelectDirection pDirection, bool pSelectedObjectsCenter);

    class BetterSelectAllPopup final : public geode::Popup {
    private:
        bool m_selectedObjectMode;
        bool m_bypassAll;

        static constexpr float SELECT_BUTTON_SIZE = 50.0f;
        static constexpr float SELECT_BUTTON_GAP = 5.0f;
        static constexpr float BUTTON_SIZE = 35.0f;

        static constexpr float PADDING = 5.0f;
        static constexpr float BACKGROUND_BUFFER = 5.0f;

        static constexpr float WIDTH = 300.0f;
        static constexpr float HEIGHT = 225.0f;

        bool init();

        void onSelectButton(cocos2d::CCObject* pSender);
        void onToggleBypassAll(cocos2d::CCObject* pSender);
        void onToggleSelectedObjectMode(cocos2d::CCObject* pSender);

    public:
        static BetterSelectAllPopup* create();
    };
}