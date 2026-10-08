#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;
using namespace alpha::prelude;

namespace BetterEditMenu {
    bool EditMenu::init() {
        if (!CCNode::init()) {
            return false;
        }

        Setup(this)
            .layout(ui::anchor())
            .anchor(Anchor::Center);

        m_buttonsScroll = Setup(AdvancedScrollLayer::create(CCSizeZero))
            .layout(ui::row()
                .alignment(AxisAlignment::Start)
                .autoScale(false)
                .gap(2.5f)
                .grow()
                .cross(false)
                .ignoreInvisible(false)
                // so the scaling animation doesnt get clipped
                .padding({5.0f, 5.0f, 5.0f, 5.0f})
            )
                .id("buttons-scroll"_spr);
        m_buttonsScroll->setKeyboardEnabled(false);
        m_buttonsScroll->setOvershoot(BUTTONS_SCROLL_OVERSHOOT);
        m_buttonsScroll->setVerticalScroll(false);
        m_buttonsScroll->setHorizontalScroll(true);
        m_buttonsScroll->blockTouchBehind(true);

        this->addEventListener(AdvancedScrollLayer::ScrollEvent(m_buttonsScroll), [this] (auto, auto) {
            this->updateArrowButtons();
        });

        m_leftArrow = ui::buttonFrame(ui::frame::BLUE_ARROW, this, menu_selector(EditMenu::onButtonArrow))
            .id("left-button"_spr)
            .scaleToFit(BUTTONS_SCROLL_ARROW_SIZE)
            .tag(0);

        m_rightArrow = ui::buttonFrame(ui::frame::BLUE_ARROW, this, menu_selector(EditMenu::onButtonArrow))
            .id("right-button"_spr)
            .scaleToFit(BUTTONS_SCROLL_ARROW_SIZE)
            .flipX()
            .tag(1);

        m_buttonsMenu = ui::menu(ui::row()
            .alignment(AxisAlignment::Center)
            .autoScale(false)
            .gap(BUTTONS_SCROLL_ARROW_GAP)
            .grow()
        )
            .id("buttons-menu"_spr)
            .layoutAnchor(Anchor::Bottom).layoutAnchorOffsetY(EDGE_PADDING / 2)
            .children(
                m_leftArrow,
                m_buttonsScroll,
                m_rightArrow
            )
            .parent(this);

        m_moveShortcutsMenu = ui::menu(ui::row()
            .alignment(AxisAlignment::Start)
            .crossAlignment(AxisAlignment::End)
            .gap(MOVE_SHORTCUT_GAP)
            .autoScale(false)
        )
            .id("move-shortcuts-menu"_spr)
            .size(MOVE_SHORTCUT_SIZE * 3 + MOVE_SHORTCUT_GAP * 2, MOVE_SHORTCUT_SIZE * 2 + MOVE_SHORTCUT_GAP);

        const auto moveInputDefaultString = misc::numToString(BetterEditMenu::moveInputDefault.get());

        m_moveAmountInput = ui::input(MOVE_AMOUNT_INPUT_SIZE, moveInputDefaultString)
            .id("move-amount-input"_spr)
            .filter("1234567890.")
            .text(moveInputDefaultString);

        m_moveMenu = ui::menu(ui::row()
            .alignment(AxisAlignment::Start)
            .reverse()
            .autoScale(false)
            .grow()
            .crossOverflow(false)
        )
            .id("move-menu"_spr)
            .height(MAIN_MENU_HEIGHT)
            .layoutAnchor(Anchor::TopRight).layoutAnchorOffset(-EDGE_PADDING / 2, -EDGE_PADDING / 2)
            .children(
                m_moveShortcutsMenu,
                ui::menu(ui::row()
                    .alignment(AxisAlignment::Start)
                    .crossAlignment(AxisAlignment::End)
                    .gap(MOVE_INPUT_MENU_GAP)
                    .autoScale(false)
                )
                    .id("move-input-menu"_spr)
                    .size(MOVE_AMOUNT_INPUT_SIZE.width, MOVE_INPUT_EXTRA_SIZE * 2 + MOVE_INPUT_MENU_GAP)
                    .children(
                        m_moveAmountInput,
                        // temp as i figure out what to do with these
                        ui::circleButtonFrame(
                            ui::frame::HORIZONTAL_ARROWS, CircleBaseColor::Green, 
                            this, menu_selector(EditMenu::onFlip)
                        )
                            .tag(FlipButton::X)
                            .scaleToFit(MOVE_INPUT_EXTRA_SIZE),
                        ui::circleButtonFrame(
                            ui::frame::VERTICAL_ARROWS, CircleBaseColor::Green,
                            this, menu_selector(EditMenu::onFlip)
                        )
                            .tag(FlipButton::Y)
                            .scaleToFit(MOVE_INPUT_EXTRA_SIZE)
                    ),
                ui::menu(ui::column()
                    .alignment(AxisAlignment::Center)
                    .crossAlignment(AxisAlignment::Start)
                    .gap(MOVE_BUTTON_GAP)
                    .autoScale(false)
                )
                    .id("move-buttons-menu"_spr)
                    .size(MOVE_BUTTON_SIZE * 3 + MOVE_BUTTON_GAP * 2, MOVE_BUTTON_SIZE * 2 + MOVE_BUTTON_GAP)
                    .children(
                        // this ordering doesnt have any logic behind it icl its js wat works after a bit of trial and error
                        ui::button(
                            EditorButtonSprite::createWithSprite("direction-5.png"_spr, 0.85f, EditorBaseColor::Green),
                            this, menu_selector(EditMenu::onMoveArrow)
                        )
                            .tag(MoveButton::Right)
                            .scaleToFit(MOVE_BUTTON_SIZE)
                            .layoutBreakLine(),
                        ui::button(
                            EditorButtonSprite::createWithSprite("direction-7.png"_spr, 0.85f, EditorBaseColor::Green),
                            this, menu_selector(EditMenu::onMoveArrow)
                        )
                            .tag(MoveButton::Down)
                            .scaleToFit(MOVE_BUTTON_SIZE),
                        ui::button(
                            EditorButtonSprite::createWithSprite("direction-1.png"_spr, 0.85f, EditorBaseColor::Green),
                            this, menu_selector(EditMenu::onMoveArrow)
                        )
                            .tag(MoveButton::Up)
                            .scaleToFit(MOVE_BUTTON_SIZE)
                            .layoutBreakLine(),
                        ui::button(
                            EditorButtonSprite::createWithSprite("direction-3.png"_spr, 0.85f, EditorBaseColor::Green),
                            this, menu_selector(EditMenu::onMoveArrow)
                        )
                            .tag(MoveButton::Left)
                            .scaleToFit(MOVE_BUTTON_SIZE)
                    )
            )
            .parent(this);

        const auto rotationInputDefaultString = misc::numToString(BetterEditMenu::rotationInputDefault.get());

        m_rotationAmountInput = ui::input(ROTATION_INPUT_SIZE, rotationInputDefaultString)
            .id("rotation-amount-input"_spr)
            .filter("1234567890.")
            .text(rotationInputDefaultString);

        m_rotationLock = ui::togglerFrame("warpLockOffBtn_001.png", "warpLockOnBtn_001.png", this, nullptr)
            .id("rotation-lock-toggle"_spr)
            .scaleToFit(ROTATION_LOCK_SIZE);

        m_rotationShortcutsMenu = ui::menu(ui::row()
            .alignment(AxisAlignment::Start)
            .crossAlignment(AxisAlignment::End)
            .gap(ROTATION_SHORTCUT_GAP)
            .autoScale(false)
        )
            .id("move-input-menu"_spr)
            .size(ROTATION_INPUT_SIZE.width + ROTATION_SHORTCUT_GAP + ROTATION_SHORTCUT_SIZE, ROTATION_SHORTCUT_SIZE * 2 + ROTATION_SHORTCUT_GAP)
            .children(
                m_rotationLock,
                m_rotationAmountInput
            );


        m_rotationMenu = ui::menu(ui::row()
            .alignment(AxisAlignment::Start)
            .autoScale(false)
            .grow()
            .crossOverflow(false)
        )
            .id("rotation-menu"_spr)
            .height(MAIN_MENU_HEIGHT)
            .layoutAnchor(Anchor::TopLeft).layoutAnchorOffset(EDGE_PADDING / 2, -EDGE_PADDING / 2)
            .children(
                m_rotationShortcutsMenu,
                ui::menu(ui::row()
                    .alignment(AxisAlignment::Start)
                    .gap(ROTATION_BUTTON_GAP)
                    .autoScale(false)
                    .grow()
                )
                    .id("rotation-buttons-menu"_spr)
                    .children(
                        ui::button(
                            EditorButtonSprite::createWithSpriteFrameName("rotate-ccw.png"_spr, 0.85f, EditorBaseColor::Green),
                            this, menu_selector(EditMenu::onRotationArrow)
                        )
                            .tag(RotationButton::CCW)
                            .scaleToFit(ROTATION_BUTTON_SIZE),
                        ui::button(
                            EditorButtonSprite::createWithSpriteFrameName("rotate-cw.png"_spr, 0.85f, EditorBaseColor::Green),
                            this, menu_selector(EditMenu::onRotationArrow)
                        )
                            .tag(RotationButton::CW)
                            .scaleToFit(ROTATION_BUTTON_SIZE)
                    )
            )
            .parent(this);

        this->createMoveShortcuts();
        this->createRotationShortcuts();
        this->position();

        return true;
    }

    void EditMenu::updateArrowButtons() {
        const auto scrollX = m_buttonsScroll->getScrollPoint().x;

        m_leftArrow->setColor(Col::White);
        m_leftArrow->setEnabled(true);
        m_rightArrow->setColor(Col::White);
        m_rightArrow->setEnabled(true);

        if (scrollX < 5.0f) {
            m_leftArrow->setColor(Col::Gray);
            m_leftArrow->setEnabled(false);
        }
        if (scrollX > (m_buttonsScroll->getHorizontalMax() - 5.0f)) {
            m_rightArrow->setColor(Col::Gray);
            m_rightArrow->setEnabled(false);
        }

        const auto hide = !m_rightArrow->isEnabled() && !m_leftArrow->isEnabled();
        m_leftArrow->setVisible(!BetterEditMenu::hideArrowsIfUnused || !hide);
        m_rightArrow->setVisible(!BetterEditMenu::hideArrowsIfUnused || !hide);
    }

    void EditMenu::onMoveArrow(cocos2d::CCObject* pSender) {
        if (selection::empty()) {
            return;
        }

        // parsing input every arrow clcik insteaed of whenever input is updated is dumb but :3c
        auto res = utils::numFromString<float>(m_moveAmountInput->getString());

        if (res.isErr()) {
            return;
        }

        const auto val = res.unwrap();

        // my api has a bug i cant fix until 2.209 so
        auto objs = editor::ui()->m_selectedObjects;

        if (auto selectedObj = editor::ui()->m_selectedObject) {
            objs = CCArray::createWithObject(selectedObj);
        }

        switch (pSender->getTag()) {
            case MoveButton::Left: {
                object::moveBy(objs, {-val, 0.0f}, true);
            break; }
            case MoveButton::Up: {
                object::moveBy(objs, {0.0f, val}, true);
            break; }
            case MoveButton::Down: {
                object::moveBy(objs, {0.0f, -val}, true);
            break; }
            case MoveButton::Right: {
                object::moveBy(objs, {val, 0.0f}, true);
            break; }
            default: {};
        }

        editor::update();
    }
    void EditMenu::onMoveShortcut(cocos2d::CCObject* pSender) {
        const auto value = static_cast<ObjWrapper<float>*>(static_cast<CCNode*>(pSender)->getUserObject("value"_spr));
        m_moveAmountInput->setString(misc::numToString(value->getValue()));
    }
    void EditMenu::onRotationArrow(cocos2d::CCObject* pSender) {
        if (selection::empty()) {
            return;
        }

        auto res = utils::numFromString<float>(m_rotationAmountInput->getString());

        if (res.isErr()) {
            return;
        }

        const auto val = res.unwrap();

        auto objs = editor::ui()->m_selectedObjects;

        if (auto selectedObj = editor::ui()->m_selectedObject) {
            objs = CCArray::createWithObject(selectedObj);
        }

        switch (pSender->getTag()) {
            case RotationButton::CW: {
                object::rotateBy(objs, val, true, AUTO_CENTER, !m_rotationLock->isToggled());
            break; }
            case RotationButton::CCW: {
                object::rotateBy(objs, -val, true, AUTO_CENTER, !m_rotationLock->isToggled());
            break; }
            default: {};
        }

        editor::update();
    }
    void EditMenu::onRotationShortcut(cocos2d::CCObject* pSender) {
        const auto value = static_cast<ObjWrapper<float>*>(static_cast<CCNode*>(pSender)->getUserObject("value"_spr));
        m_rotationAmountInput->setString(misc::numToString(value->getValue()));
    }
    void EditMenu::onFlip(cocos2d::CCObject* pSender) {
        switch (pSender->getTag()) {
            case FlipButton::X: {
                editor::ui()->transformObjectCall(EditCommand::FlipX);
            break; }
            case FlipButton::Y: {
                editor::ui()->transformObjectCall(EditCommand::FlipY);
            break; }
            default: {};
        }

        editor::update();
    }
    void EditMenu::onButtonArrow(CCObject* pSender) {
        const auto scrollX = m_buttonsScroll->getScrollPoint().x;

        if (pSender->getTag()) {
            m_buttonsScroll->setScrollX(scrollX + ui::w(m_buttonsScroll), true);
        }
        else {
            m_buttonsScroll->setScrollX(scrollX - ui::w(m_buttonsScroll), true);
        }

        this->updateArrowButtons();
    }

    void EditMenu::createMoveShortcuts() {
        m_moveShortcutsMenu->removeAllChildrenWithCleanup(true);

        auto createButton = [this] (std::string_view pStr, int pTag) {
            auto split = string::splitView(pStr, ";");
            float value = 0.0f;
            std::string text;

            if (!split.empty()) {
                value = utils::numFromString<float>(split.front()).unwrapOrDefault();

                if (split.size() > 1) {
                    text = split.back();
                }
                else {
                    text = split.front();
                }
            }

            m_moveShortcutsMenu->addChild(
                ui::circleButton(
                    Label::create(text, Font::Default), CircleBaseColor::Green, this, menu_selector(EditMenu::onMoveShortcut)
                )
                    .scaleToFit(MOVE_SHORTCUT_SIZE)
                    .tag(pTag)
                    .userObject("value"_spr, ObjWrapper<float>::create(value))
            );
        };

        createButton(BetterEditMenu::moveShortcut1.get(), 1);
        createButton(BetterEditMenu::moveShortcut2.get(), 2);
        createButton(BetterEditMenu::moveShortcut3.get(), 3);
        createButton(BetterEditMenu::moveShortcut4.get(), 4);
        createButton(BetterEditMenu::moveShortcut5.get(), 5);
        createButton(BetterEditMenu::moveShortcut6.get(), 6);

        m_moveShortcutsMenu->updateLayout();
        m_moveMenu->updateLayout();
    }
    void EditMenu::createRotationShortcuts() {
        // this entire loop could js be avoided if i wouldnt compine the shortcuts menu with the input but im lazy !
        for (auto node : m_rotationShortcutsMenu->getChildrenExt().toVector()) {
            if (typeinfo_cast<CCMenuItemSpriteExtra*>(node)) {
                node->removeMeAndCleanup();
            }
        }

        auto createButton = [this] (std::string_view pStr, int pTag) {
            auto split = string::splitView(pStr, ";");
            float value = 0.0f;
            std::string text;

            if (!split.empty()) {
                value = utils::numFromString<float>(split.front()).unwrapOrDefault();

                if (split.size() > 1) {
                    text = split.back();
                }
                else {
                    text = split.front();
                }
            }

            m_rotationShortcutsMenu->addChild(
                ui::circleButton(
                    Label::create(text, Font::Default), CircleBaseColor::Green, this, menu_selector(EditMenu::onRotationShortcut)
                )
                    .scaleToFit(ROTATION_SHORTCUT_SIZE)
                    .tag(pTag)
                    .userObject("value"_spr, ObjWrapper<float>::create(value))
            );
        };

        createButton(BetterEditMenu::rotateShortcut1.get(), 1);
        createButton(BetterEditMenu::rotateShortcut2.get(), 2);
        createButton(BetterEditMenu::rotateShortcut3.get(), 3);

        m_rotationShortcutsMenu->updateLayout();
        m_rotationMenu->updateLayout();
    }

    void EditMenu::createButtons(CCArray* pButtonArray) {
        m_buttonsScroll->removeAllChildrenWithCleanup(true);

        for (auto node : CCArrayExt(pButtonArray)) {
            auto button = typeinfo_cast<CCMenuItemSpriteExtra*>(node);

            if (!button) {
                continue;
            }

            Setup(Button::createWithNode(MirrorNode::create(button)->captureSprite(), [sender = button, target = button->m_pListener, selector = button->m_pfnSelector] (auto) {
                std::invoke(selector, target, sender);
            }))
                .tag(button->getTag())
                .scale(BUTTONS_SCROLL_BUTTON_SCALE)
                .parent(m_buttonsScroll);
        }

        m_buttonsScroll->getContentLayer()->updateLayout();
        this->position();
        this->updateArrowButtons();
    }
    void EditMenu::updateButtonSprite(int pTag) {
        auto og = static_cast<CCNode*>(editor::ui()->m_editButtonDict->objectForKey(fmt::to_string(pTag)));

        if (!og) {
            return;
        }

        auto button = static_cast<Button*>(m_buttonsScroll->getContentLayer()->getChildByTag(pTag));
        
        if (!button) {
            return;
        }

        static_cast<CCSprite*>(button->getDisplayNode())->setTexture(MirrorNode::create(og)->captureTexture());
    }

    void EditMenu::position() {
        auto bg = editor::ui()->getChildByID("background-sprite");
        auto leftLine = editor::ui()->getChildByID("spacer-line-left");
        auto rightLine = editor::ui()->getChildByID("spacer-line-right");

        if (!bg || !leftLine || !rightLine) {
            return;
        }

        const auto scale = bg->getScaleY();

        static_cast<AxisLayout*>(m_moveMenu->getLayout())->setGap(BetterEditMenu::menuGap);
        Setup(m_moveMenu)
            .scale(scale)
            .updateLayout();

        static_cast<AxisLayout*>(m_rotationMenu->getLayout())->setGap(BetterEditMenu::menuGap);
        Setup(m_rotationMenu)
            .scale(scale)
            .updateLayout();

        Setup(this)
            .size(
                (ui::x(rightLine) - ui::sw(rightLine) / 2) - (ui::x(leftLine) + ui::sw(leftLine) / 2),
                ui::sh(bg)
            )
            .pos((ui::pos(leftLine) + ui::pos(rightLine)) / 2);

        Setup(m_buttonsScroll)
            .size((std::min(ui::w(m_buttonsScroll->getContentLayer()), (ui::w(this) * BUTTONS_SCROLL_MAX_WIDTH_PERCENT) / scale)), ui::h(m_buttonsScroll->getContentLayer()))
            .updateLayout();

        Setup(m_buttonsMenu)
            .scale(scale)
            .updateLayout();

        this->updateLayout();
        this->updateArrowButtons();
    }

    EditMenu* EditMenu::create() {
        auto ret = new EditMenu;

        if (!ret->init()) {
            delete ret;

            return nullptr;
        }

        ret->autorelease();

        return ret;
    }
}