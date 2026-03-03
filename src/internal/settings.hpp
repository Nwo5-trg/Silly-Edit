#pragma once

enum class SettingCategory {
    General,
    Keybinds,
    DefaultObjectOptions,
    Ruler,
    BetterScale,
    ReplaceObjects,
    FreeSnap,
    TextObjectUtils,
    CopyPasteObjectStrings,
    BetterSelectAll,
    ObjectTabIcons
};

#define SE_SETTING_4(pCategory, pName, pType, pVarName) SE_SETTING_5(pCategory, pName, pType, pVarName,)
#define SE_SETTING_5(pCategory, pName, pType, pVarName, pDefault) SE_SETTING_6(pCategory, pName, pType, pVarName, pDefault, {})
#define SE_SETTING_6(pCategory, pName, pType, pVarName, pDefault, pDescription) \
inline Setting< pType > pVarName { pCategory , pName , "", pDefault, pDescription};
#define SE_SETTING(...) GEODE_INVOKE(GEODE_CONCAT(SE_SETTING_, GEODE_NUMBER_OF_ARGS(__VA_ARGS__)), __VA_ARGS__)

#define SE_CATEGORY_1(pName) SE_CATEGORY_2(pName, 0)
#define SE_CATEGORY_2(pName, pPriority) SE_CATEGORY_3(pName, "", pPriority)
#define SE_CATEGORY_3(pName, pLogoPath, pPriority) \
inline auto GEODE_CONCAT(se_categoryDecl, __COUNTER__) = SettingsManager::get()->registerCategory({ pName , pLogoPath , pPriority });
#define SE_CATEGORY(...) GEODE_INVOKE(GEODE_CONCAT(SE_CATEGORY_, GEODE_NUMBER_OF_ARGS(__VA_ARGS__)), __VA_ARGS__)

enum class SettingType {
    Bool,
    Int,
    Float,
    RGB,
    RGBA,
    File,
    String,
    Custom
};


template<typename ImplT, typename T = std::decay_t<ImplT>>
constexpr SettingType getSettingType() {
    return SettingType::Custom;
}

template<typename ImplT, typename T = std::decay_t<ImplT>>
requires std::same_as<T, bool>
constexpr SettingType getSettingType() {
    return SettingType::Bool;
}

template<typename ImplT, typename T = std::decay_t<ImplT>>
requires (!std::same_as<T, bool>) && std::is_integral_v<T>
constexpr SettingType getSettingType() {
    return SettingType::Int;
}

template<typename ImplT, typename T = std::decay_t<ImplT>>
requires std::is_floating_point_v<T>
constexpr SettingType getSettingType() {
    return SettingType::Float;
}

template<typename ImplT, typename T = std::decay_t<ImplT>>
requires std::same_as<T, cocos2d::ccColor3B>
constexpr SettingType getSettingType() {
    return SettingType::RGB;
}

template<typename ImplT, typename T = std::decay_t<ImplT>>
requires std::same_as<T, cocos2d::ccColor4B>
constexpr SettingType getSettingType() {
    return SettingType::RGBA;
}

template<typename ImplT, typename T = std::decay_t<ImplT>>
requires std::same_as<T, std::filesystem::path>
constexpr SettingType getSettingType() {
    return SettingType::File;
}

template<typename ImplT, typename T = std::decay_t<ImplT>>
requires std::same_as<T, std::string>
constexpr SettingType getSettingType() {
    return SettingType::String;
}

class SettingBase {
protected:
    std::string m_name;
    std::string m_category;
    std::string m_key;

    std::string m_description;

    SettingType m_type;

    bool m_loaded = false;
    
    virtual ~SettingBase() = default;

    static auto generateKey(std::string_view pCategory, std::string_view pName) {
        if (pCategory.empty()) {
            // im prolly gonna forget to change this to *not* construct a new string 
            // when geode v5 changes everything to str view params but note to self to do that
            // nvm geode v5 didnt make this string view </3
            return geode::utils::string::replace(geode::utils::string::toLower(std::string{pName}), " ", "-");
        }
        else {
            return geode::utils::string::replace(
                geode::utils::string::toLower(fmt::format("{}-{}", pCategory, pName)),
                " ", "-"
            );
        }
    }

public:
    SettingBase(std::string_view pCategory, std::string_view pName, std::string_view pKey, std::string_view pDescription, SettingType pType);

    const auto& getName() const {
        return m_name;
    }
    const auto& getCategory() const {
        return m_category;
    }
    bool hasCategory() const {
        return !m_category.empty();
    }
    const auto& getKey() const {
        return m_key;
    }
    const auto& getDescription() const {
        return m_description;
    }
    bool hasDescription() const {
        return !m_description.empty();
    }
    bool isLoaded() const {
        return m_loaded;
    }
    auto getType() const {
        return m_type;
    }
    
    virtual bool hasDefault() const = 0;

    virtual void save() const = 0;
    virtual void load() = 0;
    virtual void reset() = 0;
};

template<typename T>
class Setting final : public SettingBase {
private:
    T m_value{};
    T m_defaultValue{};
    bool m_hasDefault = false;

public:
    Setting(std::string_view pName)
        : SettingBase("", pName, "", "", getSettingType<T>()) {}
    Setting(std::string_view pName, T pDefault)
        : SettingBase("", pName, "", "", getSettingType<T>()), m_defaultValue(pDefault), m_hasDefault(true) {}
    Setting(std::string_view pName, std::string_view pKey)
        : SettingBase("", pName, pKey, "", getSettingType<T>()) {}
    Setting(std::string_view pName, std::string_view pKey, T pDefault)
        : SettingBase("", pName, pKey, "", getSettingType<T>()), m_defaultValue(pDefault), m_hasDefault(true) {}
    Setting(std::string_view pCategory, std::string_view pName, std::string_view pKey)
        : SettingBase(pCategory, pName, pKey, "", getSettingType<T>()) {}
    Setting(std::string_view pCategory, std::string_view pName, std::string_view pKey, T pDefault)
        : SettingBase(pCategory, pName, pKey, "", getSettingType<T>()), m_defaultValue(pDefault), m_hasDefault(true) {}
    Setting(std::string_view pCategory, std::string_view pName, std::string_view pKey, std::string_view pDescription)
        : SettingBase(pCategory, pName, pKey, pDescription, getSettingType<T>()) {}
    Setting(std::string_view pCategory, std::string_view pName, std::string_view pKey, T pDefault, std::string_view pDescription)
        : SettingBase(pCategory, pName, pKey, pDescription, getSettingType<T>()), m_defaultValue(pDefault), m_hasDefault(true) {}

    operator const T&() {
        return m_value;
    }
    Setting& operator=(const T& pVal) {
        set(pVal);
        return *this;
    }

    const T& get() const {
        return m_value;
    }
    T set(const T& pVal) {
        const auto ret = m_value;

        m_value = pVal;
        save();

        return ret;
    }

    T getDefault() const {
        return m_defaultValue;
    }
    virtual bool hasDefault() const override {
        return m_hasDefault;
    }

    virtual void save() const override {
        geode::Mod::get()->setSavedValue(m_key, m_value);
    }
    virtual void load() override {
        if (m_loaded) {
            return;
        }

        if (geode::Mod::get()->hasSavedValue(m_key)) {
            m_value = geode::Mod::get()->getSavedValue<T>(m_key);
        }
        else {
            set(m_defaultValue);
        }

        m_loaded = true;
    }
    virtual void reset() override {
        set(m_defaultValue);
    }
};

class Category final {
private:
    std::string m_name;
    std::string m_logoPath;
    int m_priority;
    
    std::vector<SettingBase*> m_settings;

public:
    template<typename T = int>
    requires std::is_scoped_enum_v<T> || std::same_as<T, int>
    Category(std::string_view pName, T pPriority = {})
        : m_name(pName), m_priority(static_cast<int>(pPriority))  {}
    template<typename T = int>
    requires std::is_scoped_enum_v<T> || std::same_as<T, int>
    Category(std::string_view pName, std::string_view pLogoPath, T pPriority = {})
        : m_name(pName), m_logoPath(pLogoPath), m_priority(static_cast<int>(pPriority)) {}

    bool registerSetting(SettingBase* pSetting) {
        if (!pSetting || getSetting(pSetting->getName())) {
            return false;
        }

        m_settings.push_back(pSetting);

        return true;
    }

    template<typename ImplT = void, typename T = std::conditional_t<std::same_as<ImplT, void>, SettingBase, Setting<ImplT>>>
    T* getSetting(std::string_view pName) const {
        auto it = std::ranges::find_if(m_settings, [&] (const auto& pSetting) {
            return pSetting->getName() == pName;
        });
        
        return it == m_settings.end() ? nullptr : *it;
    }

    const auto& getName() const {
        return m_name;
    }
    const auto& getLogoPath() const {
        return m_logoPath;
    }
    bool hasLogo() const {
        return !m_logoPath.empty();
    }
    auto getPriority() const {
        return m_priority;
    }
    const auto& getSettings() const {
        return m_settings;
    }
};

class SettingsManager final {
private:
    std::vector<SettingBase*> m_queuedSettings;

    std::vector<SettingBase*> m_allSettings;
    std::vector<Category> m_categories;
    Category m_uncategorized {"Uncategorized", "", std::numeric_limits<int>::min()};

public:
    static auto* get() {
        static SettingsManager inst;
        return &inst;
    };

    bool registerSetting(SettingBase* pSetting) {
        if (!pSetting) {
            return false;
        }

        m_queuedSettings.push_back(pSetting);

        return false;
    }
    template<typename ImplT = void, typename T = std::conditional_t<std::same_as<ImplT, void>, SettingBase, Setting<ImplT>>>
    T* getSetting(std::string_view pName) {
        auto it = std::ranges::find_if(m_allSettings, [&] (const auto& pSetting) {
            return pSetting->getName() == pName;
        });
        
        return it == m_allSettings.end() ? nullptr : *it;
    }

    bool registerCategory(Category pCategory) {
        if (getCategory(pCategory.getName())) {
            return false;
        }

        m_categories.push_back(std::move(pCategory));

        std::ranges::sort(m_categories, [] (const auto& pA, const auto& pB) {
            return pA.getPriority() < pB.getPriority();
        });
        
        return true;
    }
    template<typename... Args>
    void registerCategories(Args... pArgs) {
        (registerCategory(std::move(pArgs)), ...);
    }
    Category* getCategory(std::string_view pName) {
        auto it = std::ranges::find_if(m_categories, [&] (const auto& pCategory) {
            return pCategory.getName() == pName;
        });

        return it == m_categories.end() ? nullptr : &(*it);
    } 

    const auto& allSettings() const {
        return m_allSettings;
    }
    const auto& getCategories() const {
        return m_categories;
    }

    void load() {
        for (auto setting : m_queuedSettings) {
            bool shouldLoad = true;

            if (!setting->hasCategory()) {
                shouldLoad = m_uncategorized.registerSetting(setting);
            }
            else if (auto ptr = getCategory(setting->getCategory())) {
                shouldLoad = ptr->registerSetting(setting);
            }
            else {
                Category category{setting->getCategory()};

                category.registerSetting(setting);

                registerCategory(std::move(category));
            }

            if (shouldLoad) {
                m_allSettings.push_back(setting);

                setting->load();
            }
        }

        m_queuedSettings.clear();
    }
};

constexpr std::string_view SETTING_GENERATE_KEY = "";

inline SettingBase::SettingBase(std::string_view pCategory, std::string_view pName, std::string_view pKey, std::string_view pDescription, SettingType pType)
    : m_name(pName), m_category(pCategory), m_description(pDescription), m_key(pKey.empty() ? generateKey(pCategory, pName) : pKey), m_type(pType) {
        SettingsManager::get()->registerSetting(this);
    }