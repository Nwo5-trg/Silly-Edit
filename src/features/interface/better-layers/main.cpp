#include <utils/include.hpp>
#include "shared.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

namespace BetterLayers {
    EditorUI::Fields::~Fields() {
        Shared::getLayerSettingsPtr() = nullptr;
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
            fields->layerInput->setString(nwo5::utils::numToString(layer));
        }
        
        if (editor::layerLocked(layer)) {
            fields->layerInput->getInputNode()->getTextLabel()->setColor({255, 200, 2});
        }
        else {
            fields->layerInput->getInputNode()->getTextLabel()->setColor(ccWHITE);
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
                .pos(oldLayerMenu->getPositionX() + oldLayerMenu->getScaledContentWidth() / 2, oldLayerMenu->getPositionY())
                .order(oldLayerMenu);
        }
    }



    void EditorUI::onModify(auto& pSelf) {
        (void)pSelf.setHookPriorityAfterPost("EditorUI::init", nwo5::utils::TINKER_EDIT_ID);
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

    void EditorUI::selectObject(GameObject* object, bool ignoreFilter) {
        if (!BetterLayers::enabled()) {
            return GD::EditorUI::selectObject(object, ignoreFilter);
        }

        if (ignoreFilter == true) {
            return GD::EditorUI::selectObject(object, ignoreFilter);
        }

        bool canSelect = true;

        if (BetterLayers::unselectableUnfocusedLayers) {
            if (auto res = m_fields->settings->getFocusedLayer(); res.has_value() && object->m_editorLayer != res.value() && object->m_editorLayer2 != res.value()) {
                canSelect = false;
            }
        }
        if (BetterLayers::unselectableHiddenLayers) {
            canSelect = !m_fields->settings->isLayerHidden(object->m_editorLayer) && !m_fields->settings->isLayerHidden(object->m_editorLayer2);
        }

        if (canSelect) {
            GD::EditorUI::selectObject(object, ignoreFilter);
        }
        else if (m_fields->canDestroyUndo) {
            m_editorLayer->m_undoObjects->removeLastObject(false);

            m_fields->canDestroyUndo = false;
        }
    }

    void EditorUI::selectObjects(CCArray* objects, bool ignoreFilter) {
        if (!BetterLayers::enabled()) {
            return GD::EditorUI::selectObjects(objects, ignoreFilter);
        }

        if (ignoreFilter) {
            return GD::EditorUI::selectObjects(objects, ignoreFilter);
        }

        auto validObjs = CCArray::create();

        for (auto obj : CCArrayExt<GameObject*>(objects)) {
            if (BetterLayers::unselectableUnfocusedLayers) {
                if (auto res = m_fields->settings->getFocusedLayer(); res.has_value() && obj->m_editorLayer != res.value() && obj->m_editorLayer2 != res.value()) {
                    continue;
                }
            }
            if (BetterLayers::unselectableHiddenLayers) {
                if (m_fields->settings->isLayerHidden(obj->m_editorLayer) || m_fields->settings->isLayerHidden(obj->m_editorLayer2)) {
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
        auto self = editor::ui<BetterLayers::EditorUI>();

        if (!BetterLayers::enabled()) {
            return;
        }
        
        auto fields = self->m_fields.self();

        // will i make this a ccobject managed obj in the future, mayb if i remember
        fields->settings = std::make_unique<LayerSettings>();
        Shared::getLayerSettingsPtr() = fields->settings.get();

        fields->layerInput = ui::node(Setup(ui::input(LAYER_INPUT_SIZE, "All"))
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
                        editor::setLayer(std::clamp(res.unwrap(), 0, editor::constants::MAX_LAYERS));
                    }
                }
            })
        );

        fields->allLayersButton = ui::node(Setup(ui::buttonFrame(
            // girl robtop, ur function names, what the fuck is this, why only name it layer here </3
            "GJ_arrow_02_001.png", self, menu_selector(BetterLayers::EditorUI::onGoToBaseLayer)
        ))
            .id("next-free-layer-button"_spr)
            .scaleToFit(LAYER_EXTRA_BUTTON_SIZE)
        );

        fields->lockLayerButton = ui::node(Setup(ui::togglerFrame(
            "warpLockOffBtn_001.png", "warpLockOnBtn_001.png", self, menu_selector(BetterLayers::EditorUI::onToggleLayerLocked), 1.25f, 1.25f
        ))
            .id("lock-layer-button"_spr)
            .scaleToFit(LAYER_EXTRA_BUTTON_SIZE)
            .visible(BetterLayers::lockButton)
        );

        auto oldLayerMenu = self->m_currentLayerLabel->getParent();
        oldLayerMenu->setVisible(false);

        if (self->m_currentLayerLabel) {
            self->m_currentLayerLabel->setPositionX(999.0f); // i give up dealing with better edits editablelabelproxy fuck that
        }

        fields->newLayerMenu = ui::node(Setup(ui::menu(ui::horizontalDistrbLayout(GAP)))
            .id("new-layer-menu"_spr)
            .children(
                    Setup(ui::buttonFrame(
                        "GJ_optionsBtn_001.png", self, menu_selector(BetterLayers::EditorUI::onLayerSettings)
                    ))
                        .id("layer-settings-button"_spr)
                        .scaleToFit((LAYER_SHIFT_BUTTON_SIZE + LAYER_EXTRA_BUTTON_SIZE) / 2),
                    fields->lockLayerButton,
                    fields->allLayersButton,
                    Setup(ui::buttonFrame(
                        "GJ_arrow_03_001.png", self, menu_selector(BetterLayers::EditorUI::onGroupDown)
                    ))
                        .id("prev-layer-button"_spr)
                        .scaleToFit(LAYER_SHIFT_BUTTON_SIZE),
                    fields->layerInput,
                    Setup(ui::buttonFrame(
                        "GJ_arrow_03_001.png", self, menu_selector(BetterLayers::EditorUI::onGroupUp)
                    ))
                        .id("next-layer-button"_spr)
                        .scaleToFit(LAYER_SHIFT_BUTTON_SIZE)
                        .flipX(),
                    Setup(ui::buttonFrame(
                        "GJ_plusBtn_001.png", self, menu_selector(BetterLayers::EditorUI::onNextFreeLayer)
                    ))
                        .id("next-free-layer-button"_spr)
                        .scaleToFit(LAYER_EXTRA_BUTTON_SIZE)
                        .visible(BetterLayers::nextFreeButton)
                )
            .parent(self)
            .addTo(self->m_uiItems)
        );

        self->m_editorLayer->m_currentLayer = self->m_editorLayer->m_level->m_lastBuildGroupID;
    }

    void Feature::onUIUpdated(float pScale) {
        editor::layer<BetterLayers::EditorUI>()->updateLayerMenu();
    }
}