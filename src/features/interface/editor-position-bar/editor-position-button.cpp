#include <alphalaneous.alphas-ui-pack/include/API.hpp>
#include "include.hpp"

using namespace nwo5::ui::prelude;
using namespace alpha::prelude;
using namespace geode::prelude;

namespace EditorPositionBar {
    CCSprite* EditorPositionButton::createEditorSnapshot(CCPoint pPos, CCSize pViewSize) {
        const auto originalZoom = editor::zoom();
        const auto originalPos = ui::pos(editor::layer()->m_objectLayer);
        auto ui = editor::ui<EditorPositionBar::EditorUI>();
        
        editor::setZoom(1);
        ui->moveEditorToSavedPos(pPos);
        ui->setVisible(false);

        auto mirror = MirrorNode::create(editor::layer());
        mirror->setViewportSize(pViewSize);

        auto spr = mirror->captureSprite();

        editor::setZoom(originalZoom);
        editor::layer()->m_objectLayer->setPosition(originalPos);
        ui->setVisible(true);

        return spr;
    }

    bool EditorPositionButton::init(int pIndex, CCPoint pPos) {
        const CCSize view{EditorPositionBar::snapshotSize, EditorPositionBar::snapshotSize * (SIZE.height / SIZE.width)} ;
        
        auto nineSlice = *ui::nineSlice(ui::sprite::EMPTY_BACKGROUND);
        nineSlice->setScaleMultiplier(0.5f);

        if (!Button::initWithNode(
            ui::dummy()
                .layout(ui::anchor())
                .size(SIZE)
                .children(
                    Setup(this->createEditorSnapshot(pPos, CCSize{EditorPositionBar::snapshotSize, EditorPositionBar::snapshotSize * (SIZE.height / SIZE.width)}))
                        .stretchToFit(SIZE - 2.5f)
                        .layoutAnchor(Anchor::Center),
                    ui::label(fmt::to_string(pIndex), Font::Default)
                        .scaleHeightToFit(LABEL_HEIGHT)
                        .layoutAnchor(Anchor::Bottom).layoutAnchorOffsetY(2.5f),
                    Setup(nineSlice)
                        .size(SIZE)
                        .layoutAnchor(Anchor::Center)
                ),
            nullptr
        )) {
            return false;
        }

        Setup(this)
            .callback([pIndex] (Button* pSelf) {
                auto& pos = editor::layer()->m_savedPositions[pIndex];

                const CCPoint objLayerPos = (ui::winSize() / 2 - ui::pos(editor::layer()->m_objectLayer) + CCPoint{0.0f, editor::ui()->m_toolbarHeight / 2}) / editor::zoom();

                if (pos.fuzzyEquals(objLayerPos, 5.0f)) {
                    pos = CCPointZero;

                    auto parent = pSelf->getParent();

                    pSelf->removeMeAndCleanup();

                    parent->updateLayout();

                    return;
                }
                
                auto ui = editor::ui<EditorPositionBar::EditorUI>();
                ui->showPositonText(pIndex);
                ui->moveEditorToSavedPos(pos);
            })
            .stretchToFit(SIZE);

        return true;
    }

    EditorPositionButton* EditorPositionButton::create(int pIndex, CCPoint pPos) {
        auto ret = new EditorPositionButton;

        if (!ret->init(pIndex, pPos)) {
            delete ret;

            return nullptr;
        }

        ret->autorelease();

        return ret;
    }
}