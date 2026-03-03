#include <Geode/modify/CustomizeObjectLayer.hpp>
#include <internal/utils/utils.hpp>
#include "settings.hpp"

using namespace geode::prelude;
using namespace Utils::Aliases;

class $modify(TextObjectUtilsCustomizeObjectLayer, CustomizeObjectLayer) {
    struct Fields {
        std::vector<TextGameObject*> textObjects;

        TextInput* kerningInput = nullptr;
        CCMenu* textObjectUtilsMenu = nullptr;
    };

    static constexpr float VERTICAL_OFFSET = -20.0f;

    static constexpr float SIDE_BUTTON_DISTANCE = 120.0f;
    static constexpr float SIDE_BUTTON_SIZE = 25.0f;
    static constexpr float SIDE_BUTTON_GAP = 5.0f;

    void openTextMenu(float dt = {}) {
        if (auto button = m_textButton) {
            button->activate();
        }
    }

    bool init(GameObject* object, CCArray* objects) {
        if (!CustomizeObjectLayer::init(object, objects)) {
            return false;
        }
    
        if (!m_textInput) {
            return true;
        }

        if (Settings::TextObjectUtils::newlineShortcut.get().empty()) {
            Settings::TextObjectUtils::newlineShortcut.set("\\n");
        }

        m_fields->textObjects = std::move(Editor::Selection::getType<TextGameObject*>());

        Utils::setupNode(m_textInput, ChangeNodePositionY{VERTICAL_OFFSET});

        auto inputBG = Utils::setupNode(
            m_mainLayer->getChildByID("text-input-bg"),

            ChangeNodePositionY{VERTICAL_OFFSET},
            ChangeNodeWidth{-40.0f}
        );

        Utils::setupNode(m_kerningSlider, ChangeNodePositionY{VERTICAL_OFFSET});

        auto clearTextButton = static_cast<CCMenuItemSpriteExtra*>(getChildByIDRecursive("clear-text-button"));
        clearTextButton->setEnabled(false);
        clearTextButton->setOpacity(0);

        m_textInput->setMaxLabelLength(std::numeric_limits<int>::max());
        
        // ill find a better solution to this l8r
        if (Loader::get()->isModLoaded("hjfod.betteredit")) {
            this->scheduleOnce(schedule_selector(TextObjectUtilsCustomizeObjectLayer::openTextMenu), 0.0f);
        }
        else {
            openTextMenu({});
        }
 
        m_fields->textObjectUtilsMenu = Utils::setupNode(
            CCMenu::create(),

            SetNodeID{"text-object-utils-menu"_spr},
            SetNodePosition{CCPointZero},
            SetNodeParent{m_mainLayer}
        );
        m_textTabNodes->addObject(m_fields->textObjectUtilsMenu);

        Utils::setupNode(
            Utils::createButtonFrame("GJ_copyBtn_001.png", this, menu_selector(TextObjectUtilsCustomizeObjectLayer::onCopyText)),

            SetNodeID{"copy-text-button"_spr},
            SetNodePosition{inputBG->getPositionX() + (Settings::TextObjectUtils::swapCopyPaste.get() ? -SIDE_BUTTON_DISTANCE : SIDE_BUTTON_DISTANCE), inputBG->getPositionY()},
            SetNodeScaleWithSize{SIDE_BUTTON_SIZE},
            SetNodeParent{m_fields->textObjectUtilsMenu}
        );

        Utils::setupNode(
            Utils::createButtonFrame("GJ_pasteBtn_001.png", this, menu_selector(TextObjectUtilsCustomizeObjectLayer::onPasteText)),

            SetNodeID{"paste-text-button"_spr},
            SetNodePosition{inputBG->getPositionX() + (Settings::TextObjectUtils::swapCopyPaste.get() ? SIDE_BUTTON_DISTANCE : -SIDE_BUTTON_DISTANCE), inputBG->getPositionY()},
            SetNodeScale{SIDE_BUTTON_SIZE},
            SetNodeParent{m_fields->textObjectUtilsMenu}
        );

        Utils::setupNode(
            Utils::createButtonFrame("GJ_trashBtn_001.png", this, menu_selector(TextObjectUtilsCustomizeObjectLayer::onClearText)),

            SetNodeID{"clear-text-button"_spr},
            SetNodePosition{inputBG->getPositionX() + SIDE_BUTTON_DISTANCE + SIDE_BUTTON_SIZE + SIDE_BUTTON_GAP, inputBG->getPositionY()},
            SetNodeScale{SIDE_BUTTON_SIZE},
            SetNodeParent{m_fields->textObjectUtilsMenu}
        );

        Utils::setupNode(
            Utils::createButtonFrame("GJ_redoBtn_001.png", this, menu_selector(TextObjectUtilsCustomizeObjectLayer::onNewline)),

            SetNodeID{"newline-text-button"_spr},
            SetNodePosition{inputBG->getPositionX() - SIDE_BUTTON_DISTANCE - SIDE_BUTTON_SIZE - SIDE_BUTTON_GAP, inputBG->getPositionY()},
            SetNodeScale{SIDE_BUTTON_SIZE},
            SetNodeParent{m_fields->textObjectUtilsMenu}
        );

        m_fields->kerningInput = Utils::setupNode(
            Utils::createTextInput(45.0f, "0", [this]  (const std::string& pStr) {
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
            }),

            SetNodeID{"kerning-input"_spr},
            
            SetNodeParent{m_mainLayer}
        );
        m_fields->kerningInput->setCommonFilter(CommonFilter::Int);
        m_textTabNodes->addObject(m_fields->kerningInput);

        Utils::setupNode(m_kerningLabel, ChangeNodePosition{-m_fields->kerningInput->getScaledContentWidth(), VERTICAL_OFFSET});

        updateKerningLabel();
         
        return true;
    }

    void textChanged(CCTextInputNode* node) {
        if (node != m_textInput) {
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

        auto& input = m_fields->kerningInput;

        if (!input) {
            return;
        }

        m_kerningLabel->setString("Kerning: ");

        input->setPosition(
            m_kerningLabel->getPositionX() + m_kerningLabel->getScaledContentWidth() / 2 + input->getScaledContentWidth() / 2,
            m_kerningLabel->getPositionY()
        );
        input->setString(Utils::numToString(m_kerningAmount));
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
};