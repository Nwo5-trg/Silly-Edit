#pragma once

#include <Geode/modify/EditorUI.hpp>
#include <feature/include.hpp>

namespace EditorTime {
    class $feature(EditorTime) {
        void onToggled(bool pEnabled) override;
        void onUpdate() override;
    } feature;

    class $feature_modify(EditorUI) {
        struct Fields {
            asp::Instant start = asp::Instant::now();
            geode::Label* timeLabel = nullptr;
        };
    };

    class $setting_category("editor-time-logo.png"_spr, "It should be noted this time isnt entirely accurate, as it doesnt remove for idle time");

    inline SillySetting<float> scale{
        "Scale", feature, 1.0f, {0.1f, std::nullopt}
    };
    inline SillySetting<bool> showTotal{
        "Show Total", feature, false, "show total editor time next to time this session"
    };
    inline SillySetting<bool> alignWithFPSLabel{
        "Align With FPS Label", feature, false
    };
}