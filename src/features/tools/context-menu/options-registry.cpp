#include <utils/include.hpp>
#include "include.hpp"

using namespace geode::prelude;

namespace ContextMenu {
    OptionsRegistry::OptionsRegistry() {
        Option::registerOptions(m_options);
    }

    std::vector<const Option*> OptionsRegistry::forObjects(CCArray* pObjects) const {
        auto ui = editor::ui();

        std::vector<const Option*> out;

        const auto count = pObjects->count();

        for (const auto& option : m_options) {
            if (option.type == Option::Type::Spacer) {
                if (out.size() && out.back()->type != Option::Type::Spacer) {
                    out.push_back(&option);
                }
                
                continue;
            }

            switch (option.condition.type) {
                case Option::Condition::Default: {
                    if (!count) {
                        continue;
                    }
                break; }
                case Option::Condition::DisallowMultiple: {
                    if (count != 1) {
                        continue;
                    }
                break; }
                case Option::Condition::StaticOnly: {
                    if (count) {
                        continue;
                    }
                break; }
                default: break;
            }

            if (option.condition.predicate && !option.condition.predicate()) {
                continue;
            }

            out.push_back(&option);
        }

        return out;
    }
}