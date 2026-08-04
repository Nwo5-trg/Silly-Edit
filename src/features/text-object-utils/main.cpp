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
    };

    static constexpr float VERTICAL_OFFSET = -20.0f;

    static constexpr float SIDE_BUTTON_DISTANCE = 120.0f;
    static constexpr float SIDE_BUTTON_SIZE = 25.0f;
    static constexpr float SIDE_BUTTON_GAP = 5.0f;

    bool init(GameObject* object, CCArray* objects) {
        auto fields = m_fields.self();

        for (auto obj : CCArrayExt<GameObject*>(objects)) {
            if (obj->m_objectID == 914) {
                fields->textObjects.push_back(static_cast<TextGameObject*>(obj));
            }
        }

        if (fields->textObjects.size() == objects->count() && !object) {
            if (!CustomizeObjectLayer::init(static_cast<GameObject*>(objects->firstObject()), CCArray::create())) {
                return false;
            }
        }
        else {
            if (!CustomizeObjectLayer::init(object, objects)) {
                return false;
            }
        }

        if (!Settings::TextObjectUtils::enabled.get() || fields->textObjects.size() != objects->count() || !m_textInput) {
            fields->textObjects.clear();
            
            return true;
        }

        if (!object) {
            m_targetObjects = objects;
            m_targetObject = nullptr;

            const auto str = fields->textObjects.front()->m_text;

            for (auto obj : fields->textObjects) {
                if (obj->m_text != str) {
                    m_textInput->setString("");
                    
                    break;
                }
            }
        }

        if (Settings::TextObjectUtils::newlineShortcut.get().empty()) {
            Settings::TextObjectUtils::newlineShortcut.set("\\n");
        }

        m_textInput->setPositionY(m_textInput->getPositionY() + VERTICAL_OFFSET);

        auto inputBG = m_mainLayer->getChildByID("text-input-bg");
        inputBG->setPositionY(inputBG->getPositionY() + VERTICAL_OFFSET);
        inputBG->setContentSize({inputBG->getContentSize().width - 40.0f, inputBG->getContentSize().height});

        m_kerningSlider->setPositionY(m_kerningSlider->getPositionY() + VERTICAL_OFFSET);

        auto clearTextButton = static_cast<CCMenuItemSpriteExtra*>(this->getChildByIDRecursive("clear-text-button"));
        clearTextButton->setEnabled(false);
        clearTextButton->setOpacity(0);

        m_textInput->setMaxLabelLength(std::numeric_limits<int>::max());
        
        // ill find a better solution to this l8r
        if (nwo5::utils::isTinkerLoaded() || nwo5::utils::isBetterEditLoaded()) {
            Loader::get()->queueInMainThread([this] {
                this->openTextMenu();
            });
        }
        else {
            this->openTextMenu();
        }

        fields->textObjectUtilsMenu = ui::node(Setup(ui::menu(true))
            .id("text-object-utils-menu"_spr)
            .pos(CCPointZero)
            .children(
                    Setup(ui::buttonFrame(
                        "GJ_copyBtn_001.png", this, menu_selector(TextObjectUtilsCustomizeObjectLayer::onCopyText)
                    ))
                        .id("copy-text-button"_spr)
                        .pos(inputBG->getPositionX() + (Settings::TextObjectUtils::swapCopyPaste.get() ? -SIDE_BUTTON_DISTANCE : SIDE_BUTTON_DISTANCE), inputBG->getPositionY())
                        .scaleToFit(SIDE_BUTTON_SIZE),
                    Setup(ui::buttonFrame(
                        "GJ_pasteBtn_001.png", this, menu_selector(TextObjectUtilsCustomizeObjectLayer::onPasteText)
                    ))
                        .id("paste-text-button"_spr)
                        .pos(inputBG->getPositionX() + (Settings::TextObjectUtils::swapCopyPaste.get() ? SIDE_BUTTON_DISTANCE : -SIDE_BUTTON_DISTANCE), inputBG->getPositionY())
                        .scaleToFit(SIDE_BUTTON_SIZE),
                    Setup(ui::buttonFrame(
                        "GJ_trashBtn_001.png", this, menu_selector(TextObjectUtilsCustomizeObjectLayer::onClearText)
                    ))
                        .id("clear-text-button"_spr)
                        .pos(inputBG->getPositionX() + SIDE_BUTTON_DISTANCE + SIDE_BUTTON_SIZE + SIDE_BUTTON_GAP, inputBG->getPositionY())
                        .scaleToFit(SIDE_BUTTON_SIZE),
                    Setup(ui::buttonFrame(
                        "GJ_redoBtn_001.png", this, menu_selector(TextObjectUtilsCustomizeObjectLayer::onNewline)
                    ))
                        .id("newline-text-button"_spr)
                        .pos(inputBG->getPositionX() - SIDE_BUTTON_DISTANCE - SIDE_BUTTON_SIZE - SIDE_BUTTON_GAP, inputBG->getPositionY())
                        .scaleToFit(SIDE_BUTTON_SIZE)
                )
            .parent(m_mainLayer)
            .addTo(m_textTabNodes)
        );

        fields->kerningInput = ui::node(Setup(ui::input(45.0f, "0"))
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
                        obj->updateTextKerning(m_kerningAmount);
                        obj->updateTextObject(obj->m_text, false);
                    }

                    this->updateKerningLabel();
                })
            .parent(m_mainLayer)
            .addTo(m_textTabNodes)
        );

        m_kerningLabel->setPosition(
            m_kerningLabel->getPositionX() - fields->kerningInput->getScaledContentWidth() / 2,
            m_kerningLabel->getPositionY() + VERTICAL_OFFSET
        );
         
        this->updateKerningLabel();
        
        return true;
    }

    void textChanged(CCTextInputNode* node) {
        auto fields = m_fields.self();

        if (node != m_textInput || fields->textObjects.empty()) {
            return CustomizeObjectLayer::textChanged(node);
        }

        const std::string str{node->getString()};

        if (str.contains(Settings::TextObjectUtils::newlineShortcut.get())) {
            node->setString(string::replace(str, Settings::TextObjectUtils::newlineShortcut.get(), "\n"));

            return;
        }

        for (auto obj : fields->textObjects) {
            obj->updateTextObject(str, false);
        }
    }

    void sliderChanged(CCObject* sender) {
        auto fields = m_fields.self();
        
        if (fields->textObjects.empty() || !fields->kerningInput) {
            return CustomizeObjectLayer::sliderChanged(sender);
        }

        m_kerningAmount = static_cast<SliderThumb*>(sender)->getValue() * 30 - 10;

        for (auto obj : fields->textObjects) {
            obj->updateTextKerning(m_kerningAmount);
            obj->updateTextObject(obj->m_text, false);
        }

        m_kerningLabel->setString("Kerning: ");

        fields->kerningInput->setPosition(
            m_kerningLabel->getPositionX() + m_kerningLabel->getScaledContentWidth() / 2 + fields->kerningInput->getScaledContentWidth() / 2,
            m_kerningLabel->getPositionY()
        );
        fields->kerningInput->setString(nwo5::utils::numToString(m_kerningAmount));
    }
    
    void onClose(CCObject* sender) {
        auto fields = m_fields.self();

        if (!fields->textObjects.empty()) {
            m_targetObject = static_cast<GameObject*>(m_targetObjects->objectAtIndex(0));
            m_targetObjects = nullptr;
        }
        
        // dont crash uwu (idk if it still crashes in v5 but js incase)
        if (auto input = fields->kerningInput) {
            input->getInputNode()->onClickTrackNode(false);
        }
        
        CustomizeObjectLayer::onClose(sender);
    }

    void onCopyText(CCObject*) {
        clipboard::write(m_textInput->getString());
    }
    void onPasteText(CCObject*) {
        m_textInput->setString(clipboard::read());
    }
    void onClearText(CCObject*) {
        m_textInput->setString("");
    }
    void onNewline(CCObject*) {
        m_textInput->setString(fmt::format("{}\n", m_textInput->getString()));
    }

    void openTextMenu() {
        if (auto button = m_textButton) {
            button->activate();
        }
    }
};