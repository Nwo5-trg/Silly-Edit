#pragma once

#include <utils/include.hpp>

namespace DefaultObjectOptions {
    class ObjectOptions final {
    protected:
        std::array<std::string, nwo5::editor::constants::OBJECT_IDS + 1> m_options;
        bool m_defaultOption = false;

        std::string m_simpleOptionsString;

    public:
        static constexpr size_t DEFAULT_OPTION_ID = 0;

        template<typename T>
        void addOption(int pID, int pKey, T pVal) {
            m_options[pID].append(fmt::format(",{},{}", pKey, pVal));

            if (!pID) {
                m_defaultOption = true;
            }
        }
        template<>
        void addOption<bool>(int pID, int pKey, bool pVal) {
            m_options[pID].append(fmt::format(",{},{}", pKey, static_cast<int>(pVal)));

            if (!pID) {
                m_defaultOption = true;
            }
        }
        void addOption(int pID, std::string_view pCustomString);

        bool idHasOptions(int pID) const;
        // starts with ,
        std::string getOptionsStringForID(int pID) const;

        bool defaultExists() const;
        // starts with ,
        std::string getDefaultOptionsString() const;

        void updateSimpleOptionsString(bool pDontFade, bool pDontEnter, bool pNoGlow);
        // starts with ,
        std::string getSimpleOptionsString() const;

        void reset();
    };

    void parseOptions(ObjectOptions& pObjectOptions);
}