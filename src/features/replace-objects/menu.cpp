#include "include.hpp"
#include "settings.hpp"
#include <internal/utils/utils.hpp>

#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;
using namespace Utils::Aliases;

namespace ReplaceObjects {
    FindAndReplaceMenu* FindAndReplaceMenu::create() {
        auto ret = new FindAndReplaceMenu;

        if (!ret->init()) {
            delete ret;

            return nullptr;
        }

        ret->autorelease();

        return ret;
    }

    bool FindAndReplaceMenu::init() {
        if (!CCLayer::init()) {
            return false;
        }

        setContentSize({WIDTH + BACKGROUND_BUFFER, HEIGHT + BACKGROUND_BUFFER});

        m_background = Utils::setupNode(CCScale9Sprite::create("GJ_square01.png"),
            SetNodeID{"background"_spr},
            SetNodePosition{WIDTH / 2, HEIGHT / 2},
            SetNodeSize{WIDTH + BACKGROUND_BUFFER, HEIGHT + BACKGROUND_BUFFER},
            SetNodeParent{this}
        );
        m_queryBar = Utils::setupNode(
            Utils::createTextInput(WIDTH * SEARCH_BAR_SIZE - HORIZONTAL_PADDING * 2, (HEIGHT - VERTICAL_PADDING * 2 - BAR_SPACING) / 2, "Query...", [this] (const std::string& pStr) {
                this->onQueryBarUpdated(pStr);
            }),
            
            SetNodeID{"find-bar"_spr},
            SetNodePosition{
                WIDTH * FROM_SELECTION_BUTTONS_SIZE + HORIZONTAL_PADDING + (WIDTH * SEARCH_BAR_SIZE - HORIZONTAL_PADDING * 2) / 2, 
                HEIGHT - VERTICAL_PADDING - (HEIGHT - VERTICAL_PADDING * 2 - BAR_SPACING) / 4
            },
            SetNodeParent{this}
        );
        m_queryBar->setTextAlign(TextInputAlign::Left);
        m_replaceBar = Utils::setupNode(
            Utils::createTextInput(WIDTH * SEARCH_BAR_SIZE - HORIZONTAL_PADDING * 2, (HEIGHT - VERTICAL_PADDING * 2 - BAR_SPACING) / 2, "Replace...", [this] (const std::string& pStr) {
                this->onReplaceBarUpdated(pStr);
            }),
            
            SetNodeID{"replace-bar"_spr},
            SetNodePosition{
                WIDTH * FROM_SELECTION_BUTTONS_SIZE + HORIZONTAL_PADDING + (WIDTH * SEARCH_BAR_SIZE - HORIZONTAL_PADDING * 2) / 2, 
                VERTICAL_PADDING + (HEIGHT - VERTICAL_PADDING * 2 - BAR_SPACING) / 4
            },
            SetNodeParent{this}
        );
        m_replaceBar->setTextAlign(TextInputAlign::Left);

        m_buttonMenu = Utils::setupNode(CCMenu::create(),
            SetNodeID{"button-menu"_spr},
            SetNodePosition{CCPointZero},
            SetNodeSize{CCSizeZero},
            SetNodeChildren{
                Utils::setupNode(
                    Utils::createCircleButtonFrame("square_01_001.png", CircleBaseColor::Green, CircleBaseSize::Small, this, menu_selector(FindAndReplaceMenu::onFromSelection), 0.75f),

                    SetNodeID{"from-selection-find-button"_spr},
                    SetNodeTag{0},
                    SetNodePosition{(HORIZONTAL_PADDING + (WIDTH * FROM_SELECTION_BUTTONS_SIZE - HORIZONTAL_PADDING)) / 2, m_queryBar->getPositionY()},
                    SetNodeScaleWithSize{BUTTON_SIZE}
                ),
                Utils::setupNode(
                    Utils::createCircleButtonFrame("square_01_001.png", CircleBaseColor::Green, CircleBaseSize::Small, this, menu_selector(FindAndReplaceMenu::onFromSelection), 0.75f),

                    SetNodeID{"from-selection-replace-button"_spr},
                    SetNodeTag{1},
                    SetNodePosition{(HORIZONTAL_PADDING + (WIDTH * FROM_SELECTION_BUTTONS_SIZE - HORIZONTAL_PADDING)) / 2, m_replaceBar->getPositionY()},
                    SetNodeScaleWithSize{BUTTON_SIZE}
                ),
                Utils::setupNode(
                    Utils::createCircleButtonFrame("edit_findBtn_001.png", CircleBaseColor::Green, CircleBaseSize::Small, this, menu_selector(FindAndReplaceMenu::onFind)),

                    SetNodeID{"find-button"_spr},
                    SetNodePosition{
                        WIDTH * (FROM_SELECTION_BUTTONS_SIZE + SEARCH_BAR_SIZE) + HORIZONTAL_PADDING + BUTTON_SIZE / 2,
                        m_queryBar->getPositionY()
                    },
                    SetNodeScaleWithSize{BUTTON_SIZE}
                ),
                Utils::setupNode(
                    Utils::createCircleButtonFrame("edit_eGameRotBtn_001.png", CircleBaseColor::Blue, CircleBaseSize::Small, this, menu_selector(FindAndReplaceMenu::onReplace)),

                    SetNodeID{"replace-button"_spr},
                    SetNodePosition{
                        WIDTH * (FROM_SELECTION_BUTTONS_SIZE + SEARCH_BAR_SIZE) + HORIZONTAL_PADDING + BUTTON_SIZE / 2,
                        m_replaceBar->getPositionY()
                    },
                    SetNodeScaleWithSize{BUTTON_SIZE}
                ),
                Utils::setupNode(
                    (m_selectFilterToggle = CCMenuItemToggler::create(
                        CircleButtonSprite::createWithSpriteFrameName("edit_flipYBtn_001.png", 1.0f, CircleBaseColor::Gray),
                        CircleButtonSprite::createWithSpriteFrameName("edit_flipYBtn_001.png", 1.0f, CircleBaseColor::Pink),
                        this, menu_selector(FindAndReplaceMenu::onToggleSelectFilter)
                    )),

                    SetNodeID{"select-filter-toggler"_spr},
                    SetNodePosition{
                        ((WIDTH * (FROM_SELECTION_BUTTONS_SIZE + SEARCH_BAR_SIZE) + HORIZONTAL_PADDING + BUTTON_SIZE / 2) + WIDTH - HORIZONTAL_PADDING - BUTTON_SIZE / 2) / 2,
                        m_queryBar->getPositionY()
                    },
                    SetNodeScaleWithSize{BUTTON_SIZE}
                ),
                Utils::setupNode(
                    Utils::createButtonFrame("GJ_infoIcon_001.png", this, menu_selector(FindAndReplaceMenu::onHelp)),

                    SetNodeID{"help-button"_spr},
                    SetNodePosition{
                        WIDTH - HORIZONTAL_PADDING - BUTTON_SIZE / 2,
                        m_queryBar->getPositionY()
                    },
                    SetNodeScaleWithSize{BUTTON_SIZE / 1.5f}
                ),
                Utils::setupNode(
                    CCMenuItemSpriteExtra::create(
                        (m_syntaxWarningLabel = CCLabelBMFont::create("qO|rO", "bigFont.fnt")), 
                        this, menu_selector(FindAndReplaceMenu::onShowSyntaxWarnings)
                    ),

                    SetNodeID{"syntax-warnings-button"_spr},
                    SetNodePosition{WIDTH - HORIZONTAL_PADDING, m_replaceBar->getPositionY()},
                    SetNodeScaleWithSize{BUTTON_SIZE * 2}
                )
            },
            SetNodeParent{this}
        );

        m_syntaxWarningLabel->setAnchorPoint(RIGHT_CENTER_ANCHOR);
        updateSyntaxWarningLabel();

        m_queryExpression = nullptr;

        m_useQueryAsSelectFilter = Settings::ReplaceObjects::saveSettings.get() ? Mod::get()->getSavedValue<bool>("replace-object-use-query-as-select-filter") : false;
        m_selectFilterToggle->toggle(m_useQueryAsSelectFilter);

        m_queryBar->setCommonFilter(CommonFilter::Any);
        m_queryBar->setString(Settings::ReplaceObjects::saveSettings.get() ? Mod::get()->getSavedValue<std::string>("replace-object-query-bar") : "", true);
        m_replaceBar->setCommonFilter(CommonFilter::Any);
        m_replaceBar->setString(Settings::ReplaceObjects::saveSettings.get() ? Mod::get()->getSavedValue<std::string>("replace-object-replace-bar") : "", true);

        return true;
    }

    void FindAndReplaceMenu::onQueryBarUpdated(const std::string& pStr) {
        if (!handleBarShortcuts(m_queryBar, pStr)) {
            return;
        }

        auto res = parseQuery(pStr);

        if (res.isErr()) {
            m_queryExpression = nullptr;
            m_queryError = res.unwrapErr();
        }
        else {
            m_queryExpression = std::move(res.unwrap());
            m_queryError.clear();
            
            Mod::get()->setSavedValue<std::string>("replace-object-query-bar", m_queryBar->getString());
        }

        updateSyntaxWarningLabel();
    }
    void FindAndReplaceMenu::onReplaceBarUpdated(const std::string& pStr) {
        if (!handleBarShortcuts(m_queryBar, pStr)) {
            return;
        }

        auto res = parseReplace(pStr);

        if (res.isErr()) {
            m_replaceExpressions.clear();
            m_replaceError = res.unwrapErr();
        }
        else {
            m_replaceExpressions = std::move(res.unwrap());
            m_replaceError.clear();
            
            Mod::get()->setSavedValue<std::string>("replace-object-replace-bar", m_replaceBar->getString());
        }

        updateSyntaxWarningLabel();
    }
    bool FindAndReplaceMenu::handleBarShortcuts(TextInput* pBar, const std::string& pStr) {
        if (pStr.contains("/cl")) {
            pBar->setString("", true);
            return false;
        } 
        else if (pStr.contains("/co")) {
            std::string str{pStr};
            string::replaceIP(str, "/co", "");
            clipboard::write(str);
        }
        else if (pStr.contains("/pa")) {
            pBar->setString(clipboard::read(), true);
            return false;
        }

        return true;
    }

    void FindAndReplaceMenu::updateSyntaxWarningLabel() {
        m_syntaxWarningLabel->setString(
            fmt::format("q{}|r{}", m_queryError.empty() ? "O" : "!", m_replaceError.empty() ? "O" : "!").c_str()
        );

        m_syntaxWarningLabel->setColor(std::strcmp(m_syntaxWarningLabel->getString(), "qO|rO") ? ccRED : ccWHITE);
    }

    void FindAndReplaceMenu::runReplaceExpressions(CCArray* pObjs) {
        auto copy = CCArray::create();

        copy->addObjectsFromArray(pObjs);

        if (!copy->count()) {
            return Notification::create("Tried To Replace With No Objects In Level", NotificationIcon::Warning)->show();
        }

        const auto split = string::split(Editor::ui()->copyObjects(copy, false, false), ";");
        
        std::string modifiedString;

        for (auto str : split) {
            for (const auto& expression : m_replaceExpressions) {
                expression->run(str);
            }

            modifiedString.append(str);
            modifiedString.push_back(';');
        }

        if (modifiedString.empty()) {
            return;
        }

        modifiedString.pop_back();

        Editor::Object::remove(copy, true);

        auto newObjs = Editor::ui()->pasteObjects(modifiedString, false, false);
        Editor::Selection::set(newObjs, false, true);
        Editor::Selection::repairColors(newObjs);

        Editor::update();
    }

    void FindAndReplaceMenu::onFind(CCObject* pSender) {
        const auto ret = m_useQueryAsSelectFilter;
        m_useQueryAsSelectFilter = true;

        Editor::Selection::set(Editor::Selection::empty() ? Editor::ui()->m_editorLayer->m_objects : Editor::Selection::get(), true, true);

        m_useQueryAsSelectFilter = ret;

        Editor::update();
    }
    void FindAndReplaceMenu::onReplace(CCObject* pSender) {
        if (m_replaceExpressions.empty()) {
            return Notification::create("No Replace Actions Written", NotificationIcon::Warning)->show();
        }

        if (Editor::Selection::empty()) {
            Editor::Selection::set(Editor::layer()->m_objects, false, true);
        }
        
        if (const auto size = Editor::Selection::count(); size >= Settings::ReplaceObjects::replaceWarningThreshold.get()) {
            geode::createQuickPopup("HEY !!!", fmt::format("{} objects targeted. Running replace action(s) on *every single one* will probably be very slow. Are you *sure* you want to do this", size), "YES", "NVM", [&] (FLAlertLayer*, bool pBtn2) {
                if (!pBtn2) {
                    runReplaceExpressions(Editor::Selection::get());
                }
            });
        }
        else {
            runReplaceExpressions(Editor::Selection::get());
        }
    }
    void FindAndReplaceMenu::onToggleSelectFilter(CCObject* pSender) {
        m_useQueryAsSelectFilter = Utils::isToggled(pSender);
        Mod::get()->setSavedValue<bool>("replace-object-use-query-as-select-filter", m_useQueryAsSelectFilter);
    }
    void FindAndReplaceMenu::onFromSelection(CCObject* pSender) {
        const auto ids = Editor::Object::ids(Editor::Selection::get());

        if (ids.empty()) {
            return Notification::create("No Objects Selected", NotificationIcon::Warning)->show();
        }

        auto bar = pSender->getTag() ? m_replaceBar : m_queryBar;

        if (ids.size() == 1 || bar == m_replaceBar) {
            bar->setString(bar->getString().append(fmt::format("{}{}", numToString(ids.front()), bar == m_queryBar ? "o" : "")), true);
        }
        else {
            std::string str;
            
            for (auto id : ids) {
                str.append(fmt::format("{}o||", id));
            }

            str.erase(str.size() - 2);

            bar->setString(bar->getString().append(fmt::format("({})", bar->getString(), str)), true);
        }
    }
    void FindAndReplaceMenu::onShowSyntaxWarnings(CCObject* pSender) {
        FLAlertLayer::create(
            "Parsing Results",
            fmt::format(
                "Query:\n{}\nReplace:\n{}", 
                m_queryError.empty() ? "successfully parsed" : m_queryError, 
                m_replaceError.empty() ? "successfully parsed" : m_replaceError
            ),
            "OK"
        )->show();
    }
    void FindAndReplaceMenu::onHelp(CCObject* pSender) {
        FLAlertLayer::create(
            "Help",
            "i actually cant be asked to make this a proper markdown popup so u just get a github link for now ;3 https://github.com/Nwo5-trg/silly-edit/tree/main/src/features/replace-objects/syntax.md",
            "3:"
        )->show();
    }

    bool FindAndReplaceMenu::shouldSelectFilter() const {
        return m_useQueryAsSelectFilter;
    }
    bool FindAndReplaceMenu::evaluateObject(GameObject* pObj) const {
        if (!m_queryExpression) {
            return m_queryError.empty();
        }

        return m_queryExpression->evaluate(pObj);
    }
}