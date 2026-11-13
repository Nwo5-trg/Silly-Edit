#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;

namespace SillyKeybinds {
    void Feature::onEditor() {
        auto self = editor::ui();

        feature.registerKeybind<"make-object-invisible">([self] (bool pDown, bool) {
            if (!pDown || selection::empty()) {
                return;
            }

            if (sillyedit::settings::invisibleWithGroup) {
                bool hasGroup = false;

                for (auto obj : selection::getExt()) {
                    hasGroup = object::hasGroup(obj, sillyedit::settings::invisibleWithGroup);

                    if (!hasGroup) {
                        break;
                    }
                }

                if (hasGroup) {
                    object::removeGroup(selection::get(), sillyedit::settings::invisibleWithGroup);
                }
                else {
                    object::addGroup(selection::get(), sillyedit::settings::invisibleWithGroup);
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
        });

        feature.registerKeybind<"restart-playtest">([self] (bool pDown, bool pRepeat) {
            if (!pDown || pRepeat || !editor::isPlaytesting()) {
                return;
            }

            editor::ui()->m_playtestStopBtn->activate();
            editor::ui()->m_playtestBtn->activate();
        });
    }
}