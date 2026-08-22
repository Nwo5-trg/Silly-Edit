#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

namespace ScrollGroups {
    void EditorUI::scrollGroup(GameObject* pObj, bool pUp) {
        const auto target = editor::trigger::target(pObj) + (pUp ? 1 : -1);

        if (target >= 0 && target <= nwo5::editor::constants::MAX_GROUPS) {
            editor::trigger::setTarget(pObj, target);
        }
    }

    void Feature::onEditor() {
        auto self = editor::ui<ScrollGroups::EditorUI>();

        feature.registerKeybind<"modifier">([self] (bool pDown, bool) {
            self->m_fields->modifierDown = pDown;
        });

        self->addEventListener(ScrollWheelEvent(), [self] (double pX, double pY) {
            auto fields = self->m_fields.self();

            if (!ScrollGroups::enabled() || !fields->modifierDown || Sillyedit::shouldBlockScrolling()) {
                return;
            }

            const auto pos = self->m_editorLayer->m_objectLayer->convertToNodeSpace(cocos::getMousePos());
            
            auto obj = self->m_editorLayer->objectAtPosition(pos);

            if (!obj || !editor::trigger::is(obj)) {
                return;
            }

            const auto primaryTarget = editor::trigger::targetType(obj);

            if (primaryTarget == nwo5::editor::trigger::InputType::None) {
                return;
            }

            Sillyedit::shouldBlockScrolling() = true;

            if (fields->lastScroll.elapsed() > asp::Duration::fromMillis(ScrollGroups::scrollTimeout)) {
                fields->scrollingDistance = 0;
            }

            fields->lastScroll = asp::Instant::now();

            fields->scrollingDistance += ScrollGroups::reverseScroll ? -pY : pY;

            if (std::abs(fields->scrollingDistance) > DEFAULT_SCROLL_DISTANCE / ScrollGroups::scrollSensitivity) {
                self->scrollGroup(obj, fields->scrollingDistance > 0);

                fields->scrollingDistance = 0;
            }
            
        }, Priority::Early);
    }
}