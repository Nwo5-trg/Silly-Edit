#pragma once

namespace Utils {
    struct GetChildQueryBase {};
    template<typename ImplT = cocos2d::CCNode, typename T = std::remove_pointer_t<ImplT>>
    struct GetChildQuery : GetChildQueryBase {
        const std::string id;
        const int index{};

        GetChildQuery()
            = default;
        GetChildQuery(int pIndex)
            : index(pIndex) {}
        GetChildQuery(std::string_view pID)
            : id(pID) {}

        cocos2d::CCNode* query(cocos2d::CCNode* pNode) {
            if (id.empty()) {
                return pNode->getChildByType<T*>(index);
            }
            else {
                if constexpr (std::same_as<T, cocos2d::CCNode>) {
                    return pNode->getChildByID(id);
                }
                else {
                    return geode::cast::typeinfo_cast<T*>(pNode->getChildByID(id));
                }
            }
        }
    };
    template<typename ImplT = cocos2d::CCNode, typename T = std::remove_pointer_t<ImplT>>
    using GetChildQueryID = GetChildQuery<T>;
    template<typename ImplT = cocos2d::CCNode, typename T = std::remove_pointer_t<ImplT>>
    using GetChildQueryIndex = GetChildQuery<T>;
    template<typename ImplT = cocos2d::CCNode, typename T = std::remove_pointer_t<ImplT>>
    using GetChildQueryType = GetChildQuery<T>;

    template<typename ImplT = cocos2d::CCNode, typename T = std::remove_pointer_t<ImplT>, typename... Args>
    requires (std::derived_from<Args, GetChildQueryBase> && ...)
    T* getNestedChildSafe(cocos2d::CCNode* pNode, Args... pArgs) {
        if (!pNode) {
            return nullptr;
        }

        auto node = static_cast<cocos2d::CCNode*>(pNode);

        ([&] -> bool {
            return (node = pArgs.query(node));
        }() && ...);

        if constexpr (std::same_as<T, cocos2d::CCNode>) {
            return node;
        }
        else {
            return geode::cast::typeinfo_cast<T*>(node);
        }
    }

    constexpr float DEFAULT_TEXT_INPUT_HEIGHT = 30.0f;
    using TextInputCallback = std::move_only_function<void(const std::string&)>;

    geode::TextInput* createTextInput(float pWidth, float pHeight, const std::string& pPlaceholder, const std::string& pFont, TextInputCallback pCallback = [] (const std::string&) {});
    geode::TextInput* createTextInput(float pWidth, float pHeight, const std::string& pPlaceholder = "", TextInputCallback pCallback = [] (const std::string&) {});

    CCMenuItemSpriteExtra* createCircleButton(const std::string& pTexture, geode::CircleBaseColor pColor, geode::CircleBaseSize pSize, cocos2d::CCObject* pTarget, cocos2d::SEL_MenuHandler pCallback, float pScale = 1.0f);
    CCMenuItemSpriteExtra* createCircleButton(const std::string& pTexture, geode::CircleBaseColor pColor, cocos2d::CCObject* pTarget, cocos2d::SEL_MenuHandler pCallback, float pScale = 1.0f);
    CCMenuItemSpriteExtra* createCircleButtonFrame(const std::string& pTexture, geode::CircleBaseColor pColor, geode::CircleBaseSize pSize, cocos2d::CCObject* pTarget, cocos2d::SEL_MenuHandler pCallback, float pScale = 1.0f);
    CCMenuItemSpriteExtra* createCircleButtonFrame(const std::string& pTexture, geode::CircleBaseColor pColor, cocos2d::CCObject* pTarget, cocos2d::SEL_MenuHandler pCallback, float pScale = 1.0f);
    CCMenuItemSpriteExtra* createButton(const std::string& pTexture, cocos2d::CCObject* pTarget, cocos2d::SEL_MenuHandler pCallback, float pScale = 1.0f);
    CCMenuItemSpriteExtra* createButtonFrame(const std::string& pTexture, cocos2d::CCObject* pTarget, cocos2d::SEL_MenuHandler pCallback, float pScale = 1.0f);
    
    constexpr cocos2d::CCPoint TOP_RIGHT_ANCHOR = {1.0f, 1.0f};
    constexpr cocos2d::CCPoint TOP_CENTER_ANCHOR = {0.5f, 1.0f};
    constexpr cocos2d::CCPoint TOP_LEFT_ANCHOR = {0.0f, 1.0f};
    constexpr cocos2d::CCPoint LEFT_CENTER_ANCHOR = {0.0f, 0.5f};
    constexpr cocos2d::CCPoint BOTTOM_LEFT_ANCHOR = {0.0f, 0.0f};
    constexpr cocos2d::CCPoint BOTTOM_CENTER_ANCHOR = {0.5f, 0.0f};
    constexpr cocos2d::CCPoint BOTTOM_RIGHT_ANCHOR = {1.0f, 0.0f};
    constexpr cocos2d::CCPoint RIGHT_CENTER_ANCHOR = {1.0f, 0.5f};
    constexpr cocos2d::CCPoint CENTER_ANCHOR = {0.5f, 0.5f};

    namespace Aliases {  
        using Utils::TOP_RIGHT_ANCHOR, Utils::TOP_CENTER_ANCHOR, Utils::TOP_LEFT_ANCHOR,
        Utils::LEFT_CENTER_ANCHOR, Utils::BOTTOM_LEFT_ANCHOR, Utils::BOTTOM_CENTER_ANCHOR,
        Utils::BOTTOM_RIGHT_ANCHOR, Utils::RIGHT_CENTER_ANCHOR, Utils::CENTER_ANCHOR;

        using Utils::GetChildQuery;
    }
}