#pragma once

#include <settings/include.hpp>

namespace ContextMenu {
    class Option;

    class ContextMenuNode final : public cocos2d::CCNode {
    protected:
        cocos2d::CCMenu* m_optionsMenu = nullptr;
        geode::NineSlice* m_background = nullptr;
        std::vector<std::pair<geode::Label*, const Option*>> m_labels;

        bool m_specialDown;

        static constexpr float SCREEN_PADDING = 10.0f;
        static constexpr float PADDING = 5.0f;
        static constexpr float GAP = 2.5f;

        static constexpr float OPTION_HEIGHT = 7.5f;
        static constexpr float OPTION_GAP = 2.5f;
        static constexpr float ICON_SIZE = 7.5f;
        static constexpr float LABEL_WIDTH = 35.0f;
        static constexpr float SPACER_HEIGHT = 1.0f;

        static constexpr int SPACER_TAG = 1;

        bool init();
        void update(float pForceUpdate);

        void setup(std::vector<const Option*> pOptions);

    public:
        void show(cocos2d::CCPoint pPos, cocos2d::CCArray* pObjects);
        void hide();

        static ContextMenuNode* create();
    };

    struct Option final {
        enum class Condition {
            Default,
            DisallowMultiple,
            Static,
            StaticOnly
        };
        enum class Type {
            Default,
            Spacer
        };

        using Callback = geode::CopyableFunction<void(bool pSpecialDown)>;

        struct NameType {
            std::string label;
            std::optional<std::string> special = std::nullopt;

            NameType() = default;
            NameType(const char* pLabel, std::optional<std::string> pSpecial = std::nullopt)
                : label(pLabel), special(std::move(pSpecial)) {}
        } name;
        cocos2d::ccColor3B color{255, 255, 255};
        struct ConditionType {
            Condition type = Condition::Default;
            geode::CopyableFunction<bool()> predicate = nullptr;

            ConditionType() = default;
            ConditionType(Condition pType, geode::CopyableFunction<bool()> pPredicate = nullptr)
                : type(pType), predicate(std::move(pPredicate)) {}
        } condition;
        Type type = Type::Default;
        Callback callback = nullptr;

        Option(NameType pName, cocos2d::ccColor3B pColor, ConditionType pCondition, Callback pCallback)
            : name(std::move(pName)), color(pColor), condition(std::move(pCondition)), callback(std::move(pCallback)) {}
        Option(NameType pName, cocos2d::ccColor3B pColor, Callback pCallback)
            : name(std::move(pName)), color(pColor), callback(std::move(pCallback)) {}
        Option(Type pType)
            : type(pType) {}

        bool enabled() const;

    protected: 
        // this is just so i dont have to type Options:: lol
        static void registerOptions(std::deque<Option>& pOptions);

        SillySetting<bool>* setupSetting();
        SillySetting<bool>* m_setting = setupSetting();

        friend class OptionsRegistry;
    };

    class OptionsRegistry final {
    protected:
        std::deque<Option> m_options;

        OptionsRegistry();

    public:
        static OptionsRegistry* get() {
            static OptionsRegistry inst;
            return &inst;
        }

        std::vector<const Option*> forObjects(cocos2d::CCArray* pObjects) const;
    };
}