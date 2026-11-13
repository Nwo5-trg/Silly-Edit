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
        void addOption(int iD, int key, T val) {
            m_options[iD].append(fmt::format(",{},{}", key, val));

            if (!iD) {
                m_defaultOption = true;
            }
        }
        template<>
        void addOption<bool>(int iD, int key, bool val) {
            m_options[iD].append(fmt::format(",{},{}", key, static_cast<int>(val)));

            if (!iD) {
                m_defaultOption = true;
            }
        }
        void addOption(int iD, std::string_view customString);

        bool idHasOptions(int iD) const;
        // starts with ,
        std::string getOptionsStringForID(int iD) const;

        bool defaultExists() const;
        // starts with ,
        std::string getDefaultOptionsString() const;

        void updateSimpleOptionsString(bool dontFade, bool dontEnter, bool noGlow);
        // starts with ,
        std::string getSimpleOptionsString() const;

        void reset();
    };

    void parseOptions(ObjectOptions& objectOptions);
}