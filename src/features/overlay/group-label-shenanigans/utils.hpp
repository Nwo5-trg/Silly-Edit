#pragma once

#include <utils/include.hpp>

namespace GroupLabelShenanigans {
    enum class Label {
        Primary,
        Secondary,
        PrimaryInput,
        SecondaryInput
    };

    struct ObjectConfig final {
        std::vector<Label> firstRow;
        std::vector<Label> secondRow;
    };

    enum class Extra {
        None,
        ActivateGroup,
        ControlID,
        PulseTarget,
        Override,
        Follow
    };

    struct ExtraConfig final {
        Extra type = Extra::None;
        cocos2d::ccColor4B color = {0, 0, 0, 0};
        cocos2d::ccColor4B offColor = {0, 0, 0, 0};
    };
    
    class LabelOptions final {
    protected:
        std::array<ObjectConfig, nwo5::editor::constants::OBJECT_IDS + 1> m_objectConfigs;
        std::array<ExtraConfig, nwo5::editor::constants::OBJECT_IDS + 1> m_extraConfigs;
        std::array<bool, nwo5::editor::constants::OBJECT_IDS + 1> m_blacklist;

    public:
        static constexpr auto BUF_SIZE = 11 * 4 + sizeof('/') * 3 + sizeof('\n') + sizeof('\0');

        LabelOptions();
        
        void addConfig(int iD, const ObjectConfig& config);
        void addExtra(int id, const ExtraConfig& config);
        void blacklist(int iD);

        void stringForObject(char* buf, GameObject* obj);
        std::optional<cocos2d::ccColor4B> extraForObject(GameObject* obj);

        void reset();
    };

    void parseLabels(LabelOptions& labelOptions);
}