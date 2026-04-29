#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/GameObject.hpp>
#include <internal/utils/utils.hpp>
#include "settings.hpp"
#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

class $modify(BetterLayersEditorUI, EditorUI) {
    struct Fields {
        CCMenu* newLayerMenu = nullptr;
        TextInput* layerInput = nullptr;
        CCMenuItemSpriteExtra* allLayersButton = nullptr;
        CCMenuItemToggler* lockLayerButton = nullptr;
        
        // so my constructor/destructor stuff can work as expected
        std::unique_ptr<BetterLayers::LayerSettings> settings = nullptr;

        // remind me to make some kind of system of select filters in api cuz this is copy pasted from replace obj
        bool canDestroyUndo = false;
        bool updateLockButton = true;
    };

    static constexpr float GAP = 5.0f;
    static constexpr CCSize LAYER_INPUT_SIZE = {35.0f, 20.0f};
    static constexpr float LAYER_SHIFT_BUTTON_SIZE = 25.0f;
    static constexpr float LAYER_EXTRA_BUTTON_SIZE = 20.0f;

    static void onModify(auto& pSelf) {
        (void)pSelf.setHookPriority("EditorUI::init", Priority::LatePost);
    }

    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) {
            return false;
        }

        if (!Settings::BetterLayers::enabled.get()) {
            return true;
        }
        
        auto fields = m_fields.self();

        fields->settings = std::make_unique<BetterLayers::LayerSettings>();

        fields->layerInput = ui::node(
            Setup(ui::input(LAYER_INPUT_SIZE, "All"))
                .id("layer-input"_spr)
                .filter("aA1234567890")
                .maxCharCount(4)
                .callback([this] (const std::string& pStr) {
                    if (pStr == "a" || pStr == "A") {
                        editor::setLayer(editor::constants::ALL_LAYERS);
                    }
                    else if (pStr.contains('A') || pStr.contains('l')) {
                        m_fields->layerInput->setString(string::remove(pStr, "Al"));
                    }
                    else if (!pStr.empty()) {
                        auto res = utils::numFromString<int>(pStr);

                        if (auto res = utils::numFromString<int>(pStr); res.isOk()) {
                            editor::setLayer(std::clamp(res.unwrap(), 0, editor::constants::MAX_LAYERS));
                        }
                    }
                })
        );

        auto nextLayer = ui::node(
            Setup(ui::buttonFrame("GJ_arrow_03_001.png", this, menu_selector(EditorUI::onGroupUp)))
                .id("next-layer-button"_spr)
                .scaleToFit(LAYER_SHIFT_BUTTON_SIZE)
                .flipX()
        );
        auto prevLayer = ui::node(
            Setup(ui::buttonFrame("GJ_arrow_03_001.png", this, menu_selector(EditorUI::onGroupDown)))
                .id("prev-layer-button"_spr)
                .scaleToFit(LAYER_SHIFT_BUTTON_SIZE)
        );

        fields->allLayersButton = ui::node(
            // girl robtop, ur function names, what the fuck is this, why only name it layer here </3
            Setup(ui::buttonFrame("GJ_arrow_02_001.png", this, menu_selector(EditorUI::onGoToBaseLayer)))
                .id("next-free-layer-button"_spr)
                .scaleToFit(LAYER_EXTRA_BUTTON_SIZE)
        );

        auto nextFreeLayer = ui::node(
            Setup(ui::buttonFrame("GJ_plusBtn_001.png", this, menu_selector(BetterLayersEditorUI::onNextFreeLayer)))
                .id("next-free-layer-button"_spr)
                .scaleToFit(LAYER_EXTRA_BUTTON_SIZE)
                .visible(Settings::BetterLayers::nextFreeButton.get())
        );

        fields->lockLayerButton = ui::node(
            Setup(ui::togglerFrame("warpLockOffBtn_001.png", "warpLockOnBtn_001.png", this, menu_selector(BetterLayersEditorUI::onToggleLayerLocked)))
                .id("lock-layer-button"_spr)
                .scaleToFit(LAYER_EXTRA_BUTTON_SIZE)
                .visible(Settings::BetterLayers::lockButton.get())
        );

        auto layerSettings = ui::node(
            Setup(ui::buttonFrame("GJ_optionsBtn_001.png", this, menu_selector(BetterLayersEditorUI::onLayerSettings)))
                .id("layer-settings-button"_spr)
                .scaleToFit((LAYER_SHIFT_BUTTON_SIZE + LAYER_EXTRA_BUTTON_SIZE) / 2)
        );

        auto oldLayerMenu = m_currentLayerLabel->getParent();
        oldLayerMenu->setVisible(false);

        if (m_currentLayerLabel) {
            m_currentLayerLabel->setPositionX(999.0f); // i give up dealing with better edits editablelabelproxy fuck that
        }

        fields->newLayerMenu = ui::node(
            Setup(ui::menu(ui::horizontalDistrbLayout(GAP)))
                .id("new-layer-menu"_spr)
                .scale(oldLayerMenu)
                .anchor(oldLayerMenu)
                .pos(oldLayerMenu)
                .order(oldLayerMenu)
                .children(
                    layerSettings,
                    fields->lockLayerButton,
                    fields->allLayersButton,
                    prevLayer,
                    fields->layerInput,
                    nextLayer,
                    nextFreeLayer
                )
                .parent(this)
        );

        m_uiItems->addObject(fields->newLayerMenu);

        m_editorLayer->m_currentLayer = m_editorLayer->m_level->m_lastBuildGroupID;
        updateLayerMenu();

        return true;
    }

    void updateLayerMenu() {
        auto fields = m_fields.self();

        if (!fields->newLayerMenu) {
            return;
        }

        // editor technically not initialized yet when im calling in init so i cant call thru my api (mayb ill fix this eventually)
        const auto layer = m_editorLayer->m_currentLayer;

        if (layer == editor::constants::ALL_LAYERS) {
            fields->layerInput->setString("All");
        }
        else {
            fields->layerInput->setString(nwo5::utils::numToString(layer));
        }
        
        if (editor::layerLocked(layer)) {
            fields->layerInput->getInputNode()->getTextLabel()->setColor(ccYELLOW);
        }
        else {
            fields->layerInput->getInputNode()->getTextLabel()->setColor(ccWHITE);
        }

        fields->lockLayerButton->setVisible(
            Settings::BetterLayers::lockButton.get() 
            && GameManager::get()->getGameVariable(GameVar::LayerLocking) 
            && layer != editor::constants::ALL_LAYERS
        );
        if (fields->updateLockButton) {
            fields->lockLayerButton->toggle(editor::layerLocked(layer));
        }
        
        fields->allLayersButton->setVisible(layer != editor::constants::ALL_LAYERS);

        fields->newLayerMenu->updateLayout();
    }

    // hjfod highway robbery
    void onNextFreeLayer(CCObject*) {
        std::set<short> usedLayers;
        
        for (auto obj : CCArrayExt<GameObject*>(m_editorLayer->m_objects)) {
            usedLayers.insert(obj->m_editorLayer);
            usedLayers.insert(obj->m_editorLayer2);
        }

        short nextFree;
        for (nextFree = 0; nextFree < std::numeric_limits<short>::max(); nextFree++) {
            if (!usedLayers.contains(nextFree)) {
                break;
            }
        }
        
        editor::setLayer(nextFree);
    }

    void onLayerSettings(CCObject*) {
        if (editor::currentLayer() == editor::constants::ALL_LAYERS) {
            BetterLayers::EditAllLayersPopup::create(m_fields->settings.get())->show();
        }
        else {
            BetterLayers::EditLayerPopup::create(m_fields->settings.get())->show();
        }
    }

    void onToggleLayerLocked(CCObject*) {
        m_fields->updateLockButton = false;

        editor::lockLayer(editor::currentLayer(), !editor::layerLocked(editor::currentLayer()));
        
        m_fields->updateLockButton = true;
    }

    void updateGroupIDLabel() {
        EditorUI::updateGroupIDLabel();

        updateLayerMenu();
    }

    // so when clicking on an unselectable object the undo queue doesnt get filled up
    void createUndoSelectObject(bool redo) {
        EditorUI::createUndoSelectObject(redo);

        m_fields->canDestroyUndo = true;
    }

    void selectObject(GameObject* object, bool ignoreFilter) {
        if (!Settings::BetterLayers::enabled.get()) {
            return EditorUI::selectObject(object, ignoreFilter);
        }
    
        if (ignoreFilter == true) {
            return EditorUI::selectObject(object, ignoreFilter);
        }

        bool canSelect = true;

        if (Settings::BetterLayers::unselectableUnfocusedLayers.get()) {
            if (auto res = m_fields->settings->getFocusedLayer(); res.has_value() && object->m_editorLayer != res.value() && object->m_editorLayer2 != res.value()) {
                canSelect = false;
            }
        }
        if (Settings::BetterLayers::unselectableHiddenLayers.get()) {
            canSelect = !m_fields->settings->isLayerHidden(object->m_editorLayer) && !m_fields->settings->isLayerHidden(object->m_editorLayer2);
        }

        if (canSelect) {
            EditorUI::selectObject(object, ignoreFilter);
        }
        else if (m_fields->canDestroyUndo) {
            m_editorLayer->m_undoObjects->removeLastObject(false);
        }
    }

    void selectObjects(CCArray* objects, bool ignoreFilter) {
        if (!Settings::BetterLayers::enabled.get()) {
            return EditorUI::selectObjects(objects, ignoreFilter);
        }

        if (ignoreFilter) {
            return EditorUI::selectObjects(objects, ignoreFilter);
        }

        auto validObjs = CCArray::create();

        for (auto obj : CCArrayExt<GameObject*>(objects)) {
            if (Settings::BetterLayers::unselectableUnfocusedLayers.get()) {
                if (auto res = m_fields->settings->getFocusedLayer(); res.has_value() && obj->m_editorLayer != res.value() && obj->m_editorLayer2 != res.value()) {
                    continue;
                }
            }
            if (Settings::BetterLayers::unselectableHiddenLayers.get()) {
                if (m_fields->settings->isLayerHidden(obj->m_editorLayer) || m_fields->settings->isLayerHidden(obj->m_editorLayer2)) {
                    continue;
                }
            }

            validObjs->addObject(obj);
        }

        if (validObjs->count()) {
            EditorUI::selectObjects(validObjs, ignoreFilter);
        }
        else if (m_fields->canDestroyUndo) {
            m_editorLayer->m_undoObjects->removeLastObject(false);
        }
    }
};

// idk girl im at my wits end with this fucking god forsaken mod im js typing shit atp let me be done and release this shit omfg
static bool& shouldDoSillyLayerShenanigans() {
    static bool val = false;
    return val;
}

class $modify(LevelEditorLayer) {
    void updateVisibility(float dt) {
        shouldDoSillyLayerShenanigans() = !editor::isPlaytesting();

        LevelEditorLayer::updateVisibility(dt);

        shouldDoSillyLayerShenanigans() = false;
    }
};

class $modify(GameObject) {
    void setOpacity(unsigned char opacity) {
        GameObject::setOpacity(
            shouldDoSillyLayerShenanigans() 
                ? editor::ui<BetterLayersEditorUI*>()->m_fields.self()->settings->opacityForObject(opacity, this) 
                : opacity
        );
    }
};