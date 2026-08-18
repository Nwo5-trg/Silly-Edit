#include <nwo5.ui-scaling/include/include.hpp>
#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

namespace ZoomInput {
    void EditorUI::updateZoomInput(float pZoom = editor::zoom()) {
        if (auto input = m_fields->zoomInput) {
            input->setString(nwo5::utils::numToString(pZoom));
        }
    }

    void EditorUI::updateZoomContainer() {
        Setup(m_fields->zoomContainer)
            .scale(m_positionSlider->getScale() * ZoomInput::zoomInputScale)
            .pos(
                ZoomInput::centered ? CCDirector::get()->getWinSize().width / 2 : m_positionSlider->getPositionX(),
                m_positionSlider->getPositionY() + (ZoomInput::zoomInputOffset * m_positionSlider->getScale())
            ); 
    }

    void EditorUI::onZoomInputButton(CCObject*) {
        const auto val = utils::numFromString<float>(m_fields->zoomInput->getString()).unwrapOr(1.0f);

        GD::EditorUI::updateZoom(val > 0.0f ? val : 1.0f);
    }



    void EditorUI::updateZoom(float zoom) {
        GD::EditorUI::updateZoom(zoom);

        this->updateZoomInput(zoom);
    };

    void EditorUI::constrainGameLayerPosition(float x, float y) {
        if (ZoomInput::noConstrainPosition && !m_editorLayer->m_initializing) {
            GD::EditorUI::constrainGameLayerPosition(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
        }
        else {
            GD::EditorUI::constrainGameLayerPosition(x, y);
        }
    };





    bool LevelEditorLayer::init(GJGameLevel* level, bool noUI) {
        if (!GD::LevelEditorLayer::init(level, noUI)) {
            return false;
        }

        // zoom gets set after editorui init
        editor::ui<ZoomInput::EditorUI>()->updateZoomInput();

        return true;
    }





    void Feature::onEditor() {
        auto self = editor::ui<ZoomInput::EditorUI>();
        auto fields = self->m_fields.self();

        if (!ZoomInput::enabled()) {
            return;
        }

        fields->zoomInput = Setup(ui::input(BASE_ZOOM_INPUT_SIZE, "1"))
            .id("zoom-input"_spr)
            .filter("1234567890.");

        fields->zoomContainer = Setup(ui::menu(ui::row(AxisAlignment::Center, 0.0f)
            .autoScale(false)
            .grow(true)
        ))
            .id("zoom-input-container"_spr)
            .height(BASE_ZOOM_INPUT_SIZE.height)
            .anchor(Anchor::Top)
            .children(
                Setup(ui::label("Zoom: ", Font::Default))
                    .id("zoom-label"_spr)
                    .scaleHeightToFit(BASE_ZOOM_INPUT_SIZE.height),
                fields->zoomInput,
                Setup(ui::circleButtonFrame(
                    "edit_findBtn_001.png", CircleBaseColor::Green, self, menu_selector(ZoomInput::EditorUI::onZoomInputButton)
                ))
                    .id("zoom-button"_spr)
                    .scaleHeightToFit(BASE_ZOOM_INPUT_SIZE.height)
                    .prevGap(5.0f)
            )
            .parent(self)
            .addTo(self->m_uiItems);

        nwo5::utils::setupKeybind(self, "zoom-input-zoom-in", [self] (const Keybind&, bool pDown, bool, double) {
            if (pDown) {
                self->zoomGameLayer(true);
            }
        });

        nwo5::utils::setupKeybind(self, "zoom-input-zoom-out", [self] (const Keybind&, bool pDown, bool, double) {
            if (pDown) {
                self->zoomGameLayer(false);
            }
        });
    }

    void Feature::onUIUpdated(float pScale) {
        editor::ui<ZoomInput::EditorUI>()->updateZoomContainer();
    }
}