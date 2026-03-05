#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <internal/utils/utils.hpp>
#include "settings.hpp"

using namespace geode::prelude;
using namespace Utils::Aliases;

class $modify(ZoomInputEditorUI, EditorUI) {
    struct Fields {
        bool buttonZooming = false;
        bool zoomingGameLayer = false;
        bool zoomingIn = false;

        CCMenu* zoomContainer = nullptr;
        TextInput* zoomInput = nullptr;
    };

    static constexpr float BASE_ZOOM_INPUT_HEIGHT = 10.0f;

    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) {
            return false;
        }

        if (!Settings::ZoomInput::enabled.get() || !Settings::ZoomInput::zoomInput.get()) {
            return true;
        }

        auto fields = m_fields.self();

        fields->zoomContainer = Utils::setupNode(
            CCMenu::create(),

            SetNodeID{"zoom-input-container"_spr},
            SetNodeHeight{BASE_ZOOM_INPUT_HEIGHT},
            SetNodeAnchor{TOP_CENTER_ANCHOR},
            SetNodeScale{m_positionSlider->getScale() * Settings::ZoomInput::zoomInputScale.get()},
            SetNodePosition{
                Settings::ZoomInput::centered.get() ? CCDirector::get()->getWinSize().width / 2 : m_positionSlider->getPositionX(),
                m_positionSlider->getPositionY() + (Settings::ZoomInput::zoomInputOffset.get() * m_positionSlider->getScale())
            },
            SetNodeParent{this}
        );
        fields->zoomContainer->setLayout(AxisLayout::create()
            ->setGrowCrossAxis(false)
            ->setAutoScale(false)
            ->setGap(0.0f)
        );

        Utils::setupNode(
            CCLabelBMFont::create("Zoom: ", "bigFont.fnt"),

            SetNodeID{"zoom-label"_spr},
            SetNodeScaleWithHeight{BASE_ZOOM_INPUT_HEIGHT},
            SetNodeParent{fields->zoomContainer}
        );
        fields->zoomInput = Utils::setupNode(
            Utils::createTextInput(BASE_ZOOM_INPUT_HEIGHT * 2, BASE_ZOOM_INPUT_HEIGHT, "1"),

            SetNodeID{"zoom-input"_spr},
            SetNodeParent{fields->zoomContainer}
        );
        Utils::setupNode(
            Utils::createCircleButtonFrame(
                "edit_findBtn_001.png", CircleBaseColor::Green, 
                this, menu_selector(ZoomInputEditorUI::onZoomInputButton)
            ),
            SetNodeScaleWithHeight{BASE_ZOOM_INPUT_HEIGHT},
            SetNodeID{"zoom-button"_spr},

            SetNodeParent{fields->zoomContainer}
        )->setLayoutOptions(AxisLayoutOptions::create()
            ->setPrevGap(5.0f)
        );

        fields->zoomContainer->updateLayout();

        Utils::setupKeybind(this, "zoom-input-zoom-in", [this] (const Keybind&, bool pDown, bool, double) {
            if (pDown) {
                m_fields->zoomingGameLayer = true;

                zoomGameLayer(m_fields->zoomingIn = true);

                m_fields->zoomingGameLayer = false;
            }
        });

        Utils::setupKeybind(this, "zoom-input-zoom-out", [this] (const Keybind&, bool pDown, bool, double) {
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
            input->setString(Utils::numToString(Editor::zoom()));
        }
    }

    void onZoomInputButton(CCObject* pSender) {
        const auto val = utils::numFromString<float>(m_fields->zoomInput->getString()).unwrapOr(1.0f);

        EditorUI::updateZoom(val > 0.0f ? val : 1.0f);
    }

    void updateZoom(float zoom) {
        if (!Settings::ZoomInput::zoomBypass.get() || !m_fields->zoomingGameLayer) {
            return EditorUI::updateZoom(zoom);
        }

        const auto newZoom = 
            (Settings::ZoomInput::multiplicativeZoom.get() && (!Settings::ZoomInput::onlyButtons.get() || m_fields->buttonZooming))
                ? (m_fields->zoomingIn
                    ? Editor::zoom() * Settings::ZoomInput::multiplicativeZoomMod.get()
                    : Editor::zoom() / Settings::ZoomInput::multiplicativeZoomMod.get()
                  )
                : (m_fields->zoomingIn
                    ? Editor::zoom() + Settings::ZoomInput::zoomStep.get()
                    : Editor::zoom() - Settings::ZoomInput::zoomStep.get()
                  );

        EditorUI::updateZoom(
            m_fields->buttonZooming
                ? newZoom
                : std::clamp(newZoom, Settings::ZoomInput::zoomScrollMin.get(), Settings::ZoomInput::zoomScrollMax.get())
        );
    };

    void zoomIn(CCObject* sender) {
        m_fields->buttonZooming = true;
        m_fields->zoomingGameLayer = true;
        m_fields->zoomingIn = true;

        EditorUI::zoomIn(sender);

        m_fields->buttonZooming = false;
        m_fields->zoomingGameLayer = false;

        updateZoomInput();
    }
    void zoomOut(CCObject* sender) {
        m_fields->buttonZooming = true;
        m_fields->zoomingGameLayer = true;
        m_fields->zoomingIn = false;

        EditorUI::zoomOut(sender);

        m_fields->buttonZooming = false;
        m_fields->zoomingGameLayer = false;

        updateZoomInput();
    }
    void scrollWheel(float y, float x) {
        m_fields->zoomingGameLayer = true;
        m_fields->zoomingIn = y <= 0.0f && x <= 0.0f;

        EditorUI::scrollWheel(y, x);

        m_fields->zoomingGameLayer = false;

        updateZoomInput();
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