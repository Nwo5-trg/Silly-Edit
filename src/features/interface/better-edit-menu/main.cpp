#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

namespace BetterEditMenu {
    void EditButtonBar::loadFromItems(CCArray* objects, int rows, int columns, bool keepPage) {
        if (!BetterEditMenu::enabled() || this->getID() != "edit-tab-bar") {
            return GD::EditButtonBar::loadFromItems(objects, rows, columns, keepPage);
        }

        GD::EditButtonBar::loadFromItems(objects, 1, columns, keepPage);
    }

    void Feature::onToggled(bool pEnabled) {
        auto self = editor::ui<BetterEditMenu::EditorUI>();
        auto fields = self->m_fields.self();

        editor::updateEditorTabButtons();
    }

    void Feature::onUIUpdated(float pScale) {
        auto self = editor::ui<BetterEditMenu::EditorUI>();
        auto fields = self->m_fields.self();
    }
}