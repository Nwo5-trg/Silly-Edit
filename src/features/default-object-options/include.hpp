#pragma once

namespace DefaultObjectOptions {
    class ObjectOptions {
    protected:
        std::unordered_map<int, std::string> options_;
        bool defaultOption_ = false;

        std::string simpleOptionsString_;

    public:
        static constexpr size_t DEFAULT_OPTION_ID = 0;

        template<typename T>
        void addOption(int pID, int pKey, T pVal) {
            options_[pID].append(fmt::format(",{},{}", pKey, pVal));

            if (!pID) {
                defaultOption_ = true;
            }
        }
        void addOption(int pID, std::string_view pCustomString) {
            options_[pID].append(fmt::format(",{}", pCustomString));

            if (!pID) {
                defaultOption_ = true;
            }
        }

        bool idHasOptions(int pID) const {
            return options_.find(pID) != options_.end();
        }
        // starts with ,
        std::string getOptionsStringForID(int pID) const {
            if (const auto it = options_.find(pID); it != options_.end()) {
                return it->second;
            }

            return {};
        }

        bool hasDefault() const {
            return defaultOption_;
        }
        // starts with ,
        std::string getDefaultOptionsString() const {
            return getOptionsStringForID(DEFAULT_OPTION_ID);
        }

        void updateSimpleOptionsString(bool pDontFade, bool pDontEnter, bool pNoGlow) {
            // 64 == dont fade prop, 67 == dont enter prop, 96 == no glow prop
            simpleOptionsString_ =  fmt::format(
                ",64,{},67,{},96,{}", 
                static_cast<int>(pDontFade), 
                static_cast<int>(pDontEnter), 
                static_cast<int>(pNoGlow)
            );
        }
        // starts with ,
        std::string getSimpleOptionsString() const {
            return simpleOptionsString_;
        }

        void reset() {
            options_.clear();
            simpleOptionsString_.clear();
            defaultOption_ = false;
        }
    };

    void parseOptions(ObjectOptions& pObjectOptions);
}