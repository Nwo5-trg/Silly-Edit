#include "include.hpp"

using namespace geode::prelude;

namespace DefaultObjectOptions {
    void ObjectOptions::addOption(int iD, std::string_view customString) {
        m_options[iD].append(fmt::format(",{}", customString));

        if (!iD) {
            m_defaultOption = true;
        }
    }

    bool ObjectOptions::idHasOptions(int iD) const {
        return iD >= 0 && iD < m_options.size() && m_options[iD].size();
    }
    // starts with ,
    std::string ObjectOptions::getOptionsStringForID(int iD) const {
        if (iD >= 0 && iD < m_options.size()) {
            return m_options[iD];
        }

        return "";
    }

    bool ObjectOptions::defaultExists() const {
        return m_defaultOption;
    }
    std::string ObjectOptions::getDefaultOptionsString() const {
        return getOptionsStringForID(DEFAULT_OPTION_ID);
    }

    void ObjectOptions::updateSimpleOptionsString(bool dontFade, bool dontEnter, bool noGlow) {
        // 64 == dont fade prop, 67 == dont enter prop, 96 == no glow prop
        m_simpleOptionsString =  fmt::format(
            ",64,{},67,{},96,{}", 
            static_cast<int>(dontFade), 
            static_cast<int>(dontEnter), 
            static_cast<int>(noGlow)
        );
    }
    std::string ObjectOptions::getSimpleOptionsString() const {
        return m_simpleOptionsString;
    }

    void ObjectOptions::reset() {
        m_options.fill("");
        m_simpleOptionsString.clear();
        m_defaultOption = false;
    }
}