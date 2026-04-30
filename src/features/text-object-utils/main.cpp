#include <Geode/modify/CustomizeObjectLayer.hpp>
#include <internal/utils/utils.hpp>
#include "settings.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

class $modify(TextObjectUtilsCustomizeObjectLayer, CustomizeObjectLayer) {
    struct Fields {
        std::vector<TextGameObject*> textObjects;

        TextInput* kerningInput = nullptr;
        CCMenu* textObjectUtilsMenu = nullptr;

        bool textObjectMenuLoaded = false;
    };

    static constexpr float VERTICAL_OFFSET = -20.0f;

    static constexpr float SIDE_BUTTON_DISTANCE = 120.0f;
    static constexpr float SIDE_BUTTON_SIZE = 25.0f;
    static constexpr float SIDE_BUTTON_GAP = 5.0f;

    bool init(GameObject* object, CCArray* objects) {
        if (!CustomizeObjectLayer::init(object, objects)) {
            return false;
        }
    
        if (!m_textInput || !Settings::TextObjectUtils::enabled.get()) {
            return true;
        }

        if (Settings::TextObjectUtils::newlineShortcut.get().empty()) {
            Settings::TextObjectUtils::newlineShortcut.set("\\n");
        }

        m_fields->textObjects = std::move(editor::selection::getType<TextGameObject*>());

        m_textInput->setPositionY(m_textInput->getPositionY() + VERTICAL_OFFSET);

        auto inputBG = m_mainLayer->getChildByID("text-input-bg");
        inputBG->setPositionY(inputBG->getPositionY() + VERTICAL_OFFSET);
        inputBG->setContentSize({inputBG->getContentSize().width - 40.0f, inputBG->getContentSize().height});

        m_kerningSlider->setPositionY(m_kerningSlider->getPositionY() + VERTICAL_OFFSET);

        auto clearTextButton = static_cast<CCMenuItemSpriteExtra*>(getChildByIDRecursive("clear-text-button"));
        clearTextButton->setEnabled(false);
        clearTextButton->setOpacity(0);

        m_textInput->setMaxLabelLength(std::numeric_limits<int>::max());
        
        // ill find a better solution to this l8r
        if (nwo5::utils::isBetterEditLoaded()) {
            Loader::get()->queueInMainThread([this] {
                this->openTextMenu();
            });
        }
        else {
            openTextMenu();
        }
 
        auto copyTextButton = ui::node(
            Setup(ui::buttonFrame("GJ_copyBtn_001.png", this, menu_selector(TextObjectUtilsCustomizeObjectLayer::onCopyText)))
                .id("copy-text-button"_spr)
                .pos(inputBG->getPositionX() + (Settings::TextObjectUtils::swapCopyPaste.get() ? -SIDE_BUTTON_DISTANCE : SIDE_BUTTON_DISTANCE), inputBG->getPositionY())
                .scaleToFit(SIDE_BUTTON_SIZE)
        );
        auto pasteTextButton = ui::node(
            Setup(ui::buttonFrame("GJ_pasteBtn_001.png", this, menu_selector(TextObjectUtilsCustomizeObjectLayer::onPasteText)))
                .id("paste-text-button"_spr)
                .pos(inputBG->getPositionX() + (Settings::TextObjectUtils::swapCopyPaste.get() ? SIDE_BUTTON_DISTANCE : -SIDE_BUTTON_DISTANCE), inputBG->getPositionY())
                .scaleToFit(SIDE_BUTTON_SIZE)
        );
        auto customClearTextButton = ui::node(
            Setup(ui::buttonFrame("GJ_trashBtn_001.png", this, menu_selector(TextObjectUtilsCustomizeObjectLayer::onClearText)))
                .id("clear-text-button"_spr)
                .pos(inputBG->getPositionX() + SIDE_BUTTON_DISTANCE + SIDE_BUTTON_SIZE + SIDE_BUTTON_GAP, inputBG->getPositionY())
                .scaleToFit(SIDE_BUTTON_SIZE)
        );
        auto newlineTextButton = ui::node(
            Setup(ui::buttonFrame("GJ_redoBtn_001.png", this, menu_selector(TextObjectUtilsCustomizeObjectLayer::onNewline)))
                .id("newline-text-button"_spr)
                .pos(inputBG->getPositionX() - SIDE_BUTTON_DISTANCE - SIDE_BUTTON_SIZE - SIDE_BUTTON_GAP, inputBG->getPositionY())
                .scaleToFit(SIDE_BUTTON_SIZE)
        );

        m_fields->textObjectUtilsMenu = ui::node(
            Setup(ui::menu())
                .id("text-object-utils-menu"_spr)
                .pos(CCPointZero)
                .children(
                    copyTextButton,
                    pasteTextButton,
                    customClearTextButton,
                    newlineTextButton
                )
                .parent(m_mainLayer)
        );
        m_textTabNodes->addObject(m_fields->textObjectUtilsMenu);

        m_fields->kerningInput = ui::node(
            Setup(ui::input(45.0f, "0"))
                .id("kerning-input"_spr)
                .filter(CommonFilter::Int)
                .callback([this] (const std::string& pStr) {
                    if (pStr.empty()) {
                        return;
                    }

                    const auto kerning = numFromString<int>(pStr).unwrapOrDefault();
                    m_kerningAmount = kerning;
                    m_kerningSlider->setValue(std::clamp(kerning + 10.0f, 0.0f, 30.0f) / 30);

                    for (auto obj : m_fields->textObjects) {
                        obj->updateTextKerning(kerning);
                    }

                    updateKerningLabel();
                })
                .parent(m_mainLayer)
        );
        m_textTabNodes->addObject(m_fields->kerningInput);

        m_kerningLabel->setPosition(
            m_kerningLabel->getPositionX() - m_fields->kerningInput->getScaledContentWidth(),
            m_kerningLabel->getPositionY() + VERTICAL_OFFSET
        );

        updateKerningLabel();

        m_fields->textObjectMenuLoaded = true;
         
        return true;
    }

    void textChanged(CCTextInputNode* node) {
        if (node != m_textInput || !m_fields->textObjectMenuLoaded) {
            return;
        }

        const std::string str{node->getString()};

        if (str.contains(Settings::TextObjectUtils::newlineShortcut.get())) {
            node->setString(string::replace(str, Settings::TextObjectUtils::newlineShortcut.get(), "\n"));
        }
        else {
            CustomizeObjectLayer::textChanged(node);
        }
    }

    void updateKerningLabel() {
        CustomizeObjectLayer::updateKerningLabel();

        auto input = m_fields->kerningInput;

        if (!input || !m_fields->textObjectMenuLoaded) {
            return;
        }

        m_kerningLabel->setString("Kerning: ");

        input->setPosition(
            m_kerningLabel->getPositionX() + m_kerningLabel->getScaledContentWidth() / 2 + input->getScaledContentWidth() / 2,
            m_kerningLabel->getPositionY()
        );
        input->setString(nwo5::utils::numToString(m_kerningAmount));
    }

    void onClose(CCObject* sender) {
        // dont crash uwu (idk if it still crashes in v5 but js incase)
        if (m_fields->kerningInput) {
            m_fields->kerningInput->getInputNode()->onClickTrackNode(false);
        }
        
        CustomizeObjectLayer::onClose(sender);
    }

    void onCopyText(CCObject* pSender) {
        clipboard::write(m_textInput->getString());
    }
    void onPasteText(CCObject* pSender) {
        m_textInput->setString(clipboard::read());
    }
    void onClearText(CCObject* pSender) {
        m_textInput->setString("");
    }
    void onNewline(CCObject* pSender) {
        m_textInput->setString(fmt::format("{}\n", m_textInput->getString()));
    }

    void openTextMenu() {
        if (auto button = m_textButton) {
            button->activate();
        }
    }
};