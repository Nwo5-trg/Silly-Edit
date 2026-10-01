#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

namespace ContextMenu {
    bool ContextMenuNode::init() {
        if (!CCNode::create()) {
            return false;
        }

        Setup(this)
            .id("context-menu"_spr)
            .hide();

        m_background = ui::nineSlice(ui::sprite::SQUARE)
            .id("background"_spr)
            .opacity(100)
            .parent(this);

        m_optionsMenu = ui::menu(ui::column()
            .alignment(AxisAlignment::End)
            .gap(GAP)
            .crossLineAlignment(AxisAlignment::Start)
            .autoScale(false)
            .reverse()
            .grow()
            .padding({PADDING / 2, PADDING / 2, PADDING / 2, PADDING / 2})
        )
            .id("menu"_spr)
            .parent(this);

        this->scheduleUpdate();

        return true;
    }
    void ContextMenuNode::update(float pForceUpdate) {
        const auto down = sillyedit::utils::modifierDown(ContextMenu::specialModifier);

        // we abusing implicit conversions with this one
        if (this->m_specialDown == down || pForceUpdate == true) {
            return;
        }
        
        this->m_specialDown = down;

        if (!this->isVisible()) {
            return;
        }

        for (auto [label, option] : this->m_labels) {
            Setup(label)
                .text(this->m_specialDown ? option->name.special.value() : option->name.label)
                .scaleHeightToFit(OPTION_HEIGHT)
                .limitScaleWidthToFit(LABEL_WIDTH);

            label->getParent()->updateLayout();
        }
    }

    void ContextMenuNode::setup(std::vector<const Option*> pOptions) {
        m_labels.clear();

        m_optionsMenu->removeAllChildren();
        m_optionsMenu->setContentSize({ICON_SIZE + OPTION_GAP + LABEL_WIDTH + PADDING, 0.0f});

        for (auto option : pOptions) {
            if (option->type == Option::Type::Spacer) {
                auto spacer = *ui::nineSlice("square02b_001.png")
                    .size(ui::w(m_optionsMenu) - PADDING, SPACER_HEIGHT)
                    .color(Col::LightGray)
                    .opacity(150)
                    .tag(SPACER_TAG)
                    .parent(m_optionsMenu);

                spacer->setScaleMultiplier(0.1f);

                continue;
            }

            if (!option->enabled()) {
                continue;
            }

            auto wrapper = *ui::dummy(ui::row()
                .alignment(AxisAlignment::Start)
                .gap(OPTION_GAP)
                .autoScale(false)
                .grow(false)
            )  
                .id("wrapper"_spr)
                .size(ui::w(m_optionsMenu) - PADDING, OPTION_HEIGHT)
                .children(
                    ui::spr("extra-dot.png"_spr)
                        .id("icon"_spr)
                        .scaleToFit(ICON_SIZE)
                        .color(option->color)
                );
            
            auto label = *ui::label(option->name.label, Font::Default)
                .id("label"_spr)
                .scaleHeightToFit(OPTION_HEIGHT)
                .limitScaleWidthToFit(LABEL_WIDTH)
                .parent(wrapper);

            if (option->name.special.has_value()) {
                m_labels.emplace_back(label, option);
            }

            ui::button(wrapper, nullptr)
                .id("{}-option"_spr, string::replace(string::toLower(option->name.label), " ", "-"))
                .callback([this, option] (auto) {
                    option->callback(this->m_specialDown);

                    this->hide();
                })
                .parent(m_optionsMenu);
        }

        // lol (this is so slow omg)
        if (auto node = m_optionsMenu->getChildrenExt().back(); node && node->getTag() == SPACER_TAG) {
            node->removeMeAndCleanup();
            m_optionsMenu->updateLayout();
        }

        m_background->setContentSize(ui::size(m_optionsMenu));
        this->setContentSize(ui::size(m_optionsMenu));
    }

    void ContextMenuNode::show(cocos2d::CCPoint pPos, cocos2d::CCArray* pObjects) {
        this->setup(OptionsRegistry::get()->forObjects(pObjects));

        const CCPoint anchor{
            pPos.x + ui::sw(this) + PADDING > ui::winWidth() ? 1.0f : 0.0f, 1.0f
        };

        float y = pPos.y;

        if (const float minY = editor::ui()->m_toolbarHeight + PADDING; y - ui::sh(this) < minY) {
            y = minY + ui::sh(this);
        }

        if (const float maxY = ui::winHeight() - PADDING; y > maxY) {
            y = maxY;
        }

        Setup(this)
            .anchor(pPos.x + ui::sw(this) + PADDING > ui::winWidth() ? 1.0f : 0.0f, 1.0f)
            .pos(pPos.x, y)
            .scale(ContextMenu::scaleMultiplier)
            .visible(true);

        Setup(m_optionsMenu).center();
        Setup(m_background).center();

        this->update(true);
    }
    void ContextMenuNode::hide() {
        this->setVisible(false);

        m_labels.clear();
    }

    ContextMenuNode* ContextMenuNode::create() {
        auto ret = new ContextMenuNode;

        if (!ret->init()) {
            delete ret;

            return nullptr;
        }

        ret->autorelease();

        return ret;
    }
}