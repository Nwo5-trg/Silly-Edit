#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

namespace EditorTime {
    void Feature::onToggled(bool pEnabled) {
        auto self = editor::ui<EditorTime::EditorUI>();
        auto fields = self->m_fields.self();

        if (!pEnabled) {
            if (fields->timeLabel) {
                fields->timeLabel->removeMeAndCleanup();
            }

            return;
        };

        fields->timeLabel = ui::label(Font::Chat)
            .id("time-label"_spr)
            .anchor(Anchor::TopLeft)
            .addTo(self->m_uiItems)
            .order(15)
            .parent(self);
    }

    void Feature::onUpdate() {
        auto self = editor::ui<EditorTime::EditorUI>();
        auto fields = self->m_fields.self();

        if (!fields->timeLabel) {
            return;
        }

        const auto elapsed = fields->start.elapsed();
        auto str = fmt::format("Elapsed: {:02}:{:02}:{:02}", elapsed.hours(), elapsed.minutes() % 60, elapsed.seconds() % 60);

        if (EditorTime::showTotal) {
            const auto totalElapsed = asp::Duration::fromSecs(editor::layer()->m_level->m_workingTime) + elapsed;
            str += fmt::format(" (Total: {:02}:{:02}:{:02})", totalElapsed.hours(), totalElapsed.minutes() % 60, totalElapsed.seconds() % 60);
        }

        CCPoint pos{1.0f, ui::winHeight()};

        if (auto director = CCDirector::get(); director->m_bDisplayFPS && director->m_pFPSNode) {
            if (EditorTime::alignWithFPSLabel) {
                pos.x = std::ceil(ui::sw(director->m_pFPSNode) / 10) * 10 + 5.0f;
            }
            else {
                pos.y -= ui::sh(director->m_pFPSNode);
            }
        }

        Setup(fields->timeLabel)
            .scale(0.5f * EditorTime::scale)
            .pos(pos)
            .text(str);
    }
}