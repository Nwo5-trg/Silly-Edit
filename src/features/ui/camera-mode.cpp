#include <Geode/modify/SetupCameraModePopup.hpp>
#include <internal/utils/utils.hpp>
#include "settings.hpp"
#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

// todo: name some constants and not just old magic variables from 

class $modify(UISetupCameraModePopup, SetupCameraModePopup) {
    struct Fields {
        TextInput* easingInput = nullptr;
        TextInput* paddingInput = nullptr;
        CCMenuItemToggler* editCameraToggle = nullptr;
    };

    static void onModify(auto& self) {
        (void)self.setHookPriorityPost("SetupCameraModePopup::init", Priority::VeryLate);
    }

    bool init(EffectGameObject* object, CCArray* objects) {
        if (!SetupCameraModePopup::init(object, objects)) {
            return false;
        }

        if (Settings::UI::cameraMode.get()) {
            setupUI();
        }
        
        return true;
    }

    void setupUI() {
        const auto info = UI::setupRobtopPopup(this);

        auto fields = m_fields.self();

        array::clear(m_freeModeObjects);
        array::clear(m_cameraSettingsObjects);

        // free mode toggle
        m_buttonMenu->getChildByType<CCMenuItemToggler>(0)->setPosition(30.0f, 205.0f);
        m_mainLayer->getChildByType<CCLabelBMFont>(0)->setPosition(50.0f, 205.0f);

        // edit camera
        fields->editCameraToggle = ui::node(Setup(m_buttonMenu->getChildByType<CCMenuItemToggler>(1))
            .pos(325.0f, 260.0f)
            .scale(0.6f)
        );
        Setup(m_mainLayer->getChildByType<CCLabelBMFont>(1))
            .pos(fields->editCameraToggle->getPosition() + ccp(20.0f * fields->editCameraToggle->getScale(), 0.0f))
            .scale(0.25f);
        
        // disable grid snap
        auto gridSnapButton = ui::node(Setup(m_buttonMenu->getChildByType<CCMenuItemToggler>(2))
            .pos(155.0f, 205.0f)
            .scale(1.1f)
        );
        Setup(m_mainLayer->getChildByType<CCLabelBMFont>(2))
            .pos(gridSnapButton->getPosition() + ccp(20.0f * gridSnapButton->getScale(), 0.0f))
            .scale(0.4f);
        
        // easing
        Setup(m_cameraEasingSlider)
            .pos(130.0f, 140.0f)
            .visible(true);
            
        auto easingLabel = ui::node(Setup(ui::label("Easing: ", Font::Gold))
            .id("easing-label"_spr)
            .scale(0.7f)
        );
        m_mainLayer->getChildByType<CCLabelBMFont>(3)->setVisible(false);

        fields->easingInput = ui::node(Setup(ui::input(35.0f, 20.0f, "20"))
            .id("easing-input"_spr)
            .callback([this] (const std::string& pStr) {
                this->m_cameraEasing = std::clamp(numFromString<int>(pStr).unwrapOr(20), 1, 40);
                this->updateCameraEasing();
                this->m_cameraEasingSlider->setValue((this->m_cameraEasing - 1) / 39.0f);

                this->tryUpdateEditCamera();
            })
            .filter(CommonFilter::Uint)
        );

        auto easingMenu = ui::node(Setup(ui::menu(ui::horizontalDistrbLayout(
            0.0f, AxisAlignment::Center
        )))
            .id("easing-menu"_spr)
            .pos(130.0f, m_cameraEasingSlider->getPositionY() + 30)
            .children(
                easingLabel,
                fields->easingInput
            )
            .parent(m_mainLayer)
        );

        // padding
        Setup(m_cameraPaddingSlider)
            .pos(130.0f, 80.0f)
            .visible(true);

        auto paddingLabel = ui::node(Setup(ui::label("Padding: ", Font::Gold))
            .id("padding-label"_spr)
            .scale(0.7f)
        );
        m_mainLayer->getChildByType<CCLabelBMFont>(4)->setVisible(false);

        fields->paddingInput = ui::node(Setup(ui::input(35.0f, 20.0f, "20"))
            .id("padding-input"_spr)
            .callback([this] (const std::string& pStr) {
                this->m_cameraPadding = std::clamp(numFromString<float>(pStr).unwrapOr(0.5f), 0.0f, 1.0f);
                this->updateCameraPadding();
                this->m_cameraPaddingSlider->setValue(this->m_cameraPadding);

                this->tryUpdateEditCamera();
            })
            .filter("1234567890.")
        );

        auto paddingMenu = ui::node(Setup(ui::menu(ui::horizontalDistrbLayout(
            0.0f, AxisAlignment::Center
        )))
            .id("padding-menu"_spr)
            .pos(130.0f, m_cameraPaddingSlider->getPositionY() + 30)
            .children(
                paddingLabel,
                fields->paddingInput
            )
            .parent(m_mainLayer)
        );

        auto togglerMenu = ui::node(Setup(ui::menu(RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
            ->setGrowCrossAxis(true)
            ->setCrossAxisAlignment(AxisAlignment::Center)
            ->setAutoScale(false)
            ->ignoreInvisibleChildren(true)
            ->setGap(7.5f)
        ))
            .id("toggler-menu"_spr)
            .size(100.0f, info.rect.size.height)
            .pos(315.0f, info.rect.size.height / 2)
            .children(
                Setup(ui::togglerBase(
                    this, menu_selector(UISetupCameraModePopup::onToggler)
                ))
                    .id("preview-edges-toggle"_spr)
                    .tag(13)
                    .scale(0.85f)
                    .toggle(this->getProp(13)),
                Setup(ui::label("Preview Edges"))
                    .id("preview-edges-label"_spr)
                    .scale(0.4f)
                    .breakLine(),
                Setup(ui::togglerBase(
                    this, menu_selector(UISetupCameraModePopup::onToggler)
                ))
                    .id("no-touch-toggle"_spr)
                    .tag(121)
                    .scale(0.85f)
                    .toggle(this->getProp(121)),
                Setup(ui::label("No Touch"))
                    .id("no-touch-label"_spr)
                    .scale(0.4f)
                    .breakLine(),
                Setup(ui::togglerBase(
                    this, menu_selector(UISetupCameraModePopup::onToggler)
                ))
                    .id("no-effects-toggle"_spr)
                    .tag(116)
                    .scale(0.85f)
                    .toggle(this->getProp(116)),
                Setup(ui::label("No Effects"))
                    .id("no-effects-label"_spr)
                    .scale(0.4f)
                    .breakLine(),
                Setup(ui::togglerBase(
                    this, menu_selector(UISetupCameraModePopup::onToggler)
                ))
                    .id("no-particles-toggle"_spr)
                    .tag(507)
                    .scale(0.85f)
                    .toggle(this->getProp(507)),
                Setup(ui::label("No Particles"))
                    .id("no-particles-label"_spr)
                    .scale(0.4f)
                    .breakLine()
            )
            .parent(m_buttonMenu)
        );

        UI::crushTriggerTypeButtons(this);

        this->updateInputs();
    }

    void onToggler(CCObject* pSender) {
        for (auto obj : CCArrayExt<EffectGameObject*>(m_gameObjects)) {
            switch (pSender->getTag()) {
                case 13: {
                    obj->m_shouldPreview = nwo5::utils::isToggled(pSender);
                return; }
                case 121: { 
                    obj->m_isNoTouch = nwo5::utils::isToggled(pSender);
                return; }
                case 116: {  
                    obj->m_hasNoEffects = nwo5::utils::isToggled(pSender);
                return; }
                case 507: {
                    obj->m_hasNoParticles = nwo5::utils::isToggled(pSender);
                return; }
            };
        }
    }
    bool getProp(int pProp) {
        bool enabled = true;

        for (auto obj : CCArrayExt<EffectGameObject*>(m_gameObjects)) {
            switch (pProp) {
                case 13: {
                    enabled &= obj->m_shouldPreview;
                break;; }
                case 121: { 
                    enabled &= obj->m_isNoTouch;
                break; }
                case 116: {  
                    enabled &= obj->m_hasNoEffects;
                break; }
                case 507: {
                    enabled &= obj->m_hasNoParticles;
                break; }
            };
        }

        return enabled;
    }

    void updateInputs() {
        auto fields = m_fields.self();

        if (fields->easingInput) {
            fields->easingInput->setString(nwo5::utils::numToString(m_cameraEasing));
        }
        
        if (fields->paddingInput) {
            fields->paddingInput->setString(nwo5::utils::numToString(m_cameraPadding, 2));
        }
    }

    void tryUpdateEditCamera() {
        if (auto toggle = m_fields->editCameraToggle; toggle && !m_cameraSettingsEnabled) {
            this->onEditCameraSettings(toggle);
            toggle->toggle(true);
        }
    }

    void sliderChanged(CCObject* sender) {
        SetupCameraModePopup::sliderChanged(sender);

        this->updateInputs();
    }
    
    void onUnboundMode(CCObject* sender) {
        SetupCameraModePopup::onUnboundMode(sender);

        this->tryUpdateEditCamera();
    }
};