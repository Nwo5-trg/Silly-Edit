#include <utils/include.hpp>
#include "popup.hpp"
#include "general.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

namespace Settings {
    bool SettingsPopup::init() {
        if (!Popup::init(SIZE.width, SIZE.height, ui::sprite::EMPTY_BACKGROUND)) {
            return false;
        }

        const float POPUP_OFFSET = 10.0f / CCDirector::get()->getContentScaleFactor();

        static std::unordered_map<std::string, Theme> THEME_MAP{
            {"Default", {{152, 86, 51}, {84, 84, 84}, {104, 104, 104}}},
            {"Alt", {{192, 104, 64}, {114, 60, 35}, {150, 150, 150}}},
            {"Geode", {{25, 24, 33}, {16, 15, 21}, {21, 20, 27}}}
        };

        this->setID("settings-popup"_spr);

        auto background = *ui::spr(
            fmt::format("game_bg_{:02}_001.png", Settings::useCustomBackground ? Settings::settingsBackground : 13)
        );
        Setup(background)
            .id("background"_spr)
            .color(THEME_MAP[Settings::popupTheme].background)
            .layoutAnchor(Anchor::Center)
            .ignoreAnchorForPos(false)
            .order(-1)
            .textureRect({{0.0f, ui::h(background) - SIZE.height - POPUP_OFFSET * 2}, SIZE - POPUP_OFFSET * 2})
            .parent(m_mainLayer);

        Setup(Button::createWithNode(ButtonSprite::create("Close")))
            .id("close-button"_spr)
            .order(1)
            .scaleToFit(CLOSE_BUTTON_SIZE)
            .layoutAnchor(geode::Anchor::BottomRight).layoutAnchorOffset(-POPUP_OFFSET - PADDING, POPUP_OFFSET + PADDING)
            .callback([this] (Button* pSender) {
                this->onClose(pSender);
            })
            .userObject("nwo5.silly-api/tooltip", TooltipInfo::create("I should add commiting settings icl"))
            .parent(m_mainLayer);

        if (Settings::showTooltips) {
            m_tooltip = Setup(Tooltip::create(Font::Chat))
                .id("tooltip"_spr)
                .scale(0.35f)
                .parent(m_mainLayer);
        }

        auto sideBar = *Setup(CCLayerColor::create(
            color_cast<ccColor4B>(THEME_MAP[Settings::popupTheme].sideBar)
        ))
            .id("side-bar"_spr)
            .layout(AnchorLayout::create())
            .layoutAnchor(Anchor::Left).layoutAnchorOffsetX(POPUP_OFFSET)
            .ignoreAnchorForPos(false)
            .size(SIDE_BAR_WIDTH, SIZE.height - POPUP_OFFSET * 2)
            .order(-1)
            .parent(m_mainLayer);

        m_featuresScroll = Setup(alpha::ui::AdvancedScrollLayer::create(
            {SIDE_BAR_WIDTH, SIZE.height - TOP_BAR_HEIGHT - POPUP_OFFSET * 2}
        ))
            .id("feature-scroll"_spr)
            .layout(ui::column()
                .alignment(AxisAlignment::End)
                .gap(SIDE_BAR_LABEL_GAP)
                .crossAlignment(AxisAlignment::Start)
                .cross(false)
                .autoScale(false)
                .ignoreInvisible(false)
                .crossOverflow(false)
                .reverse()
                .grow()
                .crossLineAlignment(AxisAlignment::Start)
                .padding({PADDING, 2.5f, 0.0f, 5.0f})
            )
            .layoutAnchor(Anchor::BottomRight)
            .parent(sideBar);
        m_featuresScroll->blockTouchBehind(true);
        m_featuresScroll->setOvershoot(15.0f);

        auto featuresScrollbar = alpha::ui::AdvancedScrollBar::create(
            m_featuresScroll, alpha::ui::ScrollOrientation::VERTICAL
        );

        auto featuresScrollbarStyle = alpha::ui::RoundedScrollStyle();
        featuresScrollbarStyle.m_track = [] {
            auto track = alpha::ui::RoundedScrollTrack::create();
            
            track->setClickColor({0, 0, 0, 0});
            track->setBackgroundColor({0, 0, 0, 0});

            return track;
        };
        
        featuresScrollbar->setStyle(featuresScrollbarStyle);

        Setup(featuresScrollbar)
            .id("features-scroll-bar"_spr)
            .layoutAnchor(Anchor::BottomRight)
            .size(FEATURE_SCROLLBAR_WIDTH, ui::sh(m_featuresScroll))
            .parent(sideBar);

        auto topBar = *Setup(CCLayerColor::create(
            color_cast<ccColor4B>(THEME_MAP[Settings::popupTheme].topBar)
        ))
            .id("top-bar"_spr)
            .layout(AnchorLayout::create())
            .layoutAnchor(Anchor::Top).layoutAnchorOffsetY(-POPUP_OFFSET)
            .ignoreAnchorForPos(false)
            .size(SIZE.width - POPUP_OFFSET * 2, TOP_BAR_HEIGHT)
            .order(-1)
            .children(
                ui::sprFrame("d_gradient_c_01_001.png")
                    .id("shadow"_spr)
                    .layoutAnchor(Anchor::Bottom)
                    .anchor(Anchor::Top)
                    .flipY()
                    .opacity(100)
                    .color(Col::Black)
                    .stretchToFit(SIZE.width, TOP_BAR_SHADOW_HEIGHT)
            )
            .parent(m_mainLayer);

        m_versionLabel = ui::label(Font::Chat)
            .id("version-label"_spr)
            .layoutAnchor(Anchor::Left).layoutAnchorOffsetX(PADDING)
            .string(fmt::format("sillyedit {} <3", Mod::get()->getVersion().toVString()))
            .scaleHeightToFit(VERSION_LABEL_HEIGHT)
            .userObject("nwo5.silly-api/tooltip", TooltipInfo::create("Is it \"sillyedit\" or \"SillyEdit\" (idk !)"))
            .parent(topBar);

        m_keybindsButton = ui::button(
            ButtonSprite::create("Open Keybinds"), this, menu_selector(SettingsPopup::onKeybinds)
        )
            .id("keybinds-button"_spr)
            .scaleHeightToFit(TOP_BAR_BUTTON_SIZE)
            .userObject("nwo5.silly-api/tooltip", TooltipInfo::create("Open keybinds"));

        ui::menu(ui::row()
            .alignment(AxisAlignment::End)
            .gap(TOP_BAR_BUTTON_GAP)
            .autoScale(false)
            .grow()
            .reverse()
        )
            .id("top_bar_menu"_spr)
            .layoutAnchor(Anchor::Right).layoutAnchorOffsetX(-PADDING)
            .height(TOP_BAR_BUTTON_SIZE)
            .children(
                m_keybindsButton,
                ui::button(
                    ButtonSprite::create("Reset All"), this, menu_selector(SettingsPopup::onResetAll)
                )
                    .id("reset-all-button"_spr)
                    .scaleHeightToFit(TOP_BAR_BUTTON_SIZE)
                    .userObject("nwo5.silly-api/tooltip", TooltipInfo::create("Reset all settings")),
                ui::circleButtonFrame(
                    ui::frame::FOLDER, CircleBaseColor::Green, this, menu_selector(SettingsPopup::onOpenConfigDir)
                )
                    .id("config-dir-button"_spr)
                    .scaleToFit(TOP_BAR_BUTTON_SIZE)
                    .userObject("nwo5.silly-api/tooltip", TooltipInfo::create("Open config dir")),
                ui::circleButtonFrame(
                    ui::frame::SAVE, CircleBaseColor::Green, this, menu_selector(SettingsPopup::onOpenSaveDir)
                )
                    .id("save-dir-button"_spr)
                    .scaleToFit(TOP_BAR_BUTTON_SIZE)
                    .userObject("nwo5.silly-api/tooltip", TooltipInfo::create("Open save dir")),
                ui::circleButton(
                    ui::label("C", Font::Default), CircleBaseColor::Green, this, menu_selector(SettingsPopup::onCredits)
                )
                    .id("credits-button"_spr)
                    .scaleToFit(TOP_BAR_BUTTON_SIZE)
                    .userObject("nwo5.silly-api/tooltip", TooltipInfo::create("Credits"))
            )
            .parent(topBar);

        m_settingsScroll = Setup(alpha::ui::AdvancedScrollLayer::create(
            {SIZE - CCSize{SIDE_BAR_WIDTH, TOP_BAR_HEIGHT} - POPUP_OFFSET * 2}
        ))
            .id("settings-scroll"_spr)
            .layout(ui::row()
                .alignment(AxisAlignment::Start)
                .gap(SETTING_BUTTON_GAP)
                .crossAlignment(AxisAlignment::End)
                .autoScale(false)
                .ignoreInvisible()
                .crossOverflow()
                .padding({0.0f, PADDING + 5.0f, 0.0f, 0.0f})
            )
            .layoutAnchor(Anchor::BottomRight).layoutAnchorOffset(-POPUP_OFFSET, POPUP_OFFSET)
            .parent(m_mainLayer);
        m_settingsScroll->blockTouchBehind(true);
        m_settingsScroll->setOvershoot(20.0f);

        int index = 0;
        for (auto category : SettingsManager::get()->getCategories()) {
            auto name = category->name();

            if (const auto pos = name.find("Title"); pos != std::string::npos) {
                name.erase(name.find("Title"));

                ui::dummy(ui::row()
                    .alignment(AxisAlignment::Center)
                    .gap(FEATURE_LABEL_ICON_GAP)
                    .autoScale(false)
                    .grow()
                    .crossOverflow()
                )
                    .id("{}-title"_spr, string::replace(string::toLower(name), " ", "-"))
                    .height(TITLE_LABEL_HEIGHT)
                    .anchor(Anchor::Left)
                    .children(
                        ui::label(name, Font::Gold)
                            .scaleHeightToFit(TITLE_LABEL_HEIGHT)
                            .anchor(Anchor::Left)
                    )
                    .parent(m_featuresScroll);

                continue;
            }

            m_settingsMap[index] = {category, nullptr, {}};

            for (auto setting : category->getSettings()) {
                auto settingButton = Settings::createSettingButton(setting);

                // it returns false if setting is invalid on platform
                if (!settingButton) {
                    if (setting->name() == "Enabled") {
                        break;
                    }

                    continue;
                }

                m_settingsMap[index].buttons.push_back(
                    Setup(settingButton)
                        .parent(m_settingsScroll)
                );
            }

            auto featureButton = *ui::button(
                ui::label(name, Font::Default), this, menu_selector(SettingsPopup::onFeatureButton)
            );
                
            ui::menu(ui::row()
                .alignment(AxisAlignment::Center)
                .gap(FEATURE_LABEL_ICON_GAP)
                .autoScale(false)
                .grow()
                .crossOverflow()
            )
                .id("{}-category"_spr, string::replace(string::toLower(name), " ", "-"))
                .height(FEATURE_LABEL_HEIGHT)
                .anchor(Anchor::Left)
                .children(
                    ui::spr(category->logo())
                        .scaleHeightToFit(FEATURE_LABEL_HEIGHT),
                    Setup(featureButton)
                        .tag(index)
                        .scaleHeightToFit(FEATURE_LABEL_HEIGHT)
                        .anchor(Anchor::Left)
                        .userFlag(m_settingsMap[index].buttons.empty() ? "unavailable-on-platform"_spr : "")
                        .userObject("nwo5.silly-api/tooltip", TooltipInfo::create(category->description()))
                )
                .parent(m_featuresScroll);

            m_settingsMap[index].featureButton = featureButton;

            index++;
        }

        // i need to update it or it just breaks :3
        m_featuresScroll->updateLayout();
        m_featuresScroll->scroll(m_settingsScroll->getScrollPoint().x, m_settingsScroll->getScrollPoint().y);

        this->goToFeature(Settings::saveSettingPage ? Mod::get()->getSavedValue<int>("general-settings-page") : 0);

        m_closeBtn->setVisible(false);

        this->scheduleUpdate();

        this->addEventListener(SettingWithReloadChanged(), [this] (std::string pID, SettingReload pReload) {
            m_reloadSettingsActivated[pID] = pReload;
        });

        return true;
    }
    void SettingsPopup::update(float) {
        if (m_versionLabel) {
            m_versionLabel->setColor(misc::getChroma<ccColor3B>(1.5f, {}, 0.5f));
        }

        for (const auto& [_, tuple] : m_settingsMap) {
            if (auto button = tuple.featureButton) {
                button->setColor(button->getUserFlag("unavailable-on-platform"_spr) ? *Col::Gray : Col::White);
            }
        }

        if (m_selectedFeature < m_settingsMap.size()) {
            m_settingsMap[m_selectedFeature].featureButton->setColor(
                misc::getChroma<ccColor3B>(1.5f, 180.0f, (1.0f/3.0f))
            );
        }
    }

    void SettingsPopup::goToFeature(int pTag) {
        if (m_settingsMap[pTag].buttons.empty()) {
            return Notification::create("feature unsupported on platform 3:", NotificationIcon::Error)->show();
        }
        
        m_selectedFeature = pTag;

        Mod::get()->setSavedValue<int>("general-settings-page", m_selectedFeature);

        for (const auto& [index, tuple] : m_settingsMap) {
            for (auto button : tuple.buttons) {
                const auto visible = index == m_selectedFeature;

                button->setVisible(visible);
                button->setContentSize(visible ? SettingButtonBase::SIZE : CCSizeZero);

                // scroll layer changes visibility so this makes sure everything is actually hidden
                for (auto child : button->getChildrenExt()) {
                    child->setVisible(visible);
                }
            }
        }

        m_settingsScroll->updateLayout();
        m_settingsScroll->setScrollY(0.0f);

        if (ModSettingsManager::from(Mod::get())->get(m_settingsMap[m_selectedFeature].category->name()).get()) {
            static_cast<ButtonSprite*>(m_keybindsButton->getNormalImage())->setColor(Col::White);
            m_keybindsButton->setEnabled(true);
        }
        else {
            static_cast<ButtonSprite*>(m_keybindsButton->getNormalImage())->setColor(Col::Gray);
            m_keybindsButton->setEnabled(false);
        }
    }

    void SettingsPopup::onFeatureButton(cocos2d::CCObject* pSender) {
        this->goToFeature(pSender->getTag());
    }
    void SettingsPopup::onCredits(cocos2d::CCObject*) {
        MDPopup::create("Credits", CREDITS_STRING, "Close")->show();
    }
    void SettingsPopup::onOpenSaveDir(cocos2d::CCObject*) {
        file::openFolder(Mod::get()->getSaveDir());
    }
    void SettingsPopup::onOpenConfigDir(cocos2d::CCObject*) {
        file::openFolder(Mod::get()->getConfigDir());
    }
    void SettingsPopup::onResetAll(cocos2d::CCObject*) {
        geode::createQuickPopup(
            "Reset All",
            "are u sureeeee u wanna reset all settings, no taksies backsies",
            "Nvm", "Yes !",
            [this] (FLAlertLayer*, bool pBtn2) {
                if (!pBtn2) {
                    return;
                }

                for (auto button : m_settingsMap[m_selectedFeature].buttons) {
                    button->resetSetting();
                }
            },
            true, true
        );
    }
    void SettingsPopup::onKeybinds(cocos2d::CCObject*) {
        auto popup = geode::openSettingsPopup(Mod::get(), Settings::popupTheme != "Geode");

        if (!popup) {
            return;
        }

        auto content = popup->getChildByIDRecursive("content-layer");
        auto input = popup->getChildByIDRecursive("search-input");
        auto label = popup->m_mainLayer->getChildByType<CCLabelBMFont>();
        auto bottomMenu = popup->m_mainLayer->getChildByType<CCMenu>();

        if (!content || !input || !label || !bottomMenu) {
            return;
        }

        const auto selectedFeature = m_settingsMap[m_selectedFeature].category->name();

        static_cast<TextInput*>(input)->setPlaceholder("Search Keybinds...");

        if (selectedFeature == "General") {
            label->setString("Keybinds");

            return;
        }

        label->setString(fmt::format("{} Keybinds", selectedFeature).c_str());

        if (auto resetButton = bottomMenu->getChildByType<CCMenuItemSpriteExtra>(1)) {
            Setup(resetButton)
                .size(CCSizeZero)
                .hide();
            static_cast<ButtonSprite*>(resetButton->getNormalImage())->setOpacity(0);
        }

        struct SettingNode {
            CCNode* self = nullptr;
            CCMenuItemToggler* toggler = nullptr;
            CCLabelBMFont* label = nullptr;
        };
        std::vector<SettingNode> titleNodes;
        
        for (auto child : content->getChildrenExt()) {
            if (auto menu = child->getChildByType<CCMenu>(1); menu && menu->getChildrenCount() == 1) {
                if (auto toggler = menu->getChildByType<CCMenuItemToggler>()) {
                    if (auto label = child->getChildByType<CCMenu>()->getChildByType<CCLabelBMFont>(); label && std::string{label->getFntFile()} == Font::Gold) {
                        titleNodes.emplace_back(child, toggler, label);
                    }
                }
            }
        }

        for (const auto& node : titleNodes) {
            const auto feature = node.label->getString();

            node.self->setContentHeight(0.0f);
            for (auto child : node.self->getChildrenExt()) {
                child->setVisible(false);
            }

            if (selectedFeature != feature) {
                node.toggler->activate();
            }

            node.toggler->setVisible(false);
        }
    }

    void SettingsPopup::toggleSettingsDrag(bool pEnable) {
        m_settingsScroll->setDraggingEnabled(pEnable);
    }

    void SettingsPopup::onClose(cocos2d::CCObject* pSender) {
        if (!Settings::showReloadWarnings || m_reloadSettingsActivated.empty()) {
            return Popup::onClose(pSender);
        }
        
        std::vector<SettingReload> reloads;

        for (auto [_, reload] : m_reloadSettingsActivated) {
            reloads.push_back(reload);
        }

        Popup::onClose(pSender);

        std::string str;

        if (const auto count = std::ranges::count(reloads, SettingReload::Editor)) {
            str.append(fmt::format("{} settings that require editor reload, ", count));
        }
        if (const auto count = std::ranges::count(reloads, SettingReload::Game)) {
            str.append(fmt::format("{} settings that require game reload, ", count));
        }
        if (const auto count = std::ranges::count(reloads, SettingReload::Pause)) {
            str.append(fmt::format("{} settings that require editor pause menu reload, ", count));
        }
        if (const auto count = std::ranges::count(reloads, SettingReload::Popup)) {
            str.append(fmt::format("{} settings that require settings popup reload, ", count));
        }

        str.pop_back();
        str.pop_back();

        if (const auto i = str.find_last_of(','); i != std::string::npos) {
            str.replace(str.find_last_of(','), 1, ", and");
        }
        
        str.append(" have been changed");
        
        FLAlertLayer::create(
            "BTW",
            {str},
            "OK"
        )->show();
    }

    SettingsPopup* SettingsPopup::create() {
        auto ret = new SettingsPopup;

        if (!ret->init()) {
            delete ret;
            
            return nullptr;
        }

        ret->autorelease();

        return ret;
    }
}