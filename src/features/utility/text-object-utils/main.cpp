#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

namespace TextObjectUtils {
    void CustomizeObjectLayer::onCopyText(CCObject*) {
        clipboard::write(m_textInput->getString());
    }
    void CustomizeObjectLayer::onPasteText(CCObject*) {
        m_textInput->setString(clipboard::read());
    }
    void CustomizeObjectLayer::onClearText(CCObject*) {
        m_textInput->setString("");
    }
    void CustomizeObjectLayer::onNewline(CCObject*) {
        m_textInput->setString(fmt::format("{}\n", m_textInput->getString()));
    }

    void CustomizeObjectLayer::openTextMenu() {
        if (auto button = m_textButton) {
            button->activate();
        }
    }


    
    bool CustomizeObjectLayer::init(GameObject* object, CCArray* objects) {
        if (!GD::CustomizeObjectLayer::init(object, objects)) {
            return false;
        }
    
        if (!m_textInput || !TextObjectUtils::enabled()) {
            return true;
        }

        if (TextObjectUtils::newlineShortcut.get().empty()) {
            TextObjectUtils::newlineShortcut.set("\\n");
        }

        auto fields = m_fields.self();

        for (auto obj : objects->asExt()) {
            if (auto textObj = typeinfo_cast<TextGameObject*>(obj)) {
                fields->textObjects.push_back(textObj); 
            }
        }

        m_textInput->setPositionY(ui::y(m_textInput) + VERTICAL_OFFSET);

        auto inputBG = m_mainLayer->getChildByID("text-input-bg");

        Setup(inputBG)
            .moveY(VERTICAL_OFFSET)
            .size(ui::w(inputBG) - 40.0f, ui::h(inputBG));

        m_kerningSlider->setPositionY(ui::y(m_kerningSlider) + VERTICAL_OFFSET);

        if (auto clearTextButton = static_cast<CCMenuItemSpriteExtra*>(this->getChildByIDRecursive("clear-text-button"))) {
            clearTextButton->setEnabled(false);
            clearTextButton->setOpacity(0);
        }

        m_textInput->setMaxLabelLength(std::numeric_limits<int>::max());
        
        // ill find a better solution to this l8r
        if (sillyedit::utils::isTinkerLoaded() || sillyedit::utils::isBetterEditLoaded()) {
            Loader::get()->queueInMainThread([this] {
                this->openTextMenu();
            });
        }
        else {
            openTextMenu();
        }
 
        fields->textObjectUtilsMenu = ui::menu(true)
            .id("text-object-utils-menu"_spr)
            .pos(CCPointZero)
            .children(
                ui::buttonFrame(
                    ui::frame::COPY_BUTTON, this, menu_selector(TextObjectUtils::CustomizeObjectLayer::onCopyText)
                )
                    .id("copy-text-button"_spr)
                    .pos(ui::x(inputBG) + (TextObjectUtils::swapCopyPaste.get() ? -SIDE_BUTTON_DISTANCE : SIDE_BUTTON_DISTANCE), ui::y(inputBG))
                    .scaleToFit(SIDE_BUTTON_SIZE),
                ui::buttonFrame(
                    ui::frame::PASTE_BUTTON, this, menu_selector(TextObjectUtils::CustomizeObjectLayer::onPasteText)
                )
                    .id("paste-text-button"_spr)
                    .pos(ui::x(inputBG) + (TextObjectUtils::swapCopyPaste.get() ? SIDE_BUTTON_DISTANCE : -SIDE_BUTTON_DISTANCE), ui::y(inputBG))
                    .scaleToFit(SIDE_BUTTON_SIZE),
                ui::buttonFrame(
                    ui::frame::TRASH_BUTTON, this, menu_selector(TextObjectUtils::CustomizeObjectLayer::onClearText)
                )
                    .id("clear-text-button"_spr)
                    .pos(ui::x(inputBG) + SIDE_BUTTON_DISTANCE + SIDE_BUTTON_SIZE + SIDE_BUTTON_GAP, ui::y(inputBG))
                    .scaleToFit(SIDE_BUTTON_SIZE),
                ui::buttonFrame(
                    ui::frame::REDO_BUTTON, this, menu_selector(TextObjectUtils::CustomizeObjectLayer::onNewline)
                )
                    .id("newline-text-button"_spr)
                    .pos(ui::x(inputBG) - SIDE_BUTTON_DISTANCE - SIDE_BUTTON_SIZE - SIDE_BUTTON_GAP, ui::y(inputBG))
                    .scaleToFit(SIDE_BUTTON_SIZE)
            )
            .parent(m_mainLayer)
            .addTo(m_textTabNodes);

        fields->kerningInput = ui::input(45.0f, "0")
            .id("kerning-input"_spr)
            .filter(CommonFilter::Int)
            .callback([this] (const std::string& pStr) {
                if (pStr.empty()) {
                    return;
                }

                const auto kerning = utils::numFromString<int>(pStr).unwrapOrDefault();
                this->m_kerningAmount = kerning;
                this->m_kerningSlider->setValue(std::clamp(kerning + 10.0f, 0.0f, 30.0f) / 30);

                for (auto obj : m_fields->textObjects) {
                    obj->updateTextKerning(kerning);
                }

                this->updateKerningLabel();
            })
            .parent(m_mainLayer)
            .addTo(m_textTabNodes);

        m_kerningLabel->setPosition(
            ui::x(m_kerningLabel) - ui::sw(m_fields->kerningInput) / 2,
            ui::y(m_kerningLabel) + VERTICAL_OFFSET
        );

        m_fields->textObjectMenuLoaded = true;
         
        this->updateKerningLabel();
        
        return true;
    }

    void CustomizeObjectLayer::textChanged(CCTextInputNode* node) {
        if (node != m_textInput || !m_fields->textObjectMenuLoaded) {
            return;
        }

        const std::string str{node->getString()};

        if (str.contains(TextObjectUtils::newlineShortcut.get())) {
            node->setString(string::replace(str, TextObjectUtils::newlineShortcut.get(), "\n"));
        }
        else {
            GD::CustomizeObjectLayer::textChanged(node);
        }
    }

    void CustomizeObjectLayer::updateKerningLabel() {
        GD::CustomizeObjectLayer::updateKerningLabel();
        
        auto fields = m_fields.self();
        auto input = fields->kerningInput;

        if (!input || !fields->textObjectMenuLoaded) {
            return;
        }

        m_kerningLabel->setString("Kerning: ");

        Setup(input)
            .pos(
                ui::x(m_kerningLabel) + ui::sw(m_kerningLabel) / 2 + ui::sw(input) / 2,
                ui::y(m_kerningLabel)
            )
            .string(misc::numToString(m_kerningAmount));
    }
    
    void CustomizeObjectLayer::onClose(CCObject* sender) {
        auto fields = m_fields.self();

        if (fields->kerningInput) {
            fields->kerningInput->getInputNode()->onClickTrackNode(false);
        }
        
        GD::CustomizeObjectLayer::onClose(sender);
    }
}