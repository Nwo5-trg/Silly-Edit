#include <Geode/modify/GJScaleControl.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <internal/utils/utils.hpp>
#include "settings.hpp"

using namespace geode::prelude;
using namespace Utils::Aliases;

class $modify(BetterScaleGJScaleControl, GJScaleControl) {
    struct Fields {
        std::vector<CCNode*> scaleNodes;
        std::vector<CCNode*> scaleXYNodes;

        CCLabelBMFont* newScaleLabel = nullptr;
        TextInput* scaleInput = nullptr;
        CCLabelBMFont* newScaleXLabel = nullptr;
        TextInput* scaleXInput = nullptr;
        CCLabelBMFont* newScaleYLabel = nullptr;
        TextInput* scaleYInput = nullptr;

        std::vector<float> shortcuts;
        CCMenu* shortcutsMenu = nullptr;
        CCMenu* shortcutsXMenu = nullptr;
        CCMenu* shortcutsYMenu = nullptr;

        CCMenu* extrasMenu = nullptr;

        bool betterScaleLoaded = false;
    };

    static constexpr CCSize INPUT_SIZE = {45.0f, 20.0f};
    static constexpr float LABEL_SCALE = 0.75f;

    static constexpr float SHORTCUT_SIZE = 20.0f;
    static constexpr float SHORTCUT_SPACE = 30.0f;
    static constexpr float SHORTCUT_GAP = 5.0f;

    static constexpr float EXTRAS_BUTTON_SIZE = 30.0f;
    static constexpr float EXTRAS_GAP = 5.0f;

    static constexpr float DEFAULT_SLIDER_Y_HEIGHT = 60.0f;
    static constexpr float DEFAULT_LABEL_Y_HEIGHT = 90.0f;

    static constexpr float DEFAULT_LOCK_HEIGHT = 60.0f;
    static constexpr float DEFAULT_LOCK_XY_HEIGHT = 120.0f;

    void customScale(float pScale, ObjectScaleType pType) {
        const auto num = Settings::BetterScale::allowNegative.get() ? pScale : std::abs(pScale);

        switch (pType) {
            case ObjectScaleType::XY: {
                Editor::Object::scale(Editor::Selection::get(), num, true, Editor::AUTO_CENTER, !m_scaleLocked);

                m_sliderXY->setValue(valueFromScale(num));
            break; }
            case ObjectScaleType::X: {
                Editor::Object::scaleX(Editor::Selection::get(), num, true, Editor::AUTO_CENTER, !m_scaleLocked);

                m_sliderX->setValue(valueFromScale(num));
            break; }
            case ObjectScaleType::Y: {
                Editor::Object::scaleY(Editor::Selection::get(), num, true, Editor::AUTO_CENTER, !m_scaleLocked);

                m_sliderY->setValue(valueFromScale(num)); 
            break; }
        }

        Editor::update(false);
    }

    bool init() {
        if (!GJScaleControl::init()) {
            return false;
        }

        if (!Settings::BetterScale::enabled.get()) {
            return true;
        }

        auto fields = m_fields.self();

        m_scaleLabel->setOpacity(0);

        fields->newScaleLabel = Utils::setupNode(
            CCLabelBMFont::create("Scale: ", "bigFont.fnt"),

            SetNodeID{"new_scale-label"_spr},
            SetNodeScale{LABEL_SCALE},
            SetNodePosition{-INPUT_SIZE.width / 2, m_scaleLabel->getPositionY()},
            SetNodeParent{this}
        );
        fields->scaleNodes.push_back(fields->newScaleLabel);

        fields->scaleInput = Utils::setupNode(
            Utils::createTextInput(INPUT_SIZE.width, INPUT_SIZE.height, "1", [this] (const std::string& pStr) {
                if (!pStr.empty()) {
                    const auto num = utils::numFromString<float>(pStr).unwrapOrDefault();

                    customScale(num, ObjectScaleType::XY);

                    updateInputValues();
                }
            }),

            SetNodeID{"scale-input"_spr},
            SetNodePosition{fields->newScaleLabel->getScaledContentWidth() / 2, m_scaleLabel->getPositionY()},
            SetNodeParent{this}
        );
        fields->scaleNodes.push_back(fields->scaleInput);

        fields->shortcutsMenu = Utils::setupNode(
            CCMenu::create(),

            SetNodeID{"shortcuts-menu"_spr},
            SetNodeTag{static_cast<int>(ObjectScaleType::XY)},
            SetNodeHeight{SHORTCUT_SIZE},
            SetNodePosition{0.0f, fields->newScaleLabel->getPositionY() + SHORTCUT_SPACE},
            SetNodeParent{this}
        );
        fields->shortcutsMenu->setLayout(AxisLayout::create()
            ->setGrowCrossAxis(false)
            ->setAutoScale(false)
            ->setGap(SHORTCUT_GAP)
        );
        fields->scaleNodes.push_back(fields->shortcutsMenu);

        m_scaleXLabel->setOpacity(0);

        fields->newScaleXLabel = Utils::setupNode(
            CCLabelBMFont::create("ScaleX: ", "bigFont.fnt"),

            SetNodeID{"new-scale-x-label"_spr},
            SetNodeScale{LABEL_SCALE},
            SetNodePosition{-INPUT_SIZE.width / 2, m_scaleXLabel->getPositionY()},
            SetNodeParent{this}
        );
        fields->scaleXYNodes.push_back(fields->newScaleXLabel);

        fields->scaleXInput = Utils::setupNode(
            Utils::createTextInput(INPUT_SIZE.width, INPUT_SIZE.height, "1", [this] (const std::string& pStr) {
                if (!pStr.empty()) {
                    const auto num = utils::numFromString<float>(pStr).unwrapOrDefault();
                    
                    customScale(num, ObjectScaleType::X);

                    updateInputValues();
                }
            }),

            SetNodeID{"scale-x-input"_spr},
            SetNodePosition{fields->newScaleXLabel->getScaledContentWidth() / 2, m_scaleXLabel->getPositionY()},
            SetNodeParent{this}
        );
        fields->scaleXYNodes.push_back(fields->scaleXInput);

        fields->shortcutsXMenu = Utils::setupNode(
            CCMenu::create(),

            SetNodeID{"shortcuts-x-menu"_spr},
            SetNodeTag{static_cast<int>(ObjectScaleType::X)},
            SetNodeHeight{SHORTCUT_SIZE},
            SetNodePosition{0.0f, fields->newScaleXLabel->getPositionY() + SHORTCUT_SPACE},
            SetNodeParent{this}
        );
        fields->shortcutsXMenu->setLayout(AxisLayout::create()
            ->setGrowCrossAxis(false)
            ->setAutoScale(false)
            ->setGap(SHORTCUT_GAP)
        );
        fields->scaleXYNodes.push_back(fields->shortcutsXMenu);

        m_scaleYLabel->setOpacity(0);

        fields->newScaleYLabel = Utils::setupNode(
            CCLabelBMFont::create("ScaleY: ", "bigFont.fnt"),

            SetNodeID{"new-scale-y-label"_spr},
            SetNodeScale{LABEL_SCALE},
            SetNodePosition{-INPUT_SIZE.width / 2, m_scaleYLabel->getPositionY()},
            SetNodeParent{this}
        );
        fields->scaleXYNodes.push_back(fields->newScaleYLabel);

        fields->scaleYInput = Utils::setupNode(
            Utils::createTextInput(INPUT_SIZE.width, INPUT_SIZE.height, "1", [this] (const std::string& pStr) {
                if (!pStr.empty()) {
                    const auto num = utils::numFromString<float>(pStr).unwrapOrDefault();
                    
                    customScale(num, ObjectScaleType::Y);

                    updateInputValues();
                }
            }),

            SetNodeID{"scale-y-input"_spr},
            SetNodePosition{fields->newScaleYLabel->getScaledContentWidth() / 2, m_scaleYLabel->getPositionY()},
            SetNodeParent{this}
        );
        fields->scaleXYNodes.push_back(fields->scaleYInput);

        fields->shortcutsYMenu = Utils::setupNode(
            CCMenu::create(),

            SetNodeID{"shortcuts-y-menu"_spr},
            SetNodeTag{static_cast<int>(ObjectScaleType::Y)},
            SetNodeHeight{SHORTCUT_SIZE},
            SetNodePosition{0.0f, DEFAULT_LABEL_Y_HEIGHT + SHORTCUT_SPACE * 2},
            SetNodeParent{this}
        );
        fields->shortcutsYMenu->setLayout(AxisLayout::create()
            ->setGrowCrossAxis(false)
            ->setAutoScale(false)
            ->setGap(SHORTCUT_GAP)
        );
        fields->scaleXYNodes.push_back(fields->shortcutsYMenu);

        m_scaleLockButton->getParent()->setVisible(false);

        fields->extrasMenu = Utils::setupNode(
            CCMenu::create(),

            SetNodeID{"extras-menu"_spr},
            SetNodeHeight{EXTRAS_BUTTON_SIZE},
            SetNodePositionX{0.0f},
            SetNodeParent{this}
        );
        fields->extrasMenu->setLayout(AxisLayout::create()
            ->setGrowCrossAxis(false)
            ->setAutoScale(false)
            ->setGap(EXTRAS_GAP)
        );

        Utils::setupNode(
            CCMenuItemToggler::create(
                Settings::BetterScale::newLockTexture 
                    ? CircleButtonSprite::createWithSprite("unlocked-icon.png"_spr, 1.0f, CircleBaseColor::Gray)
                    : CCSprite::createWithSpriteFrameName("warpLockOffBtn_001.png"),
                Settings::BetterScale::newLockTexture 
                    ? CircleButtonSprite::createWithSprite("locked-icon.png"_spr, 1.0f, CircleBaseColor::Blue)
                    : CCSprite::createWithSpriteFrameName("warpLockOnBtn_001.png"),
                this, menu_selector(GJScaleControl::onToggleLockScale)
            ),

            SetNodeID{"lock-button"_spr},
            SetNodeScaleWithSize{EXTRAS_BUTTON_SIZE},
            SetNodeParent{fields->extrasMenu}
        );

        if (Settings::BetterScale::switchModeButton) {
            Utils::setupNode(
                Utils::createCircleButtonFrame(
                    "GJ_sortIcon_001.png", CircleBaseColor::Pink, 
                    this, menu_selector(BetterScaleGJScaleControl::onSwitchMode)
                ),

                SetNodeID{"switch-mode-button"_spr},
                SetNodeScaleWithSize{EXTRAS_BUTTON_SIZE},
                SetNodeParent{fields->extrasMenu}
            );
        }

        fields->extrasMenu->updateLayout();

        updateShortcuts();

        fields->betterScaleLoaded = true;

        return true;
    }

    void updateShortcuts() {
        auto fields = m_fields.self();
        fields->shortcuts.clear();

        fields->shortcutsMenu->removeAllChildren();
        fields->shortcutsXMenu->removeAllChildren();
        fields->shortcutsYMenu->removeAllChildren();

        if (Settings::BetterScale::shortcutsString.get().empty()) {
            return;
        }

        const auto split = string::split(Settings::BetterScale::shortcutsString.get(), ",");

        for (auto menu : {fields->shortcutsMenu, fields->shortcutsXMenu, fields->shortcutsYMenu}) {
            for (int i = 0; i < split.size(); i++) {
                Utils::setupNode(
                    CCMenuItemSpriteExtra::create(
                        CircleButtonSprite::create(CCLabelBMFont::create(split[i].c_str(), "bigFont.fnt")),
                        this, menu_selector(BetterScaleGJScaleControl::onScaleShortcut)
                    ),

                    SetNodeTag{i},
                    SetNodeScaleWithSize{SHORTCUT_SIZE},
                    SetNodeParent{menu}
                );
            }

            menu->updateLayout();
        }

        for (const auto& str : split) {
            fields->shortcuts.push_back(utils::numFromString<float>(str).unwrapOr(1.0f));
        }
    }

    void ccTouchMoved(CCTouch* touch, CCEvent* event) {
        GJScaleControl::ccTouchMoved(touch, event);

        if (m_fields->betterScaleLoaded) {
            updateInputValues();
        }
    }

    void updateInputValues() {
        auto fields = m_fields.self();

        CCSize max{std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest()};

        for (auto obj : Editor::Selection::getExt()) {
            max.width = std::max(max.width, obj->m_scaleX);
            max.height = std::max(max.height, obj->m_scaleY);
        }

        fields->scaleInput->setString(Utils::numToString(std::max(max.width, max.height)));
        fields->scaleXInput->setString(Utils::numToString(max.width));
        fields->scaleYInput->setString(Utils::numToString(max.height));
    }

    void updateCustomNodes() {
        auto fields = m_fields.self();

        const auto scaleVisible = m_scaleLabel->isVisible();

        for (auto node : fields->scaleNodes) {
            node->setVisible(scaleVisible);
        }
        for (auto node : fields->scaleXYNodes) {
            node->setVisible(!scaleVisible);
        }
        
        if (Settings::BetterScale::shortcutsString.get().empty()) {
            fields->extrasMenu->setPositionY(scaleVisible ? DEFAULT_LOCK_HEIGHT : DEFAULT_LOCK_XY_HEIGHT);

            fields->scaleYInput->setPositionY(DEFAULT_LABEL_Y_HEIGHT);
            fields->newScaleYLabel->setPositionY(DEFAULT_LABEL_Y_HEIGHT);
            m_sliderY->setPositionY(DEFAULT_SLIDER_Y_HEIGHT);
        }
        else {
            fields->extrasMenu->setPositionY(
                scaleVisible ? (DEFAULT_LOCK_HEIGHT + SHORTCUT_SPACE) : (DEFAULT_LOCK_XY_HEIGHT + SHORTCUT_SPACE * 2)
            );

            fields->scaleYInput->setPositionY(DEFAULT_LABEL_Y_HEIGHT + SHORTCUT_SPACE);
            fields->newScaleYLabel->setPositionY(DEFAULT_LABEL_Y_HEIGHT + SHORTCUT_SPACE);
            m_sliderY->setPositionY(DEFAULT_SLIDER_Y_HEIGHT + SHORTCUT_SPACE);
        }
    }

    void onScaleShortcut(CCObject* pSender) {
        customScale(
            m_fields->shortcuts[pSender->getTag()], 
            static_cast<ObjectScaleType>(static_cast<CCNode*>(pSender)->getParent()->getTag())
        );

        updateInputValues();
    }

    void onSwitchMode(CCObject* pSender) {
        Editor::activateScaleControl(!m_scaleLabel->isVisible());
    }
};

class $modify(EditorUI) {
    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) {
            return false;
        }

        Utils::setupKeybind(this, "better-scale-activate-scale-control", [this] (const Keybind&, bool pDown, bool, double) {
            if (Settings::BetterScale::enabled.get() && pDown && !Editor::Selection::empty()) {
                if (m_scaleControl && m_scaleControl->isVisible() && m_scaleControl->m_scaleLabel->isVisible()) {
                    deactivateScaleControl();
                }
                else {
                    Editor::activateScaleControl(false);
                }
            }
        });
        Utils::setupKeybind(this, "better-scale-activate-scale-xy-control", [this] (const Keybind&, bool pDown, bool, double) {
            if (Settings::BetterScale::enabled.get() && pDown && !Editor::Selection::empty()) {
                if (m_scaleControl && m_scaleControl->isVisible() && !m_scaleControl->m_scaleLabel->isVisible()) {
                    deactivateScaleControl();
                }
                else {
                    Editor::activateScaleControl(true);
                }
            }
        });

        return true;
    }

    void activateScaleControl(CCObject* sender) {
        if (!Settings::BetterScale::enabled.get()) {
            return EditorUI::activateScaleControl(sender);
        }

        EditorUI::activateScaleControl(sender);

        if (auto control = reinterpret_cast<BetterScaleGJScaleControl*>(m_scaleControl); control && control->m_fields->betterScaleLoaded) {
            control->setScale(
                Settings::BetterScale::lockControlSize.get() 
                    ? ((1 / Editor::zoom()) * Settings::BetterScale::controlSize.get()) 
                    : Settings::BetterScale::controlSize.get()
            );
            control->setPosition(Editor::Selection::center() + CCPoint{0.0f, Settings::BetterScale::controlOffset});
            control->updateCustomNodes();
            control->updateInputValues();
        }
    }
    
    void updateScaleControl() {
        if (!Settings::BetterScale::enabled.get()) {
            return EditorUI::updateScaleControl();
        }

        EditorUI::updateScaleControl();

        if (auto control = reinterpret_cast<BetterScaleGJScaleControl*>(m_scaleControl); control && control->m_fields->betterScaleLoaded) {
            control->updateCustomNodes();
            control->updateInputValues();
        }
    }
};