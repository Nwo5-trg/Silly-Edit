#include <nwo5.ui-scaling/include/include.hpp>
#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

namespace ZoomInput {
    void EditorUI::updateZoomInput(float pZoom = editor::zoom()) {
        if (auto input = m_fields->zoomInput) {
            input->setString(misc::numToString(pZoom));
        }
    }

    void EditorUI::updateZoomContainer() {
        if (auto container = m_fields->zoomContainer) {
            Setup(container)
                .scale(m_positionSlider->getScale() * ZoomInput::zoomInputScale)
                .pos(
                    ZoomInput::centered ? CCDirector::get()->getWinSize().width / 2 : ui::x(m_positionSlider),
                    ui::y(m_positionSlider) + (ZoomInput::zoomInputOffset * m_positionSlider->getScale())
                ); 
        }
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

        fields->zoomInput = ui::input(BASE_ZOOM_INPUT_SIZE, "1")
            .id("zoom-input"_spr)
            .filter("1234567890.");

        fields->zoomContainer = ui::menu(ui::row()
            .alignment(AxisAlignment::Center)
            .gap(0.0f)
            .autoScale(false)
            .grow()
        )
            .id("zoom-input-container"_spr)
            .height(BASE_ZOOM_INPUT_SIZE.height)
            .anchor(Anchor::Top)
            .children(
                ui::label("Zoom: ", Font::Default)
                    .id("zoom-label"_spr)
                    .scaleHeightToFit(BASE_ZOOM_INPUT_SIZE.height),
                fields->zoomInput,
                ui::circleButtonFrame(
                    ui::frame::FIND, CircleBaseColor::Green, self, menu_selector(ZoomInput::EditorUI::onZoomInputButton)
                )
                    .id("zoom-button"_spr)
                    .scaleHeightToFit(BASE_ZOOM_INPUT_SIZE.height)
                    .layoutPrevGap(5.0f)
            )
            .parent(self)
            .addTo(self->m_uiItems);

        feature.registerKeybind<"zoom-in">([self] (bool pDown, bool) {
            if (pDown) {
                self->updateZoom(editor::zoom() / 0.9f);
            }
        });

        feature.registerKeybind<"zoom-out">([self] (bool pDown, bool) {
            if (pDown) {
                self->updateZoom(editor::zoom() * 0.9f);
            }
        });
    }

    void Feature::onUIUpdated(float pScale) {
        editor::ui<ZoomInput::EditorUI>()->updateZoomContainer();
    }
}