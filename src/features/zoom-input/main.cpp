#include <nwo5.ui-scaling/include/include.hpp>
#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

static constexpr CCSize BASE_ZOOM_INPUT_SIZE = {20.0f, 10.0f};

void ZoomInput::EditorUI::updateZoomInput(float pZoom = editor::zoom()) {
    if (auto input = m_fields->zoomInput) {
        input->setString(nwo5::utils::numToString(pZoom));
    }
}

void ZoomInput::EditorUI::updateZoomContainer(float pScale) {
    Setup(m_fields->zoomContainer)
        .scale(m_positionSlider->getScale() * ZoomInput::zoomInputScale)
        .pos(
            ZoomInput::centered ? CCDirector::get()->getWinSize().width / 2 : m_positionSlider->getPositionX(),
            m_positionSlider->getPositionY() + (ZoomInput::zoomInputOffset * m_positionSlider->getScale())
        ); 
}

void ZoomInput::EditorUI::onZoomInputButton(CCObject*) {
    const auto val = utils::numFromString<float>(m_fields->zoomInput->getString()).unwrapOr(1.0f);

    GD::EditorUI::updateZoom(val > 0.0f ? val : 1.0f);
}

void ZoomInput::EditorUI::updateZoom(float zoom) {
    GD::EditorUI::updateZoom(zoom);

    this->updateZoomInput(zoom);
};

void ZoomInput::EditorUI::constrainGameLayerPosition(float x, float y) {
    if (ZoomInput::noConstrainPosition && !m_editorLayer->m_initializing) {
        GD::EditorUI::constrainGameLayerPosition(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
    }
    else {
        GD::EditorUI::constrainGameLayerPosition(x, y);
    }
};

bool ZoomInput::LevelEditorLayer::init(GJGameLevel* level, bool noUI) {
    if (!GD::LevelEditorLayer::init(level, noUI)) {
        return false;
    }

    // zoom gets set after editorui init
    editor::ui<ZoomInput::EditorUI>()->updateZoomInput();

    return true;
}

void ZoomInput::Feature::onEditor() {
    if (!ZoomInput::enabled()) {
        return;
    }

    auto ui = editor::ui<ZoomInput::EditorUI>();

    auto fields = ui->m_fields.self();
    
    fields->zoomContainer = ui::node(Setup(ui::menu(ui::row(AxisAlignment::Start, 0.0f)
        .autoScale(false)
    ))
        .id("zoom-input-container"_spr)
        .height(BASE_ZOOM_INPUT_SIZE.height)
        .anchor(TOP_CENTER_ANCHOR)
        .children(
            Setup(ui::label("Zoom: ", Font::Default))
                .id("zoom-label"_spr)
                .scaleHeightToFit(BASE_ZOOM_INPUT_SIZE.height),
            (fields->zoomInput = ui::node(Setup(ui::input(BASE_ZOOM_INPUT_SIZE, "1"))
                .filter("1234567890.")
                .id("zoom-input"_spr))),
            Setup(ui::circleButtonFrame(
                "edit_findBtn_001.png", CircleBaseColor::Green, ui, menu_selector(ZoomInput::EditorUI::onZoomInputButton)
            ))
                .id("zoom-button"_spr)
                .scaleHeightToFit(BASE_ZOOM_INPUT_SIZE.height)
                .prevGap(5.0f)
        )
        .parent(ui)
        .addTo(ui->m_uiItems)
    );

    nwo5::utils::setupKeybind(ui, "zoom-input-zoom-in", [ui] (const Keybind&, bool pDown, bool, double) {
        if (pDown) {
            ui->zoomGameLayer(true);
        }
    });

    nwo5::utils::setupKeybind(ui, "zoom-input-zoom-out", [ui] (const Keybind&, bool pDown, bool, double) {
        if (pDown) {
            ui->zoomGameLayer(false);
        }
    });
}

void ZoomInput::Feature::onUIUpdated(float pScale) {
    if (auto ui = editor::ui<ZoomInput::EditorUI>()) {
        ui->updateZoom(pScale);
    }
}