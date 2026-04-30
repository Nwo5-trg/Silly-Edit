#include "include.hpp"

using namespace geode::prelude;

namespace DefaultObjectOptions {
    void ObjectOptions::addOption(int pID, std::string_view pCustomString) {
        m_options[pID].append(fmt::format(",{}", pCustomString));

        if (!pID) {
            m_defaultOption = true;
        }
    }

    bool ObjectOptions::idHasOptions(int pID) const {
        return m_options.find(pID) != m_options.end();
    }
    // starts with ,
    std::string ObjectOptions::getOptionsStringForID(int pID) const {
        if (const auto it = m_options.find(pID); it != m_options.end()) {
            return it->second;
        }

        return "";
    }

    bool ObjectOptions::defaultExists() const {
        return m_defaultOption;
    }
    std::string ObjectOptions::getDefaultOptionsString() const {
        return getOptionsStringForID(DEFAULT_OPTION_ID);
    }

    void ObjectOptions::updateSimpleOptionsString(bool pDontFade, bool pDontEnter, bool pNoGlow) {
        // 64 == dont fade prop, 67 == dont enter prop, 96 == no glow prop
        m_simpleOptionsString =  fmt::format(
            ",64,{},67,{},96,{}", 
            static_cast<int>(pDontFade), 
            static_cast<int>(pDontEnter), 
            static_cast<int>(pNoGlow)
        );
    }
    std::string ObjectOptions::getSimpleOptionsString() const {
        return m_simpleOptionsString;
    }

    void ObjectOptions::reset() {
        m_options.clear();
        m_simpleOptionsString.clear();
        m_defaultOption = false;
    }
}