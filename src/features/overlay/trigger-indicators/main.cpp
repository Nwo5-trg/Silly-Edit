#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

namespace TriggerIndicators {
    void LevelEditorLayer::updateDebugDraw() {
        GD::LevelEditorLayer::updateDebugDraw();
    }





    void Feature::onEditor() {
        auto self = editor::layer<TriggerIndicators::LevelEditorLayer>();
    }
}