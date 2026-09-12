#include <utils/include.hpp>
#include "include.hpp"
#include "shared.hpp"

using namespace geode::prelude;

namespace sillyedit::shared {
    void removePreviewObject() {
        if (!PlaceObjectPreview::enabled() || editor::notLoaded(editor::LoadedType::UI)) {
            return;
        }

        if (auto& obj = editor::ui<PlaceObjectPreview::EditorUI>()->m_fields->previewObject) {
            object::remove(obj);

            obj = nullptr;
        }
    }
}   