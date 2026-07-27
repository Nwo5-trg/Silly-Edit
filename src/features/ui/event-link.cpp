#include <Geode/modify/SetupEventLinkPopup.hpp>
#include <internal/utils/utils.hpp>
#include "settings.hpp"
#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

constexpr size_t EVENTS_AMOUNT = 78;

class $modify(UISetupEventLinkPopup, SetupEventLinkPopup) {
    struct Fields {
        std::set<int> eventIDs;
        std::vector<int> pinnedEvents;

        CCMenu* togglerMenu = nullptr;
        CCMenu* pageButtonMenu = nullptr;
        size_t page = 0;
        size_t lastPage = 0;
    };

    static constexpr size_t TOGGLERS_PER_PAGE = 8;
    static constexpr float TOGGLER_SIZE = 25.0f;
    static constexpr float TOGGLER_GAP = 5.0f;
    static constexpr float LABEL_LENGTH = 70.0f;
    static constexpr float PAGE_BUTTON_SIZE = 7.5f;
    static constexpr float PAGE_ARROW_BUTTON_SIZE = 20.0f;
    static constexpr float PADDING = 20.0f;
    static constexpr float EVENT_SHORTCUTS_BACKGROUND_WIDTH = 180.0f;
    static constexpr float MISC_BUTTON_SIZE = 30.0f;
    static constexpr float MISC_BUTTON_PADDING = 5.0f;

    static void onModify(auto& self) {
        (void)self.setHookPriorityPost("SetupEventLinkPopup::init", Priority::VeryLate);
    }

    auto getPinnedEvents() {
        std::vector<int> out;

        for (const auto substr : string::splitView(Settings::UI::pinnedEvents.get(), ",")) {
            if (const auto val = utils::numFromString<int>(substr).unwrapOrDefault(); val > 0 && val <= 78) {
                out.push_back(val);
            }
        }

        return out;
    }
    auto getEventIDS() {
        std::set<int> out;

        for (size_t i = 1; i <= EVENTS_AMOUNT; i++) {
            bool enabled = true;

            for (auto obj : CCArrayExt<EventLinkTrigger*>(m_gameObjects)) {
                enabled &= obj->m_eventIDs.contains(i);
            }

            if (enabled) {
                out.insert(enabled);
            }
        }

        return out;
    }

    bool init(EventLinkTrigger* object, CCArray* objects) {
        if (!SetupEventLinkPopup::init(object, objects)) {
            return false;
        }

        if (Settings::UI::eventLink.get()) {
            setupUI();
        }
        
        return true;
    }

    void setupUI() {
        const auto info = UI::setupRobtopPopup(this);

        auto fields = m_fields.self();

        info.okButton->setVisible(false);
        info.infoButton->setVisible(false);
        info.title->setPosition(100.0f, 266.0f);

        // select event button
        Setup(m_buttonMenu->getChildByType<CCMenuItemSpriteExtra>(8))
            .pos(info.rect.size - MISC_BUTTON_PADDING)
            // look into y scaletofit woudlnt work here ????
            .scale(0.55f);

        auto bg = ui::node(Setup(NineSlice::create("GJ_square02.png"))
            .id("event-shortcuts-background"_spr)
            .pos(info.rect.size.width - EVENT_SHORTCUTS_BACKGROUND_WIDTH / 2, info.rect.size.height / 2)
            .size(EVENT_SHORTCUTS_BACKGROUND_WIDTH, info.rect.size.height)
            .parent(m_mainLayer)
        );

        auto closeButton = ui::node(Setup(ui::buttonFrame(
            "GJ_closeBtn_001.png", this, menu_selector(UISetupEventLinkPopup::onClose)
        ))
            .id("close-button"_spr)
            .pos(MISC_BUTTON_PADDING, info.rect.size.height - MISC_BUTTON_PADDING)
            .scaleToFit(MISC_BUTTON_SIZE)
            .parent(m_buttonMenu)
        );

        const UI::PropInput gidInput{51, this};
        gidInput.label->setString("Group ID:");
        UI::moveTextInput(gidInput, {100.0f, 210.0f});

        const UI::PropInput materialInput{447, this};
        materialInput.label->setString("Material ID:");
        UI::moveTextInput(materialInput, {100.0f, 145.0f});

        const UI::PropInput playerInput{525, this};
        playerInput.label->setString("Player ID:");
        UI::moveTextInput(playerInput, {100.0f, 80.0f});

        fields->togglerMenu = ui::node(Setup(ui::menu(RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
            ->setGrowCrossAxis(true)
            ->setCrossAxisAlignment(AxisAlignment::Start)
            ->setAutoScale(false)
            ->ignoreInvisibleChildren(true)
            ->setGap(TOGGLER_GAP)
        ))
            .id("toggle-menu"_spr)
            .size(TOGGLER_SIZE + TOGGLER_GAP + LABEL_LENGTH, TOGGLER_SIZE * TOGGLERS_PER_PAGE + TOGGLER_GAP * (TOGGLERS_PER_PAGE - 1))
            .parent(m_buttonMenu)
        );
        fields->togglerMenu->setPosition(bg->getPositionX(), info.rect.size.height - fields->togglerMenu->getContentHeight() / 2 - PADDING);

        fields->pinnedEvents = this->getPinnedEvents();
        fields->eventIDs = this->getEventIDS();

        const auto pinnedEvents = fields->pinnedEvents.size();
        for (size_t i = 0; i < (pinnedEvents + EVENTS_AMOUNT); i++) {
            if (i && (!(i % TOGGLERS_PER_PAGE) || i == pinnedEvents) ){
                fields->lastPage++;
            }

            const auto event = i < pinnedEvents ? fields->pinnedEvents[i] : (i - pinnedEvents) + 1;

            auto toggler = ui::node(Setup(ui::togglerBase(this, menu_selector(UISetupEventLinkPopup::onEventToggle)))
                .scaleToFit(TOGGLER_SIZE)
                .toggle(fields->eventIDs.contains(event))
                .id(nwo5::utils::numToString(fields->lastPage))
                .tag(i < pinnedEvents ? fields->pinnedEvents[i] : (i - pinnedEvents) + 1)
                .hide()
                .parent(fields->togglerMenu)
            );
            auto label = ui::node(Setup(ui::label(GJBaseGameLayer::gameEventToString(enum_cast<GJGameEvent>(event))))
                .limitScaleWidthToFit(LABEL_LENGTH)
                .id(nwo5::utils::numToString(fields->lastPage))
                .tag(i < pinnedEvents ? fields->pinnedEvents[i] : (i - pinnedEvents) + 1)
                .hide()
                .parent(fields->togglerMenu)
            );
        }

        fields->pageButtonMenu = ui::node(Setup(ui::menu(ui::horizontalDistrbLayout(
            5.0f, AxisAlignment::Center
        )))
            .id("page-button-menu"_spr)
            .pos(bg->getPositionX(), PADDING)
            .parent(m_buttonMenu)
        );

        for (size_t i = 0; i <= fields->lastPage; i++) {
            Setup(ui::buttonFrame(
                "menuCircleWhite.png", this, menu_selector(UISetupEventLinkPopup::onPageButton
            )))
                .tag(i)   
                .scaleToFit(PAGE_BUTTON_SIZE)
                .parent(fields->pageButtonMenu);
        }

        auto next = ui::node(Setup(ui::buttonFrame(
            "GJ_arrow_02_001.png", this, menu_selector(UISetupEventLinkPopup::onNextPage)
        ))
            .id("next-page-button"_spr)
            .pos(bg->getPositionX() + EVENT_SHORTCUTS_BACKGROUND_WIDTH / 2 - PADDING / 2, bg->getPositionY())
            .scaleToFit(PAGE_ARROW_BUTTON_SIZE)
            .flipX()
            .parent(m_buttonMenu)
        );

        auto prev = ui::node(Setup(ui::buttonFrame(
            "GJ_arrow_02_001.png", this, menu_selector(UISetupEventLinkPopup::onPreviousPage)
        ))
            .id("previous-page-button"_spr)
            .pos(bg->getPositionX() - EVENT_SHORTCUTS_BACKGROUND_WIDTH / 2 + PADDING / 2, bg->getPositionY())
            .scaleToFit(PAGE_ARROW_BUTTON_SIZE)
            .parent(m_buttonMenu)
        );

        UI::crushTriggerTypeButtons(this);

        goToPage(0);
    }


    void onEventToggle(CCObject* pSender) {
        auto fields = m_fields.self();

        const auto event = pSender->getTag();
        const auto toggled = nwo5::utils::isToggled(pSender);
        
        if (nwo5::utils::isToggled(pSender)) {
            fields->eventIDs.insert(event);

            for (auto obj : CCArrayExt<EventLinkTrigger*>(m_gameObjects)) {
                obj->m_eventIDs.insert(event);
            }
        }
        else {
            fields->eventIDs.erase(event);

            for (auto obj : CCArrayExt<EventLinkTrigger*>(m_gameObjects)) {
                obj->m_eventIDs.erase(event);
            }
        }

        for (auto node : fields->togglerMenu->getChildrenExt()) {
            if (node == pSender) {
                continue;
            }

            if (auto toggler = typeinfo_cast<CCMenuItemToggler*>(node)) {
                toggler->toggle(fields->eventIDs.contains(toggler->getTag()));
            }
        }
    }

    void goToPage(size_t pPage) {
        auto fields = m_fields.self();

        fields->page = pPage;

        for (auto node : fields->togglerMenu->getChildrenExt()) {
            node->setVisible(numFromString<size_t>(node->getID()).unwrapOrDefault() == pPage);
        }

        fields->togglerMenu->updateLayout();

        for (auto node : fields->pageButtonMenu->getChildrenExt<CCMenuItemSpriteExtra*>()) {
            node->setColor(node->getTag() == pPage ? ccWHITE : ccGRAY);
        }
    }

    void onPageButton(CCObject* pSender) {
        this->goToPage(pSender->getTag());
    }
    void onNextPage(CCObject*) {
        auto fields = m_fields.self();

        goToPage(fields->page == fields->lastPage ? 0 : fields->page + 1);
    }
    void onPreviousPage(CCObject*) {
        auto fields = m_fields.self();

        goToPage(!fields->page ? fields->lastPage : fields->page - 1);
    }
};