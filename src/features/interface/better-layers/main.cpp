#include <utils/include.hpp>
#include "shared.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

namespace BetterLayers {
    EditorUI::Fields::~Fields() {
        sillyedit::shared::getLayerSettingsPtr() = nullptr;
    }

    void EditorUI::updateLayerMenu() {
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
            fields->layerInput->setString(misc::numToString(layer));
        }
        
        if (editor::layerLocked(layer)) {
            fields->layerInput->getInputNode()->getTextLabel()->setColor({255, 200, 0});
        }
        else {
            fields->layerInput->getInputNode()->getTextLabel()->setColor(Col::White);
        }

        fields->lockLayerButton->setVisible(
            BetterLayers::lockButton 
            && GameManager::get()->getGameVariable(GameVar::LayerLocking) 
            && layer != editor::constants::ALL_LAYERS
        );
        if (fields->updateLockButton) {
            fields->lockLayerButton->toggle(editor::layerLocked(layer));
        }
        
        fields->allLayersButton->setVisible(layer != editor::constants::ALL_LAYERS);

        fields->newLayerMenu->updateLayout();

        if (auto oldLayerMenu = m_currentLayerLabel->getParent()) {
            Setup(fields->newLayerMenu)
                .scale(oldLayerMenu)
                .anchor(Anchor::Right)
                .pos(ui::x(oldLayerMenu) + ui::sw(oldLayerMenu) / 2, ui::y(oldLayerMenu))
                .order(oldLayerMenu);
        }
    }


    
    // hjfod highway robbery
    void EditorUI::onNextFreeLayer(CCObject*) {
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

    void EditorUI::onLayerSettings(CCObject*) {
        if (editor::currentLayer() == editor::constants::ALL_LAYERS) {
            EditAllLayersPopup::create(m_fields->settings.get())->show();
        }
        else {
            EditLayerPopup::create(m_fields->settings.get())->show();
        }
    }

    void EditorUI::onToggleLayerLocked(CCObject*) {
        m_fields->updateLockButton = false;

        editor::lockLayer(editor::currentLayer(), !editor::layerLocked(editor::currentLayer()));
        
        m_fields->updateLockButton = true;
    }

    void EditorUI::updateGroupIDLabel() {
        GD::EditorUI::updateGroupIDLabel();

        this->updateLayerMenu();
    }

    // so when clicking on an unselectable object the undo queue doesnt get filled up
    void EditorUI::createUndoSelectObject(bool redo) {
        GD::EditorUI::createUndoSelectObject(redo);

        m_fields->canDestroyUndo = true;
    }

    bool EditorUI::canSelectObject(GameObject* object) {
        if (!BetterLayers::enabled()) {
            return GD::EditorUI::canSelectObject(object);
        }

        if (!object || !GD::EditorUI::canSelectObject(object)) {
            return false;
        }

        auto fields = m_fields.self();

        if (!fields->settings) {
            return true;
        }

        bool canSelect = true;

        if (BetterLayers::unselectableUnfocusedLayers || BetterLayers::unselectableHiddenLayers) {
            if (BetterLayers::unselectableUnfocusedLayers) {
                if (auto res = fields->settings->getFocusedLayer(); res.has_value() && object->m_editorLayer != res.value() && object->m_editorLayer2 != res.value()) {
                    canSelect = false;
                }
            }
            if (BetterLayers::unselectableHiddenLayers) {
                canSelect = !fields->settings->isLayerHidden(object->m_editorLayer) && !fields->settings->isLayerHidden(object->m_editorLayer2);
            }
        }

        return canSelect;
    }

    void EditorUI::selectObjects(CCArray* objects, bool ignoreFilter) {
        if (!BetterLayers::enabled()) {
            return GD::EditorUI::selectObjects(objects, ignoreFilter);
        }

        if (ignoreFilter) {
            return GD::EditorUI::selectObjects(objects, ignoreFilter);
        }
        
        auto fields = m_fields.self();

        if (!fields->settings) {
            return GD::EditorUI::selectObjects(objects, ignoreFilter);
        }

        auto validObjs = CCArray::create();

        for (auto obj : CCArrayExt<GameObject*>(objects)) {
            if (!obj) {
                continue;
            }
            
            if (BetterLayers::unselectableUnfocusedLayers) {
                if (auto res = fields->settings->getFocusedLayer(); res.has_value() && obj->m_editorLayer != res.value() && obj->m_editorLayer2 != res.value()) {
                    continue;
                }
            }
            if (BetterLayers::unselectableHiddenLayers) {
                if (fields->settings->isLayerHidden(obj->m_editorLayer) || fields->settings->isLayerHidden(obj->m_editorLayer2)) {
                    continue;
                }
            }

            validObjs->addObject(obj);
        }

        if (validObjs->count()) {
            GD::EditorUI::selectObjects(validObjs, ignoreFilter);
        }
        else if (m_fields->canDestroyUndo) {
            m_editorLayer->m_undoObjects->removeLastObject(false);

            m_fields->canDestroyUndo = false;
        }
    }





    void Feature::onEditor() {
        if (!BetterLayers::enabled()) {
            return;
        }
        
        auto self = editor::ui<BetterLayers::EditorUI>();
        auto fields = self->m_fields.self();

        // will i make this a ccobject managed obj in the future, mayb if i remember
        fields->settings = std::make_unique<LayerSettings>();

        fields->layerInput = ui::input(LAYER_INPUT_SIZE, "All")
            .id("layer-input"_spr)
            .filter("aA1234567890")
            .maxCharCount(4)
            .callback([self] (const std::string& pStr) {
                if (pStr == "a" || pStr == "A") {
                    editor::setLayer(editor::constants::ALL_LAYERS);
                }
                else if (pStr.contains('A') || pStr.contains('l')) {
                    self->m_fields->layerInput->setString(string::remove(pStr, "Al"));
                }
                else if (!pStr.empty()) {
                    if (auto res = utils::numFromString<int>(pStr); res.isOk()) {
                        editor::setLayer(std::clamp(res.unwrap(), -1, editor::constants::MAX_LAYERS));
                    }
                }
            });

        fields->allLayersButton = ui::buttonFrame(
            // girl robtop, ur function names, what the fuck is this, why only name it layer here </3
            "GJ_arrow_02_001.png", self, menu_selector(BetterLayers::EditorUI::onGoToBaseLayer)
        )
            .id("next-free-layer-button"_spr)
            .scaleToFit(LAYER_EXTRA_BUTTON_SIZE);

        fields->lockLayerButton = ui::togglerFrame(
            "warpLockOffBtn_001.png", "warpLockOnBtn_001.png", self, menu_selector(BetterLayers::EditorUI::onToggleLayerLocked), 1.25f, 1.25f
        )
            .id("lock-layer-button"_spr)
            .scaleToFit(LAYER_EXTRA_BUTTON_SIZE)
            .visible(BetterLayers::lockButton);

        auto oldLayerMenu = self->m_currentLayerLabel->getParent();
        oldLayerMenu->setVisible(false);

        if (self->m_currentLayerLabel) {
            self->m_currentLayerLabel->setPositionX(999.0f); // i give up dealing with better edits editablelabelproxy fuck that
        }

        fields->newLayerMenu = ui::menu(ui::row()
            .alignment(AxisAlignment::Start)
            .gap(GAP)
            .autoScale(false)
            .grow()
        )
            .id("new-layer-menu"_spr)
            .children(
                    ui::buttonFrame(
                        "GJ_optionsBtn_001.png", self, menu_selector(BetterLayers::EditorUI::onLayerSettings)
                    )
                        .id("layer-settings-button"_spr)
                        .scaleToFit((LAYER_SHIFT_BUTTON_SIZE + LAYER_EXTRA_BUTTON_SIZE) / 2)
                        .visible(BetterLayers::layerSettingsButton),
                    fields->lockLayerButton,
                    fields->allLayersButton,
                    ui::buttonFrame(
                        "GJ_arrow_03_001.png", self, menu_selector(BetterLayers::EditorUI::onGroupDown)
                    )
                        .id("prev-layer-button"_spr)
                        .scaleToFit(LAYER_SHIFT_BUTTON_SIZE),
                    fields->layerInput,
                    ui::buttonFrame(
                        "GJ_arrow_03_001.png", self, menu_selector(BetterLayers::EditorUI::onGroupUp)
                    )
                        .id("next-layer-button"_spr)
                        .scaleToFit(LAYER_SHIFT_BUTTON_SIZE)
                        .flipX(),
                    ui::buttonFrame(
                        "GJ_plusBtn_001.png", self, menu_selector(BetterLayers::EditorUI::onNextFreeLayer)
                    )
                        .id("next-free-layer-button"_spr)
                        .scaleToFit(LAYER_EXTRA_BUTTON_SIZE)
                        .visible(BetterLayers::nextFreeButton)
                )
            .parent(self)
            .addTo(self->m_uiItems);

        self->m_editorLayer->m_currentLayer = self->m_editorLayer->m_level->m_lastBuildGroupID;
    }

    void Feature::onUIUpdated(float pScale) {
        editor::ui<BetterLayers::EditorUI>()->updateLayerMenu();
    }
}