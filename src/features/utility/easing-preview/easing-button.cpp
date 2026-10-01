#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

namespace EasingPreview {
    bool EasingButton::init(EasingType pEasing, PreviewType pType, float pExponent, geode::CopyableFunction<void(EasingType)> pCallback) {
        auto bg = ui::node(Setup(NineSlice::create(ui::sprite::SQUARE))
            .id("background"_spr)
            .size(SIZE)
            .opacity(100)
        );

        if (!CCMenuItemSpriteExtra::init(bg, nullptr, nullptr, nullptr)) {
            return false;
        }

        this->scheduleUpdate();

        m_easing = pEasing;
        m_exponent = pExponent;

        Setup(this)
            .callback([easing = pEasing, callback = std::move(pCallback)] (EasingButton*) {
                callback(easing);
            })
            .id("select-easing-button"_spr);

        m_drawnode = Setup(SillyDrawNode::create())
            .id("draw"_spr)
            .size(SIZE - PADDING)
            .parent(this).center();

        m_dot = ui::spr("smallDot.png")
            .id("dot"_spr)
            .pos(ui::pos(m_drawnode) - ui::size(m_drawnode) / 2)
            .scaleToFit(DOT_SIZE)
            .opacity(0)
            .parent(this);

        m_dot->runAction(CCRepeatForever::create(CCSequence::create(
            CCFadeTo::create(EasingPreview::animationResetTime / 2, 255),
            CCSpawn::create(
                GameToolbox::getEasedAction(
                    CCMoveBy::create(EasingPreview::animationDuration, {0.0f, ui::h(m_drawnode)}),
                    enum_cast<int>(m_easing), m_exponent
                ),
                CCMoveBy::create(EasingPreview::animationDuration, {ui::w(m_drawnode), 0.0f}),
                nullptr
            ),
            CCFadeTo::create(EasingPreview::animationResetTime / 2, 0),
            CCMoveTo::create(0.0f, ui::pos(m_drawnode) - ui::size(m_drawnode) / 2),
            nullptr
        )));

        if (pType == PreviewType::None) {
            return true;
        }

        m_sprite = ui::sprFrame("square_01_001.png")
            .id("sprite"_spr)
            .scaleToFit(SPRITE_SIZE)
            .opacity(0)
            .parent(this);

        CCActionInterval* action = nullptr;
        CCActionInterval* returnAction = nullptr;

        switch (pType) {
            case PreviewType::Move: {
                m_sprite->setPosition({(SPRITE_SIZE + PADDING) / 2, SIZE.height / 2});

                action = GameToolbox::getEasedAction(
                    CCMoveTo::create(EasingPreview::animationDuration, {SIZE.width - (SPRITE_SIZE + PADDING) / 2, SIZE.height / 2}),
                    enum_cast<int>(m_easing), m_exponent
                );
                returnAction = CCMoveTo::create(0.0f, {(SPRITE_SIZE + PADDING) / 2, SIZE.height / 2});
            break; }
            case PreviewType::Rotate: {
                m_sprite->setPosition(m_drawnode->getPosition());

                action = GameToolbox::getEasedAction(
                    CCRotateBy::create(EasingPreview::animationDuration, 360.0f),
                    enum_cast<int>(m_easing), m_exponent
                );
            break; }
            case PreviewType::Scale: {
                m_sprite->setPosition(m_drawnode->getPosition());
                
                action = GameToolbox::getEasedAction(
                    CCScaleTo::create(EasingPreview::animationDuration, SIZE.height / SPRITE_SIZE - PADDING),
                    enum_cast<int>(m_easing), m_exponent
                );
                returnAction = CCScaleTo::create(0.0f, m_sprite->getScale());
            break; }
            default: {};
        }

        if (returnAction) {
            m_sprite->runAction(CCRepeatForever::create(CCSequence::create(
                CCFadeTo::create(EasingPreview::animationResetTime / 2, 255),
                action,
                CCFadeTo::create(EasingPreview::animationResetTime / 2, 0),
                returnAction,
                nullptr
            )));
        }
        else {
            m_sprite->runAction(CCRepeatForever::create(CCSequence::create(
                CCFadeTo::create(EasingPreview::animationResetTime / 2, 255),
                action,
                CCFadeTo::create(EasingPreview::animationResetTime / 2, 0),
                nullptr
            )));
        }

        this->setGraphMode(false);

        return true;
    }
    void EasingButton::update(float) {
        if (!m_drawnode || !m_drawnode->isVisible()) {
            return;
        }

        m_drawnode->clear();
    
        std::optional<CCPoint> last;
        for (size_t i = 0; i < EasingPreview::graphDetail + 1; i++) {
            const auto t = static_cast<float>(i) / EasingPreview::graphDetail;
            const CCPoint pos{-ui::size(m_drawnode) / 2 + ui::size(m_drawnode) * CCPoint{t, misc::ease(m_easing, t, m_exponent)}};

            if (!last.has_value()) {
                last = pos;

                continue;
            }

            m_drawnode->drawSegment(
                last.value(), pos, EasingPreview::graphThickness,
                EasingPreview::chroma ? sillyedit::utils::getChroma<ccColor4F>() : Col::Green
            );
            
            last = pos;
        }

        // const auto t = std::fmod(m_start.elapsed().seconds<float>() / EasingPreview::animationDuration, 1.0f);

        // m_dot->setPosition(ui::pos(m_drawnode) - ui::size(m_drawnode) / 2 + ui::size(m_drawnode) * CCPoint{t, misc::ease(m_easing, t, m_exponent)});
    }

    void EasingButton::setGraphMode(bool pEnabled) {
        if (m_drawnode && m_dot) {
            m_drawnode->setVisible(pEnabled);
            m_dot->setVisible(pEnabled);
        }
        if (m_sprite) {
            m_sprite->setVisible(!pEnabled);
        }
    }

    EasingButton* EasingButton::create(EasingType pEasing, PreviewType pType, float pExponent, geode::CopyableFunction<void(EasingType)> pCallback) {
        auto ret = new EasingButton;

        if (!ret->init(pEasing, pType, pExponent, std::move(pCallback))) {
            delete ret;

            return nullptr;
        }

        ret->autorelease();

        return ret;
    }
};