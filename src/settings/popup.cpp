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
            {"Custom BG", {{192, 104, 64}, {114, 60, 35}, {150, 150, 150}}},
            {"Geode", {{25, 24, 33}, {16, 15, 21}, {21, 20, 27}}}
        };

        this->setID("settings-popup"_spr);

        auto background = CCSprite::create(
            fmt::format("game_bg_{:02}_001.png", Settings::useCustomBackground ? Settings::settingsBackground : 13).c_str()
        );
        Setup(background)
            .id("background"_spr)
            .color(THEME_MAP[Settings::popupTheme].background)
            .optionsAnchor(Anchor::Center)
            .ignoreAnchorForPos(false)
            .order(-1)
            .textureRect({{0.0f, ui::h(background) - SIZE.height - POPUP_OFFSET * 2}, SIZE - POPUP_OFFSET * 2})
            .parent(m_mainLayer);

        Setup(Button::createWithNode(ButtonSprite::create("Close")))
            .id("close-button"_spr)
            .scaleToFit(CLOSE_BUTTON_SIZE)
            .anchor(Anchor::BottomRight)
            .optionsAnchor(geode::Anchor::BottomRight)
            .anchorOffset(-POPUP_OFFSET - EDGE_PADDING, POPUP_OFFSET + EDGE_PADDING)
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

        auto sideBar = ui::node(Setup(CCLayerColor::create(
            color_cast<ccColor4B>(THEME_MAP[Settings::popupTheme].sideBar)
        ))
            .id("side-bar"_spr)
            .layout(AnchorLayout::create())
            .anchor(Anchor::Left)
            .optionsAnchor(Anchor::Left)
            .anchorOffsetX(POPUP_OFFSET)
            .ignoreAnchorForPos(false)
            .size(SIDE_BAR_WIDTH, SIZE.height - POPUP_OFFSET * 2)
            .order(-1)
            .parent(m_mainLayer)
        );

        m_featuresScroll = Setup(alpha::ui::AdvancedScrollLayer::create(
            {SIDE_BAR_WIDTH, SIZE.height - TOP_BAR_HEIGHT - POPUP_OFFSET * 2}
        ))
            .id("feature-scroll"_spr)
            .layout(ui::column(AxisAlignment::End, SIDE_BAR_LABEL_GAP)
                .autoScale(false)
                .ignoreInvisible(false)
                .crossOverflow(false)
                .reverse()
                .grow(true)
                .crossAlignment(AxisAlignment::Start)
                .crossLineAlignment(AxisAlignment::Start)
                .cross(false)
                .padding({EDGE_PADDING, 2.5f, 0.0f, 5.0f})
            )
            .anchor(Anchor::BottomRight)
            .optionsAnchor(Anchor::BottomRight)
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
            .anchor(Anchor::BottomRight)
            .optionsAnchor(Anchor::BottomRight)
            .size(FEATURE_SCROLLBAR_WIDTH, ui::sh(m_featuresScroll))
            .parent(sideBar);

        auto topBar = ui::node(Setup(CCLayerColor::create(
            color_cast<ccColor4B>(THEME_MAP[Settings::popupTheme].topBar)
        ))
            .id("top-bar"_spr)
            .layout(AnchorLayout::create())
            .anchor(Anchor::Top)
            .optionsAnchor(Anchor::Top)
            .anchorOffsetY(-POPUP_OFFSET)
            .ignoreAnchorForPos(false)
            .size(SIZE.width - POPUP_OFFSET * 2, TOP_BAR_HEIGHT)
            .order(-1)
            .children(
                Setup(CCSprite::createWithSpriteFrameName("d_gradient_c_01_001.png"))
                    .id("shadow"_spr)
                    .anchor(Anchor::Top)
                    .optionsAnchor(Anchor::Bottom)
                    .flipY()
                    .opacity(100)
                    .color(ccBLACK)
                    .stretchToFit(SIZE.width, TOP_BAR_SHADOW_HEIGHT)
            )
            .parent(m_mainLayer)
        );

        m_versionLabel = Setup(ui::label(Font::Chat))
            .id("version-label"_spr)
            .anchor(Anchor::Left)
            .optionsAnchor(Anchor::Left)
            .anchorOffsetX(EDGE_PADDING)
            .string(fmt::format("sillyedit {} <3", Mod::get()->getVersion().toVString()))
            .scaleHeightToFit(VERSION_LABEL_HEIGHT)
            .userObject("nwo5.silly-api/tooltip", TooltipInfo::create("Is it \"sillyedit\" or \"SillyEdit\" (idk !)"))
            .parent(topBar);

        m_keybindsButton = Setup(ui::button(
            ButtonSprite::create("Open Keybinds"), this, menu_selector(SettingsPopup::onKeybinds)
        ))
            .id("keybinds-button"_spr)
            .scaleHeightToFit(TOP_BAR_BUTTON_SIZE)
            .userObject("nwo5.silly-api/tooltip", TooltipInfo::create("Open keybinds"));

        Setup(ui::menu(ui::row(AxisAlignment::End, TOP_BAR_BUTTON_GAP)
            .autoScale(false)
            .grow(true)
            .reverse()
        ))
            .id("top_bar_menu"_spr)
            .anchor(Anchor::Right)
            .optionsAnchor(Anchor::Right)
            .anchorOffsetX(-EDGE_PADDING)
            .height(TOP_BAR_BUTTON_SIZE)
            .children(
                m_keybindsButton,
                Setup(ui::circleButtonFrame(
                    ui::frame::FOLDER, CircleBaseColor::Green, this, menu_selector(SettingsPopup::onOpenConfigDir)
                ))
                    .id("config-dir-button"_spr)
                    .scaleToFit(TOP_BAR_BUTTON_SIZE)
                    .userObject("nwo5.silly-api/tooltip", TooltipInfo::create("Open config dir")),
                Setup(ui::circleButtonFrame(
                    ui::frame::SAVE, CircleBaseColor::Green, this, menu_selector(SettingsPopup::onOpenSaveDir)
                ))
                    .id("save-dir-button"_spr)
                    .scaleToFit(TOP_BAR_BUTTON_SIZE)
                    .userObject("nwo5.silly-api/tooltip", TooltipInfo::create("Open save dir")),
                Setup(ui::circleButton(
                    ui::label("C"), CircleBaseColor::Green, this, menu_selector(SettingsPopup::onCredits)
                ))
                    .id("credits-button"_spr)
                    .scaleToFit(TOP_BAR_BUTTON_SIZE)
                    .userObject("nwo5.silly-api/tooltip", TooltipInfo::create("Credits"))
            )
            .parent(topBar);

        m_settingsScroll = Setup(alpha::ui::AdvancedScrollLayer::create(
            {SIZE - CCSize{SIDE_BAR_WIDTH, TOP_BAR_HEIGHT} - POPUP_OFFSET * 2}
        ))
            .id("settings-scroll"_spr)
            .layout(ui::row(AxisAlignment::Start, SETTING_BUTTON_GAP, AxisAlignment::End)
                .autoScale(false)
                .ignoreInvisible()
                .crossOverflow()
                .padding({0.0f, EDGE_PADDING + 5.0f, 0.0f, 0.0f})
            )
            .anchor(Anchor::BottomRight)
            .optionsAnchor(Anchor::BottomRight)
            .anchorOffset(-POPUP_OFFSET, POPUP_OFFSET)
            .parent(m_mainLayer);
        m_settingsScroll->blockTouchBehind(true);
        m_settingsScroll->setOvershoot(20.0f);

        int index = 0;
        for (auto category : SettingsManager::get()->getCategories()) {
            auto name = category->name();

            if (const auto pos = name.find("Title"); pos != std::string::npos) {
                name.erase(name.find("Title"));

                Setup(ui::dummy())
                    .id("{}-title"_spr, string::replace(string::toLower(name), " ", "-"))
                    .layout(ui::row(AxisAlignment::Center, FEATURE_LABEL_ICON_GAP)
                        .autoScale(false)
                        .grow(true)
                        .crossOverflow()
                    )
                    .height(TITLE_LABEL_HEIGHT)
                    .anchor(Anchor::Left)
                    .children(
                        Setup(ui::label(name, Font::Gold))
                            .scaleHeightToFit(TITLE_LABEL_HEIGHT)
                            .anchor(Anchor::Left)
                    )
                    .parent(m_featuresScroll);

                continue;
            }

            auto featureButton = ui::button(
                ui::label(name), this, menu_selector(SettingsPopup::onFeatureButton)
            );
                
            Setup(ui::menu(ui::row(AxisAlignment::Center, FEATURE_LABEL_ICON_GAP)
                .autoScale(false)
                .grow(true)
                .crossOverflow()
            ))
                .id("{}-category"_spr, string::replace(string::toLower(name), " ", "-"))
                .height(FEATURE_LABEL_HEIGHT)
                .anchor(Anchor::Left)
                .children(
                    Setup(CCSprite::create(category->logo().c_str()))
                        .scaleHeightToFit(FEATURE_LABEL_HEIGHT),
                    Setup(featureButton)
                        .tag(index)
                        .scaleHeightToFit(FEATURE_LABEL_HEIGHT)
                        .anchor(Anchor::Left)
                        .userObject("nwo5.silly-api/tooltip", TooltipInfo::create(category->description()))
                )
                .parent(m_featuresScroll);

            m_featureButtons[index] = featureButton;

            m_settingsMap[index] = {{}, category};
            for (auto setting : category->getSettings()) {
                auto settingButton = Settings::createSettingButton(setting);

                // it returns false if setting is invalid on platform
                if (!settingButton) {
                    if (setting->name() == "Enabled") {
                        break;
                    }

                    continue;
                }

                m_settingsMap[index].first.push_back(
                    Setup(settingButton)
                        .parent(m_settingsScroll)
                );
            }

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
        if (m_selectedFeature < m_featureButtons.size()) {
            m_featureButtons[m_selectedFeature]->setColor(
                nwo5::utils::getChroma<ccColor3B>(1.5f, 180.0f, (1.0f/3.0f))
            );
        }
        if (m_versionLabel) {
            m_versionLabel->setColor(nwo5::utils::getChroma<ccColor3B>(1.5f, {}, 0.5f));
        }
    }

    void SettingsPopup::goToFeature(int pTag) {
        if (m_settingsMap[pTag].first.empty()) {
            return Notification::create("feature unsupported on platform 3:", NotificationIcon::Error)->show();
        }
        m_selectedFeature = pTag;

        Mod::get()->setSavedValue<int>("general-settings-page", m_selectedFeature);

        for (auto [index, button] : m_featureButtons) {
            button->setColor(m_settingsMap[index].first.size() ? ccWHITE : ccGRAY);
        }

        for (const auto& [index, pair] : m_settingsMap) {
            for (auto button : pair.first) {
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

        if (ModSettingsManager::from(Mod::get())->get(m_settingsMap[m_selectedFeature].second->name()).get()) {
            static_cast<ButtonSprite*>(m_keybindsButton->getNormalImage())->setColor(ccWHITE);
            m_keybindsButton->setEnabled(true);
        }
        else {
            static_cast<ButtonSprite*>(m_keybindsButton->getNormalImage())->setColor(ccGRAY);
            m_keybindsButton->setEnabled(false);
        }
    }

    void SettingsPopup::onFeatureButton(cocos2d::CCObject* pSender) {
        this->goToFeature(pSender->getTag());
    }
    void SettingsPopup::onCredits(cocos2d::CCObject*) {
        MDPopup::create(
            "Credits",
R"(tyyyy <cr>\<3</c> !

## Special Thanks
### Alpha
- made tinker
- replace obj impl
- setting popup inspo
- some general help with stuff

### Ery
- geode gremlin
- pr for obj tab icons
- permission to use them as assets
- prolly accepting this mod

### HJFod
- made better edit
- let me steal a bunch of stuff
- let me have a bunch of other stuff

## Credits

### gdjayy
- replace object suggestion

### CreatorCreepy
- feedback for replace object
- feedback for floodfill

### CarlIsBored
- trigger id search suggestion

### like all the hosts of cornbread megacollab
- better select all suggestion

### Doranell
- text obj utils suggestion

### DasshuDev
- copy particle string idea
)",     
            "Close"
        )->show();
    }
    void SettingsPopup::onOpenSaveDir(cocos2d::CCObject*) {
        file::openFolder(Mod::get()->getSaveDir());
    }
    void SettingsPopup::onOpenConfigDir(cocos2d::CCObject*) {
        file::openFolder(Mod::get()->getConfigDir());
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

        const auto selectedFeature = m_settingsMap[m_selectedFeature].second->name();

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