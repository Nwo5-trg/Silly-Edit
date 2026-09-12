#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;
using namespace nwo5::ui::prelude;

namespace ContextMenu {
    void EditorUI::keyDown(enumKeyCodes key, double timestamp) {
        if (key == cocos2d::KEY_Escape) {
            if (auto menu = m_fields->menu; menu && menu->isVisible()) {
                menu->hide();

                return;
            }
        }
       
        GD::EditorUI::keyDown(key, timestamp);
    }

    void EditorUI::moveObjectCall(EditCommand command) {
        if (auto menu = m_fields->menu; menu && menu->isVisible()) {
            menu->hide();
        }
        
        GD::EditorUI::moveObjectCall(command);
    }

    bool EditorUI::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
        if (!GD::EditorUI::ccTouchBegan(touch, event)) {
            return false;
        }

        if (auto menu = m_fields->menu; menu && menu->isVisible()) {
            menu->hide();
        }

        return true;
    }





    void Feature::onEditor() {
        auto self = editor::ui<ContextMenu::EditorUI>();
        
        auto menu = self->m_fields->menu = Setup(ContextMenuNode::create())
            .order(15)
            .parent(self);

        feature.registerKeybind<"modifier">([self] (bool pDown, bool) {
            self->m_fields->modifierDown = pDown;
        });

        self->addEventListener(MouseInputEvent(), [self, menu] (const MouseInputData& pData) {
            if (!ContextMenu::enabled() || !menu) {
                if (menu) {
                    menu->hide();
                }

                return;
            }
            
            if (pData.action != MouseInputData::Action::Press || pData.button != MouseInputData::Button::Right) {
                return;
            }

            menu->hide();

            const auto mousePos = cocos::getMousePos();

            if (mousePos.y < self->m_toolbarHeight) {
                return;
            }

            const auto pos = self->m_editorLayer->m_objectLayer->convertToNodeSpace(mousePos);
            
            auto obj = self->m_editorLayer->objectAtPosition(pos);

            if (obj && obj->m_isSelected) {
                menu->show(mousePos, selection::get());
            }
            else if (obj && ContextMenu::selectOnClick) {
                selection::set(obj);  
                editor::update();

                menu->show(mousePos, selection::get());
            }
            else {
                menu->show(mousePos, CCArray::create());
            }
        });

        self->addEventListener(ScrollWheelEvent(), [self, menu] (double, double) {
            if (menu && menu->isVisible()) {
                if (ContextMenu::scrollAway) {
                    menu->hide();
                }
                else {
                    sillyedit::utils::shouldBlockScrolling() = true;
                }
            }
        }, Priority::Early);

        self->addEventListener(OnPlaytestEvent(true), [self, menu] {
            if (menu) {
                menu->hide();

                return;
            }
        });

        self->addEventListener(ObjectsDeselectedEvent(), [self, menu] (auto) {
            if (menu) {
                menu->hide();

                return;
            }
        });

        self->addEventListener(ObjectsDeletedEvent(), [self, menu] (auto) {
            if (menu) {
                menu->hide();

                return;
            }
        });
    }
}