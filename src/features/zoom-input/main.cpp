#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <internal/utils/utils.hpp>
#include "settings.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

// ill prolly add a seperate modifier for keybind zoom
// so remind me to replace ternary hell with a proper enum system

class $modify(ZoomInputEditorUI, EditorUI) {
    struct Fields {
        bool buttonZooming = false;
        bool zoomingGameLayer = false;
        bool zoomingIn = false;

        CCMenu* zoomContainer = nullptr;
        TextInput* zoomInput = nullptr;
    };

    static constexpr CCSize BASE_ZOOM_INPUT_SIZE = {20.0f, 10.0f};

    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) {
            return false;
        }

        if (!Settings::ZoomInput::zoomInput.get()) {
            return true;
        }

        auto fields = m_fields.self();

        auto zoomLabel = ui::node(
            Setup(ui::label("Zoom: ", Font::Default))
                .id("zoom-label"_spr)
                .scaleHeightToFit(BASE_ZOOM_INPUT_SIZE.height)
        );

        fields->zoomInput = ui::node(
            Setup(ui::input(BASE_ZOOM_INPUT_SIZE, "1"))
                .filter("1234567890.")
                .id("zoom-input"_spr)
        );

        auto zoomButton = ui::node(
            Setup(ui::circleButtonFrame(
                "edit_findBtn_001.png", CircleBaseColor::Green, this, menu_selector(ZoomInputEditorUI::onZoomInputButton)
            ))
                .id("zoom-button"_spr)
                .scaleHeightToFit(BASE_ZOOM_INPUT_SIZE.height)
        );
        zoomButton->setLayoutOptions(AxisLayoutOptions::create()
            ->setPrevGap(5.0f)
        );

        fields->zoomContainer = ui::node(
            Setup(ui::menu(nwo5::ui::horizontalDistrbLayout(0.0f)))
                .id("zoom-input-container"_spr)
                .height(BASE_ZOOM_INPUT_SIZE.height)
                .anchor(TOP_CENTER_ANCHOR)
                .scale(m_positionSlider->getScale() * Settings::ZoomInput::zoomInputScale.get())
                .pos(
                    Settings::ZoomInput::centered.get() ? CCDirector::get()->getWinSize().width / 2 : m_positionSlider->getPositionX(),
                    m_positionSlider->getPositionY() + (Settings::ZoomInput::zoomInputOffset.get() * m_positionSlider->getScale())
                )
                .children(
                    zoomLabel,
                    fields->zoomInput,
                    zoomButton
                )
                .parent(this)
        );
        m_uiItems->addObject(fields->zoomContainer);

        nwo5::utils::setupKeybind(this, "zoom-input-zoom-in", [this] (const Keybind&, bool pDown, bool, double) {
            if (pDown) {
                m_fields->zoomingGameLayer = true;

                zoomGameLayer(m_fields->zoomingIn = true);

                m_fields->zoomingGameLayer = false;
            }
        });

        nwo5::utils::setupKeybind(this, "zoom-input-zoom-out", [this] (const Keybind&, bool pDown, bool, double) {
            if (pDown) {
                m_fields->zoomingGameLayer = true;
                m_fields->zoomingIn = false;

                zoomGameLayer(m_fields->zoomingIn = false);

                m_fields->zoomingGameLayer = false;
            }
        });

        return true;
    }

    void updateZoomInput() {
        if (auto input = m_fields->zoomInput) {
            input->setString(nwo5::utils::numToString(editor::zoom()));
        }
    }

    void onZoomInputButton(CCObject* pSender) {
        const auto val = numFromString<float>(m_fields->zoomInput->getString()).unwrapOr(1.0f);

        EditorUI::updateZoom(val > 0.0f ? val : 1.0f);
    }

    void updateZoom(float zoom) {
        if (nwo5::utils::isTinkerLoaded() && Settings::ZoomInput::zoomInput.get()) {
            EditorUI::updateZoom(zoom);

            return updateZoomInput();
        }

        if (!Settings::ZoomInput::zoomBypass.get() || !m_fields->zoomingGameLayer) {
            return EditorUI::updateZoom(zoom);
        }

        const auto newZoom = 
            (Settings::ZoomInput::multiplicativeZoom.get() && (!Settings::ZoomInput::onlyButtons.get() || m_fields->buttonZooming))
                ? (m_fields->zoomingIn
                    ? editor::zoom() * Settings::ZoomInput::multiplicativeZoomMod.get()
                    : editor::zoom() / Settings::ZoomInput::multiplicativeZoomMod.get()
                  )
                : (m_fields->zoomingIn
                    ? editor::zoom() + Settings::ZoomInput::zoomStep.get()
                    : editor::zoom() - Settings::ZoomInput::zoomStep.get()
                  );

        EditorUI::updateZoom(
            m_fields->buttonZooming
                ? newZoom
                : std::clamp(newZoom, Settings::ZoomInput::zoomScrollMin.get(), Settings::ZoomInput::zoomScrollMax.get())
        );

        updateZoomInput();
    };

    void zoomIn(CCObject* sender) {
        m_fields->buttonZooming = true;
        m_fields->zoomingGameLayer = true;
        m_fields->zoomingIn = true;

        EditorUI::zoomIn(sender);

        m_fields->buttonZooming = false;
        m_fields->zoomingGameLayer = false;
    }
    void zoomOut(CCObject* sender) {
        m_fields->buttonZooming = true;
        m_fields->zoomingGameLayer = true;
        m_fields->zoomingIn = false;

        EditorUI::zoomOut(sender);

        m_fields->buttonZooming = false;
        m_fields->zoomingGameLayer = false;
    }

    void scrollWheel(float y, float x) {
        m_fields->zoomingGameLayer = true;
        m_fields->zoomingIn = y <= 0.0f && x <= 0.0f;

        EditorUI::scrollWheel(y, x);

        m_fields->zoomingGameLayer = false;
    }

    void constrainGameLayerPosition(float x, float y) {
        if (Settings::ZoomInput::zoomBypass.get() && Settings::ZoomInput::noConstrainPosition.get() && !m_editorLayer->m_initializing) {
            EditorUI::constrainGameLayerPosition(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
        }
        else {
            EditorUI::constrainGameLayerPosition(x, y);
        }
    };
};

class $modify(LevelEditorLayer) {
    bool init(GJGameLevel* level, bool noUI) {
        if (!LevelEditorLayer::init(level, noUI)) {
            return false;
        }

        // zoom gets set after editorui init
        reinterpret_cast<ZoomInputEditorUI*>(m_editorUI)->updateZoomInput();

        return true;
    }
};