#include <utils/include.hpp>
#include "core/popup.hpp"
#include "popup.hpp"
#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

namespace sillyedit::settings {
    bool SettingsPopup::init() {
        if (!Popup::init(SIZE.width, SIZE.height, ui::sprite::EMPTY_BACKGROUND)) {
            return false;
        }

        Setup(m_mainLayer->getChildByType<NineSlice>())
            .id("outline"_spr)
            .order(5);

        m_closeBtn->setVisible(false);

        const float POPUP_OFFSET = 10.0f / CCDirector::get()->getContentScaleFactor();

        Setup(this)
            .id("settings-popup"_spr)
            .opacity(sillyedit::settings::popupOverlayOpacity.get());

        m_background = ui::spr("game_bg_13_001.png");

        Setup(m_background)
            .id("background"_spr)
            .layoutAnchor(Anchor::Center)
            .ignoreAnchorForPos(false)
            .order(-1)
            .textureRect({{0.0f, ui::h(m_background) - SIZE.height - POPUP_OFFSET * 2}, SIZE - POPUP_OFFSET * 2})
            .parent(m_mainLayer);

        if (sillyedit::settings::showTooltips) {
            m_tooltip = Setup(Tooltip::create(Font::Chat))
                .id("tooltip"_spr)
                .scale(0.35f)
                .parent(m_mainLayer);
        }

        m_versionLabel = ui::label(Font::Chat)
            .id("version-label"_spr)
            .layoutAnchor(Anchor::Left).layoutAnchorOffsetX(PADDING)
            .string(fmt::format("sillyedit {} <3", Mod::get()->getVersion().toVString()))
            .scaleHeightToFit(VERSION_LABEL_HEIGHT)
            .userObject("nwo5.silly-api/tooltip", TooltipInfo::create("Is it \"sillyedit\" or \"SillyEdit\" (idk !)"));

        m_keybindsButton = ui::button(
            ButtonSprite::create("Open Keybinds"), this, menu_selector(SettingsPopup::onKeybinds)
        )
            .id("keybinds-button"_spr)
            .scaleHeightToFit(TOP_BAR_BUTTON_SIZE)
            .userObject("nwo5.silly-api/tooltip", TooltipInfo::create("Open keybinds"));

        m_incompatWarningButton = ui::button(
            ui::dummy(ui::row()
                .alignment(AxisAlignment::Start)
                .gap(2.5f)
                .autoScale(false)
                .grow()
            )
                .children(
                    ui::sprFrame(ui::frame::WARNING)
                        .scaleToFit(INCOMPAT_WARNING_HEIGHT),
                    ui::label("Potential Incompat", Font::Gold)
                        .scaleHeightToFit(INCOMPAT_WARNING_HEIGHT * (3.0f/4.0f))
                ),
            this, menu_selector(SettingsPopup::onIncompatWarning)
        )
            .id("incompat-warning-button")
            .userObject("nwo5.silly-api/tooltip", TooltipInfo::create("Feature potentially conflicting with installed mods, click for more info"));

        m_topShadow = ui::sprFrame("d_gradient_c_01_001.png")
            .id("shadow"_spr)
            .layoutAnchor(Anchor::Bottom)
            .anchor(Anchor::Top)
            .flipY()
            .stretchToFit(SIZE.width, TOP_BAR_SHADOW_HEIGHT);

        m_topBar = Setup(CCLayerColor::create())
            .id("top-bar"_spr)
            .layout(AnchorLayout::create())
            .layoutAnchor(Anchor::Top).layoutAnchorOffsetY(-POPUP_OFFSET)
            .ignoreAnchorForPos(false)
            .size(SIZE.width - POPUP_OFFSET * 2, TOP_BAR_HEIGHT)
            .order(-1)
            .children(
                m_versionLabel,
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
                            .userObject("nwo5.silly-api/tooltip", TooltipInfo::create("Credits")),
                        m_incompatWarningButton
                    ),
                m_topShadow
            )
            .parent(m_mainLayer);

        m_sideBar = Setup(CCLayerColor::create())
            .id("side-bar"_spr)
            .layout(AnchorLayout::create())
            .layoutAnchor(Anchor::BottomLeft).layoutAnchorOffset(POPUP_OFFSET, POPUP_OFFSET)
            .ignoreAnchorForPos(false)
            .size(SIDE_BAR_WIDTH, SIZE.height - POPUP_OFFSET * 2 - ui::h(m_topBar))
            .order(-1)
            .parent(m_mainLayer);

        m_featuresScroll = Setup(alpha::ui::AdvancedScrollLayer::create(ui::size(m_sideBar)))
            .id("feature-scroll"_spr)
            .layout(ui::column()
                .alignment(AxisAlignment::End)
                .gap(SIDE_BAR_LABEL_GAP)
                .crossAlignment(AxisAlignment::Start)
                .cross(false)
                .autoScale(false)
                .crossOverflow(false)
                .reverse()
                .grow()
                .ignoreInvisible(false)
                .crossLineAlignment(AxisAlignment::Start)
                .padding({PADDING, 2.5f, 0.0f, 5.0f})
            )
            .layoutAnchor(Anchor::Center)
            .parent(m_sideBar);
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
            .parent(m_sideBar);

        m_settingsScroll = Setup(alpha::ui::AdvancedScrollLayer::create(
            {SIZE - CCSize{SIDE_BAR_WIDTH, TOP_BAR_HEIGHT} - POPUP_OFFSET * 2}
        ))
            .id("settings-scroll"_spr)
            .layout(ui::row()
                .alignment(AxisAlignment::Start)
                .gap(SETTING_BUTTON_GAP)
                .crossAlignment(AxisAlignment::End)
                .autoScale(false)
                .crossOverflow()
                .ignoreInvisible(false)
                .padding({0.0f, PADDING + 5.0f, 0.0f, 0.0f})
            )
            .layoutAnchor(Anchor::BottomRight).layoutAnchorOffset(-POPUP_OFFSET, POPUP_OFFSET)
            .parent(m_mainLayer);
        m_settingsScroll->blockTouchBehind(true);
        m_settingsScroll->setOvershoot(20.0f);

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

            auto& entry = m_settingsMap[name];
            entry = {FeatureManager::get()->getFeature(category->name()), category, nullptr, {}};

            for (auto setting : category->getSettings()) {
                auto settingButton = sillyedit::settings::createSettingButton(setting);

                // it returns false if setting is invalid on platform
                if (!settingButton) {
                    if (setting->name() == "Enabled") {
                        break;
                    }

                    continue;
                }

                entry.buttons.push_back(settingButton);
            }

            auto featureButton = *ui::button(
                ui::label(name, Font::Default), this, menu_selector(SettingsPopup::onFeatureButton)
            );
                
            ui::menu(ui::row()
                .alignment(AxisAlignment::Center)
                .gap(FEATURE_LABEL_ICON_GAP)
                .autoScale(false)
                .grow()
            )
                .id("{}-category"_spr, string::replace(string::toLower(name), " ", "-"))
                .height(FEATURE_LABEL_HEIGHT)
                .anchor(Anchor::Left)
                .children(
                    ui::spr(category->logo())
                        .scaleHeightToFit(FEATURE_LABEL_HEIGHT),
                    Setup(featureButton)
                        .scaleHeightToFit(FEATURE_LABEL_HEIGHT)
                        .anchor(Anchor::Left)
                        // one liner abuse :3
                        .userFlag(entry.buttons.empty() ? "unavailable-on-platform"_spr : "")
                        .userObject("category"_spr, ObjWrapper<std::string>::create(name))
                        .userObject("nwo5.silly-api/tooltip", TooltipInfo::create(category->description()))
                )
                .parent(m_featuresScroll);

            entry.featureButton = featureButton;
        }

        // i need to update it or it just breaks :3
        m_featuresScroll->updateLayout();
        m_featuresScroll->scroll(0.0f, 0.0f);

        ui::dummy()
            .id("let-me-scroll-further-down-uwu"_spr)
            .size(ui::w(m_settingsScroll->getContentLayer()), SettingButtonBase::SIZE.height * 2)
            .order(1)
            .parent(m_settingsScroll);

        auto searchSpacer = *ui::sprFrame("edit_vLine_001.png")
            .id("search-spacer"_spr)
            .stretchToFit(1.0f, FLOATING_MENU_BUTTON_SIZE)
            .hide();

        auto searchCycleDownButton = *ui::buttonFrame(ui::frame::BLUE_ARROW, this, menu_selector(SettingsPopup::onCycleSearch))
            .id("search-cycle-down-button"_spr)
            .tag(SearchCycleArrow::Down)
            .rotation(-90.0f)
            .scaleWidthToFit(SEARCH_CYCLE_BUTTON_SIZE)
            .hide()
            .layoutPrevGap(SEARCH_CYCLE_BUTTON_GAP);

        auto searchCycleUpButton = *ui::buttonFrame(ui::frame::BLUE_ARROW, this, menu_selector(SettingsPopup::onCycleSearch))
            .id("search-cycle-up-button"_spr)
            .tag(SearchCycleArrow::Up)
            .rotation(90.0f)
            .scaleWidthToFit(SEARCH_CYCLE_BUTTON_SIZE)
            .hide();

        m_searchInput = ui::input(SEARCH_INPUT_SIZE, "Query...")
            .id("search-input"_spr)
            .callback([this] (const std::string& pStr) {
                m_searchIndex = 0;
                m_currentSearch = pStr;
                
                this->updateCurrentSearch();
            })
            .hide();

        m_searchIndexLabel = ui::label("0/0", Font::Gold)
            .id("search-index-label"_spr)
            .scaleHeightToFit(FLOATING_MENU_BUTTON_SIZE)
            .hide();

        m_searchNodes = {
            searchSpacer,
            searchCycleDownButton,
            searchCycleUpButton,
            m_searchInput,
            m_searchIndexLabel
        };

        m_floatingMenuBackground = Setup(CCLayerColor::create())
            .id("floating-menu-background"_spr)
            .opacity(150)
            .size(ui::w(m_topBar) - ui::w(m_sideBar), FLOATING_MENU_BUTTON_SIZE + PADDING)
            .layout(ui::anchor())
            .layoutAnchor(geode::Anchor::BottomRight).layoutAnchorOffset(-POPUP_OFFSET, POPUP_OFFSET)
            .children(
                ui::menu(ui::row()
                    .alignment(AxisAlignment::End)
                    .gap(FLOATING_MENU_GAP)
                    .autoScale(false)
                    .grow()
                    .reverse()
                )
                    .id("floating-menu"_spr)
                    .order(10)
                    .layoutAnchor(geode::Anchor::Right).layoutAnchorOffsetX(-PADDING)
                    .children(
                        ui::button(ButtonSprite::create("Close"), this, menu_selector(SettingsPopup::onClose))
                            .id("close-button"_spr)
                            .scaleHeightToFit(FLOATING_MENU_BUTTON_SIZE)
                            .layoutAnchor(geode::Anchor::BottomRight).layoutAnchorOffset(-POPUP_OFFSET - PADDING, POPUP_OFFSET + PADDING)
                            .userObject("nwo5.silly-api/tooltip", TooltipInfo::create("I should add commiting settings icl")),
                        ui::circleButtonFrame(ui::frame::SEARCH, CircleBaseColor::Green, this, menu_selector(SettingsPopup::onOpenSearch))
                            .id("search-button"_spr)
                            .scaleToFit(FLOATING_MENU_BUTTON_SIZE)
                            .userObject("nwo5.silly-api/tooltip", TooltipInfo::create("Toogle search settings")),
                        searchSpacer,
                        searchCycleDownButton,
                        searchCycleUpButton,
                        m_searchInput,
                        m_searchIndexLabel
                    )
            )
            .parent(m_mainLayer);

        m_selectedFeature = sillyedit::settings::saveSettingPage
            ? Mod::get()->getSavedValue<std::string>("general-settings-page")
            : "General";

        this->updateTheme();
        this->updateCurrentFeature();

        this->scheduleUpdate();

        this->addEventListener(SettingWithReloadChanged(), [this] (std::string pID, SettingReload pReload) {
            m_reloadSettingsActivated[pID] = pReload;
        });

        nwo5::settings::listenForAllSavedSettingChanges(this, [this] (std::string_view, GenericSetting* pSetting) {
            this->updateTheme();
        }, "Popup");

        return true;
    }
    void SettingsPopup::update(float) {
        if (m_versionLabel) {
            m_versionLabel->setColor(misc::getChroma<ccColor3B>(1.5f, {}, 0.5f));
        }

        for (const auto& [index, tuple] : m_settingsMap) {
            if (auto button = tuple.featureButton) {
                if (button->getUserFlag("unavailable-on-platform"_spr)) {
                    button->setColor(Col::Gray);
                }
                else if (m_unmatchedFeatures.contains(index)) {
                    button->setColor(m_theme.unmatchedSettingColor);
                }
                else {
                    button->setColor(m_theme.textColor);
                }
            }
        }

        if (m_settingsMap.contains(m_selectedFeature)) {
            const auto col = m_unmatchedFeatures.contains(m_selectedFeature)
                ? misc::getChroma<ccColor3B>(1.5f, 180.0f, (1.0f/3.0f), m_theme.unmatchedSelectedFeatureDarkness)
                : misc::getChroma<ccColor3B>(1.5f, 180.0f, (1.0f/3.0f));

            m_settingsMap[m_selectedFeature].featureButton->setColor(col);
        }
    }

    void SettingsPopup::updateCurrentFeature() {
        if (!m_settingsMap.contains(m_selectedFeature)) {
            m_selectedFeature = "General";
        }

        Mod::get()->setSavedValue<std::string>("general-settings-page", m_selectedFeature);

        for (auto node : m_settingsScroll->getContentLayer()->getChildrenExt().toVector()) {
            // so i dont remove the dummy node letting me scroll further
            if (!node->getZOrder()) {
                m_settingsScroll->removeChild(node, true);
            }
        }
        
        for (const auto& button : m_settingsMap[m_selectedFeature].buttons) {
            m_settingsScroll->addChild(button);
        }

        this->updateSettingLabels();

        m_settingsScroll->updateLayout();
        m_settingsScroll->setScrollY(0.0f);

        m_incompatWarningButton->setVisible(false);

        if (auto feature = m_settingsMap[m_selectedFeature].feature) {
            if (const auto& data = feature->data(); data.hasIncompats()) {
                for (const auto& mod : data.incompatMods.list) {
                    if (Loader::get()->isModLoaded(mod)) {
                        m_incompatWarningButton->setVisible(true);

                        break;
                    }
                }
            }
        }

        m_incompatWarningButton->getParent()->updateLayout();

        if (ModSettingsManager::from(Mod::get())->get(m_settingsMap[m_selectedFeature].category->name()).get()) {
            static_cast<ButtonSprite*>(m_keybindsButton->getNormalImage())->setColor(Col::White);
            m_keybindsButton->setEnabled(true);
        }
        else {
            static_cast<ButtonSprite*>(m_keybindsButton->getNormalImage())->setColor(Col::Gray);
            m_keybindsButton->setEnabled(false);
        }
    }
    void SettingsPopup::updateCurrentSearch() {
        m_unmatchedFeatures.clear();
        m_unmatchedSettings.clear();

        m_currentSearchMatches = 0;
        
        if (m_currentSearch.empty()) {
            this->updateCurrentFeature();

            m_searchIndexLabel->setText("0/0");

            return;
        }

        CCNode* matchedFeatureButton = nullptr;
        CCNode* matchedSettingButton = nullptr;

        for (auto [featureName, tuple] : m_settingsMap) {
            if (!tuple.validOnPlatform()) {
                continue;
            }

            double weighted = 0.0;
            const auto featureMatched = sillyedit::utils::weightedFuzzyMatch(featureName, m_currentSearch, 1, weighted);

            if (featureMatched && weighted >= 60.0 + 10.0 * m_currentSearch.size()) {
                if (m_currentSearchMatches++ == m_searchIndex) {
                    m_selectedFeature = featureName;
                    matchedFeatureButton = tuple.featureButton;
                }
            }
            else {
                bool settingMatchFound = false;

                for (auto button : tuple.buttons) {
                    weighted = 0;
                    const auto settingMatched = sillyedit::utils::weightedFuzzyMatch(button->getSetting()->name(), m_currentSearch, 1, weighted);

                    if (settingMatched && weighted >= 60.0 + 10.0 * m_currentSearch.size()) {
                        settingMatchFound = true;

                        if (m_currentSearchMatches++ == m_searchIndex) {
                            m_selectedFeature = featureName;
                            matchedFeatureButton = tuple.featureButton;
                            matchedSettingButton = button;
                        }
                    }
                    else {
                        m_unmatchedSettings.insert(button);
                    }
                }

                if (!settingMatchFound) {
                    m_unmatchedFeatures.insert(featureName);
                }
            }
        }

        this->updateCurrentFeature();

        if (!m_currentSearchMatches) {
            m_searchIndexLabel->setText("0/0");
            m_searchInput->getInputNode()->getTextLabel()->setColor(Col::LightGray);

            return;
        }

        m_searchInput->getInputNode()->getTextLabel()->setColor(m_theme.textColor);
        m_searchIndexLabel->setText(fmt::format("{}/{}", m_searchIndex + 1, m_currentSearchMatches));

        m_featuresScroll->updateLayout();
        m_featuresScroll->setScrollY(
            matchedFeatureButton 
                ? ((ui::h(m_featuresScroll->getContentLayer()) - ui::y(matchedFeatureButton->getParent())) - ui::sh(matchedFeatureButton->getParent()) * 1.5f - 5.0f)
                : 0.0f
        );
        m_settingsScroll->updateLayout();
        m_settingsScroll->setScrollY(
            matchedSettingButton 
                ? ((ui::h(m_settingsScroll->getContentLayer()) - ui::y(matchedSettingButton)) - ui::sh(matchedSettingButton) / 2 - 5.0f)
                : 0.0f
        );
    }

    void SettingsPopup::onFeatureButton(CCObject* pSender) {
        auto obj = static_cast<ObjWrapper<std::string>*>(static_cast<CCNode*>(pSender)->getUserObject("category"_spr));
        const auto feature = obj ? obj->getValue() : "General";

        if (!m_settingsMap[m_selectedFeature].validOnPlatform()) {
            return Notification::create("feature unsupported on platform 3:", NotificationIcon::Error)->show();
        }

        m_selectedFeature = feature;
        
        this->updateCurrentFeature();
    }
    void SettingsPopup::onIncompatWarning(CCObject*) {
        FLAlertLayer::create(
            "Incompat warning !",
            m_settingsMap[m_selectedFeature].feature->data().incompatMessage,
            "Oki"
        )->show();
    }
    void SettingsPopup::onOpenSearch(CCObject*) {
        if (m_searchInput->isVisible()) {
            for (auto node : m_searchNodes) {
                node->setVisible(false);
            }

            m_searchInput->getParent()->updateLayout();
            m_searchInput->defocus();

            m_currentSearch = "";
            this->updateCurrentSearch();
        }
        else {
            for (auto node : m_searchNodes) {
                node->setVisible(true);
            }
            
            m_searchInput->getParent()->updateLayout();
            m_searchInput->focus();

            m_currentSearch = m_searchInput->getString();
            
            // if the search is empty i dont want settings scroll to weirdly snap to 0.0f
            if (!m_currentSearch.empty()) {
                this->updateCurrentSearch();
            }
        }
    }
    void SettingsPopup::onCycleSearch(CCObject* pSender) {
        if (pSender->getTag() == SearchCycleArrow::Up) {
            if (++m_searchIndex >= m_currentSearchMatches) {
                m_searchIndex = 0;
            }
        }
        else {
            if (--m_searchIndex < 0) {
                m_searchIndex = m_currentSearchMatches - 1;
            }
        }

        this->updateCurrentSearch();
    }
    void SettingsPopup::onCredits(CCObject*) {
        MDPopup::create("Credits", CREDITS_STRING, "Close")->show();
    }
    void SettingsPopup::onOpenSaveDir(CCObject*) {
        file::openFolder(Mod::get()->getSaveDir());
    }
    void SettingsPopup::onOpenConfigDir(CCObject*) {
        file::openFolder(Mod::get()->getConfigDir());
    }
    void SettingsPopup::onResetAll(CCObject*) {
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
    void SettingsPopup::onKeybinds(CCObject*) {
        auto popup = geode::openSettingsPopup(Mod::get(), sillyedit::settings::popupTheme != "Geode");

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

    void SettingsPopup::updateTheme() {
        // allow themes to customize unmatched feature/setting color, unmatched selected feature darkness, floating menu opacity/color (fuck it the opacity for all the menus also making floating menu color different for geode theme), top menu shadow opacity
        static StringMap<Theme> THEME_MAP{
            {"Default", {{152, 86, 51, 255}, {84, 84, 84, 255}, {104, 104, 104, 255}, {0, 0, 0, 100}, {0, 0, 0, 50}, Col::White, {127, 127, 127}, 0.5f}},
            {"Alt", {{192, 104, 64, 255}, {114, 60, 35, 255}, {150, 150, 150, 255}, {0, 0, 0, 100}, {0, 0, 0, 50}, Col::White, {127, 127, 127}, 0.5f}},
            {"Geode", {{29, 28, 38, 255}, {16, 15, 21, 255}, {22, 21, 29, 255}, {0, 0, 0, 100}, {13, 11, 16, 200}, Col::White, Col::DarkGray, 0.5f}},
            {"Catpuccin", {{17, 17, 27, 255}, {30, 30, 46, 255}, {24, 24, 37, 255}, {0, 0, 0, 100}, {13, 11, 16, 200}, {205, 214, 244}, {108, 112, 134}, 0.5f}}
        };

        if (sillyedit::settings::popupTheme == "Custom") {
            m_theme = {
                sillyedit::settings::backgroundColor,
                sillyedit::settings::sideBarColor,
                sillyedit::settings::topBarColor,
                sillyedit::settings::topShadowColor,
                sillyedit::settings::floatingMenuColor,
                sillyedit::settings::textColor,
                sillyedit::settings::unmatchedSettingColor,
                sillyedit::settings::unmatchedSelectedFeatureDarkness
            };
        }
        else {
            m_theme = THEME_MAP[sillyedit::settings::popupTheme.get()];
        }
        
        Setup(m_background)
            .color(color_cast<ccColor3B>(m_theme.background))
            .opacity(m_theme.background.a);

        m_background->setTexture(CCTextureCache::get()->addImage(
            fmt::format("game_bg_{:02}_001.png", sillyedit::settings::useCustomBackground ? sillyedit::settings::settingsBackground : 13).c_str(), true
        ));

        Setup(m_sideBar)
            .color(color_cast<ccColor3B>(m_theme.sideBar))
            .opacity(m_theme.sideBar.a);

        Setup(m_topBar)
            .color(color_cast<ccColor3B>(m_theme.topBar))
            .opacity(m_theme.topBar.a);

        Setup(m_topShadow)
            .color(color_cast<ccColor3B>(m_theme.topShadow))
            .opacity(m_theme.topShadow.a);

        Setup(m_floatingMenuBackground)
            .color(color_cast<ccColor3B>(m_theme.floatingMenuBackground))
            .opacity(m_theme.floatingMenuBackground.a);

        this->updateSettingLabels();
    }
    void SettingsPopup::updateSettingLabels() {
        for (const auto& [_, tuple] : m_settingsMap) {
            for (auto button : tuple.buttons) {
                button->getLabel()->setColor(m_unmatchedSettings.contains(button) ? m_theme.unmatchedSettingColor : m_theme.textColor);
            }
        }
    }

    void SettingsPopup::toggleSettingsDrag(bool pEnable) {
        m_settingsScroll->setDraggingEnabled(pEnable);
    }

    void SettingsPopup::onClose(CCObject* pSender) {
        if (!sillyedit::settings::showReloadWarnings || m_reloadSettingsActivated.empty()) {
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