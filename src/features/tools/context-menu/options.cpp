#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;

namespace ContextMenu {
    void Option::registerOptions(std::deque<Option>& pOptions) {pOptions = {
        Option{"Edit Group", Col::Salmon, {Condition::Default, [] {
            return editor::ui()->m_editGroupBtn->m_animationEnabled;
        }}, [] (bool) {
            editor::ui()->m_editGroupBtn->activate();
        }},
        Option{"Edit Object", Col::Blue, {Condition::Default, [] {
            return editor::ui()->m_editObjectBtn->m_animationEnabled;
        }}, [] (bool) {
            editor::ui()->m_editObjectBtn->activate();
        }},
        Option{"Edit Extras", Col::DarkAqua, {Condition::Default, [] {
            return editor::ui()->m_editGroupBtn->m_animationEnabled;
        }}, [] (bool) {
            auto obj = editor::ui()->m_selectedObject;
            auto objs = editor::ui()->m_selectedObjects;

            SetupObjectOptionsPopup::create(obj, objs, SetGroupIDLayer::create(obj, objs))->show();
        }},
        Option{"Edit Special", Col::Teal, {Condition::Default, [] {
            return editor::ui()->m_editSpecialBtn->m_animationEnabled;
        }}, [] (bool) {
            editor::ui()->m_editSpecialBtn->activate();
        }},
        Option{Type::Spacer},
        Option{"Create Mode", Col::Blue, Condition::StaticOnly, [] (bool) {
            editor::ui()->toggleMode(editor::ui()->m_buildModeBtn);
        }},
        Option{"Edit Mode", Col::Green, Condition::StaticOnly, [] (bool) {
            editor::ui()->toggleMode(editor::ui()->m_editModeBtn);
        }},
        Option{"Delete Mode", Col::DarkGray, Condition::StaticOnly, [] (bool) {
            editor::ui()->toggleMode(editor::ui()->m_deleteModeBtn);
        }},
        Option{Type::Spacer},
        Option{"Flip X", Col::Yellow, [] (bool) {
            editor::ui()->transformObjectCall(EditCommand::FlipX);
        }},
        Option{"Flip Y", Col::Yellow, [] (bool) {
            editor::ui()->transformObjectCall(EditCommand::FlipY);
        }},
        Option{{"Scale", "Scale XY"}, Col::Green, [] (bool pSpecialDown) {
            if (editor::ui()->m_scaleControl->isVisible()) {
                editor::ui()->deactivateScaleControl();
            }
            else {
                editor::activateScaleControl(pSpecialDown);
            }
        }},
        Option{"Transform", Col::Red, [] (bool) {
            if (editor::ui()->m_transformControl->isVisible()) {
                editor::ui()->deactivateTransformControl();
            }
            else {
                editor::activateTransformControl();
            }
        }},
        Option{Type::Spacer},
        Option{"Select All", Col::LightGray, Condition::Static, [] (bool) {
            editor::ui()->selectAll();
        }},
        Option{"Deselect All", Col::Gray, Condition::Static, [] (bool) {
            editor::ui()->deselectAll();

            editor::update();
        }},
        Option{Type::Spacer},
        Option{"Delete", Col::DarkGray, [] (bool) {
            editor::ui()->onDeleteSelected(nullptr);
        }},
        Option{{"Copy", "Copy Values"}, Col::LightBlue, {Condition::Default, [] {
            return editor::ui()->m_copyBtn->m_animationEnabled;
        }}, [] (bool pSpecialDown) {
            if (pSpecialDown) {
                editor::ui()->m_copyValuesBtn->activate();
            }
            else {
                editor::ui()->m_copyBtn->activate();
            }
        }},
        Option{"Paste", Col::Pink, {Condition::Static, [] {
            return editor::ui()->m_pasteBtn->m_animationEnabled;
        }}, [] (bool) {
            editor::ui()->m_pasteBtn->activate();
        }},
        Option{{"Paste State", "Paste Colors"}, Col::Pink, {Condition::Default, [] {
            return editor::ui()->m_pasteStateBtn->m_animationEnabled;
        }}, [] (bool pSpecialDown) {
            if (pSpecialDown) {
                editor::ui()->m_pasteColorBtn->activate();
            }
            else {
                editor::ui()->m_pasteStateBtn->activate();
            }
        }},
        Option{"Duplicate", Col::Orange, {Condition::Default, [] {
            return editor::ui()->m_copyPasteBtn->m_animationEnabled;
        }}, [] (bool) {
            editor::ui()->m_copyPasteBtn->activate();
        }},
        Option{Type::Spacer},
        Option{"Toggle Invisible", Col::White, [] (bool) {
            if (Settings::invisibleWithGroup) {
                bool hasGroup = false;

                for (auto obj : selection::getExt()) {
                    hasGroup = object::hasGroup(obj, Settings::invisibleWithGroup);

                    if (!hasGroup) {
                        break;
                    }
                }

                if (hasGroup) {
                    object::removeGroup(selection::get(), Settings::invisibleWithGroup);
                }
                else {
                    object::addGroup(selection::get(), Settings::invisibleWithGroup);
                }
            }
            else {
                bool isHide = false;

                for (auto obj : selection::getExt()) {
                    isHide = obj->m_isHide;

                    if (!isHide) {
                        break;
                    }
                }

                for (auto obj : selection::getExt()) {
                    obj->m_isHide = !isHide;
                }
            }
        }},
        Option{"Go To Layer", Col::Magenta, [] (bool) {
            std::optional<int> layer = std::nullopt;

            for (auto obj : selection::getExt()) {
                const auto lastLayer = layer;

                layer = obj->m_editorLayer2 ? obj->m_editorLayer2 : obj->m_editorLayer2;

                if (!lastLayer.has_value()) {
                    continue;
                }

                if (lastLayer.value() != layer) {
                    layer = editor::constants::ALL_LAYERS;

                    break;
                }
            }

            editor::setLayer(layer.value());
        }}
    };}

    bool Option::enabled() const {
        return m_setting ? m_setting->get() : false;
    }

    SillySetting<bool>* Option::setupSetting() {
        return static_cast<SillySetting<bool>*>(SettingsManager::get()->getSetting(
            nwo5::settings::generateKey(name.label, feature)
        ));
    }
}