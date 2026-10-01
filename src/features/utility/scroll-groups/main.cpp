#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

static constexpr float DEFAULT_SCROLL_DISTANCE = 2.5f;

namespace ScrollGroups {
    void EditorUI::scrollGroup(GameObject* pObj, bool pUp, bool pSecondaryGroup) {
        if (pSecondaryGroup) {
            const auto target = trigger::center(pObj) + (pUp ? 1 : -1);

            if (target >= 0 && target <= nwo5::editor::constants::MAX_GROUPS) {
                trigger::setCenter(pObj, target);
            }
            }
        else {
            const auto target = trigger::target(pObj) + (pUp ? 1 : -1);

            if (target >= 0 && target <= nwo5::editor::constants::MAX_GROUPS) {
                trigger::setTarget(pObj, target);
            }
        }
    }

    void Feature::onEditor() {
        auto self = editor::ui<ScrollGroups::EditorUI>();

        self->addEventListener(ScrollWheelEvent(), [self] (double pX, double pY) {
            auto fields = self->m_fields.self();

            if (!ScrollGroups::enabled() || !sillyedit::utils::modifierDown(ScrollGroups::modifier) || sillyedit::utils::shouldBlockScrolling()) {
                return;
            }

            const auto pos = self->m_editorLayer->m_objectLayer->convertToNodeSpace(cocos::getMousePos());
            
            auto obj = self->m_editorLayer->objectAtPosition(pos);

            if (!obj || !sillyedit::utils::isTriggerFast(obj)) {
                return;
            }

            const auto primaryTarget = trigger::targetType(obj);

            if (primaryTarget == trigger::InputType::None) {
                return;
            }

            sillyedit::utils::shouldBlockScrolling() = true;

            if (fields->lastScroll.elapsed() > asp::Duration::fromMillis(ScrollGroups::scrollTimeout)) {
                fields->scrollingDistance = 0;
            }

            fields->lastScroll = asp::Instant::now();

            fields->scrollingDistance += ScrollGroups::reverseScroll ? -pY : pY;

            if (std::abs(fields->scrollingDistance) > DEFAULT_SCROLL_DISTANCE / ScrollGroups::scrollSensitivity) {
                auto dispatcher = CCKeyboardDispatcher::get();

                self->scrollGroup(
                    obj, fields->scrollingDistance > 0,
                    sillyedit::utils::modifierDown(ScrollGroups::secondaryScroll)
                );

                fields->scrollingDistance = 0;
            }
            
        }, Priority::Early);
    }
}