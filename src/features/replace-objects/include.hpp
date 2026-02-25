#pragma once

namespace ReplaceObjects {
    enum class QueryTokenType {
        OrOperator,
        AndOperator,
        NotOperator,
        OpenParentheses,
        ClosingParentheses,
        Data,
        Identifier
    };
    struct QueryToken {
        QueryTokenType type;
        std::string data;
        int location;

        QueryToken(QueryTokenType pType, std::string_view pData, int pLocation)
            : type(pType), data(pData), location(pLocation) {}
    };

    #define SE_SETUP_QUERY(pName) \
        virtual bool evaluate(GameObject* pObj) const; \
        template<typename... Args> \
        static auto create(Args... pArgs) { \
            return std::unique_ptr< QueryExpression > (new pName(std::move(pArgs)...)); \
        }

    class QueryExpression {
    public:
        virtual ~QueryExpression() = default;
        
        virtual bool evaluate(GameObject* pObj) const = 0;
    };
    class QueryConditionOr : QueryExpression {
    protected:
        std::unique_ptr<QueryExpression> m_rhs;
        std::unique_ptr<QueryExpression> m_lhs;
    public:
        QueryConditionOr(std::unique_ptr<QueryExpression> pRHS, std::unique_ptr<QueryExpression> pLHS)
            : m_rhs(std::move(pRHS)), m_lhs(std::move(pLHS)) {}

        SE_SETUP_QUERY(QueryConditionOr)
    };
    class QueryConditionAnd : QueryExpression {
    protected:
        std::unique_ptr<QueryExpression> m_rhs;
        std::unique_ptr<QueryExpression> m_lhs;
    public:
        QueryConditionAnd(std::unique_ptr<QueryExpression> pRHS, std::unique_ptr<QueryExpression> pLHS)
            : m_rhs(std::move(pRHS)), m_lhs(std::move(pLHS)) {}

        SE_SETUP_QUERY(QueryConditionAnd)
    };
    class QueryConditionNot : QueryExpression {
    protected:
        std::unique_ptr<QueryExpression> m_expression;
    public:
        QueryConditionNot(std::unique_ptr<QueryExpression> pExpression)
            : m_expression(std::move(pExpression)) {}
        
        SE_SETUP_QUERY(QueryConditionNot)
    };
    class QueryObjectGroup : QueryExpression {
    protected:
        int m_group;
    public:
        QueryObjectGroup(std::string_view pData)
            : m_group(geode::utils::numFromString<int>(pData).unwrapOrDefault()) {}
        
        SE_SETUP_QUERY(QueryObjectGroup)
    };
    class QueryObjectID : QueryExpression {
    protected:
        int m_id;
    public:
        QueryObjectID(std::string_view pData)
            : m_id(geode::utils::numFromString<int>(pData).unwrapOrDefault()) {}

        SE_SETUP_QUERY(QueryObjectID)
    };
    class QueryTargetGroup : QueryExpression {
    protected:
        int m_group;
    public:
        QueryTargetGroup(std::string_view pData)
            : m_group(geode::utils::numFromString<int>(pData).unwrapOrDefault()) {}

        SE_SETUP_QUERY(QueryTargetGroup)
    };
    class QueryCenterGroup : QueryExpression {
    protected:
        int m_group;
    public:
        QueryCenterGroup(std::string_view pData)
            : m_group(geode::utils::numFromString<int>(pData).unwrapOrDefault()) {}

        SE_SETUP_QUERY(QueryCenterGroup)
    };
    class QueryItemID : QueryExpression {
    protected:
        int m_item;
    public:
        QueryItemID(std::string_view pData)
            : m_item(geode::utils::numFromString<int>(pData).unwrapOrDefault()) {}

        SE_SETUP_QUERY(QueryItemID)
    };
    class QueryObjectBaseColor : QueryExpression {
    protected:
        int m_color;
    public:
        QueryObjectBaseColor(std::string_view pData)
            : m_color(geode::utils::numFromString<int>(pData).unwrapOrDefault()) {}

        SE_SETUP_QUERY(QueryObjectBaseColor)
    };
    class QueryObjectDetailColor : QueryExpression {
    protected:
        int m_color;
    public:
        QueryObjectDetailColor(std::string_view pData)
            : m_color(geode::utils::numFromString<int>(pData).unwrapOrDefault()) {}

        SE_SETUP_QUERY(QueryObjectDetailColor)
    };
    
    geode::Result<std::unique_ptr<QueryExpression>> parseQuery(std::string_view pStr);

    enum class ReplaceTokenType {
        ActionOperator,
        Seperator,
        Data,
        Identifier
    };
    struct ReplaceToken {
        ReplaceTokenType type;
        std::string data;
        int location;

        ReplaceToken(ReplaceTokenType pType, std::string_view pData, int pLocation)
            : type(pType), data(pData), location(pLocation) {}
    };

    enum class ReplaceActionOperator {
        Set,
        Add,
        Sub
    };

    #define SE_SETUP_REPLACE(pName) \
        virtual void run(std::string& pStr) const; \
        template<typename... Args> \
        static auto create(Args... pArgs) { \
            return std::unique_ptr< ReplaceExpression > (new pName(std::move(pArgs)...)); \
        }
    
    class ReplaceExpression {
    protected:
        bool m_generic;
        ReplaceActionOperator m_actionType;

        ReplaceExpression(std::string_view pData, std::string_view pAction) // we get a little silly around here
            : m_actionType(pAction.starts_with('=') ? ReplaceActionOperator::Set
                                : pAction.starts_with('>') ? ReplaceActionOperator::Add
                                    : ReplaceActionOperator::Sub),
            m_generic(pData == ".") {}

    public:
        virtual ~ReplaceExpression() = default;
        
        virtual void run(std::string& pStr) const = 0;
    };
    class ReplaceObjectGroup : ReplaceExpression {
    protected:
        int m_group;
        int m_mod;
    public:
        ReplaceObjectGroup(std::string_view pData, std::string_view pAction, std::string_view pMod)
            : ReplaceExpression(pData, pAction), m_group(geode::utils::numFromString<int>(pData).unwrapOrDefault())
            , m_mod(geode::utils::numFromString<int>(pMod).unwrapOrDefault()) {}

        SE_SETUP_REPLACE(ReplaceObjectGroup)
    };
    class ReplaceObjectID : ReplaceExpression {
    protected:
        int m_id;
        int m_mod;
    public:
        ReplaceObjectID(std::string_view pData, std::string_view pAction, std::string_view pMod)
            : ReplaceExpression(pData, pAction), m_id(geode::utils::numFromString<int>(pData).unwrapOrDefault())
            , m_mod(geode::utils::numFromString<int>(pMod).unwrapOrDefault()) {}

        SE_SETUP_REPLACE(ReplaceObjectID)
    };
    class ReplaceTargetGroup : ReplaceExpression {
    protected:
        int m_group;
        int m_mod;
    public:
        ReplaceTargetGroup(std::string_view pData, std::string_view pAction, std::string_view pMod)
            : ReplaceExpression(pData, pAction), m_group(geode::utils::numFromString<int>(pData).unwrapOrDefault())
            , m_mod(geode::utils::numFromString<int>(pMod).unwrapOrDefault()) {}

        SE_SETUP_REPLACE(ReplaceTargetGroup)
    };
    class ReplaceCenterGroup : ReplaceExpression {
    protected:
        int m_group;
        int m_mod;
    public:
        ReplaceCenterGroup(std::string_view pData, std::string_view pAction, std::string_view pMod)
            : ReplaceExpression(pData, pAction), m_group(geode::utils::numFromString<int>(pData).unwrapOrDefault())
            , m_mod(geode::utils::numFromString<int>(pMod).unwrapOrDefault()) {}

        SE_SETUP_REPLACE(ReplaceCenterGroup)
    };
    class ReplaceItemID : ReplaceExpression {
    protected:
        int m_item;
        int m_mod;
    public:
        ReplaceItemID(std::string_view pData, std::string_view pAction, std::string_view pMod)
            : ReplaceExpression(pData, pAction), m_item(geode::utils::numFromString<int>(pData).unwrapOrDefault())
            , m_mod(geode::utils::numFromString<int>(pMod).unwrapOrDefault()) {}

        SE_SETUP_REPLACE(ReplaceItemID)
    };
    class ReplaceObjectString : ReplaceExpression {
    protected:
        std::string m_string;
    public:
        ReplaceObjectString(std::string_view, std::string_view pAction, std::string_view pMod)
            : ReplaceExpression(".", pAction), m_string(pMod) {}

        SE_SETUP_REPLACE(ReplaceObjectString)
    };
    
    geode::Result<std::vector<std::unique_ptr<ReplaceExpression>>> parseReplace(std::string_view pStr);

    inline bool isProllyData(char pChar) {
        return std::isdigit(pChar) || pChar == '-' || pChar == '.';
    }

    class FindAndReplaceMenu final : public cocos2d::CCLayer {
    private:
        cocos2d::CCMenu* m_buttonMenu;
        geode::TextInput* m_queryBar;
        geode::TextInput* m_replaceBar;
        cocos2d::CCLabelBMFont* m_syntaxWarningLabel;
        CCMenuItemToggler* m_selectFilterToggle;
        cocos2d::extension::CCScale9Sprite* m_background;

        bool m_useQueryAsSelectFilter;

        std::string m_queryError;
        std::string m_replaceError;

        std::unique_ptr<QueryExpression> m_queryExpression;
        std::vector<std::unique_ptr<ReplaceExpression> > m_replaceExpressions;

        static constexpr float FROM_SELECTION_BUTTONS_SIZE = 0.1f;
        static constexpr float SEARCH_BAR_SIZE = 0.65f;
        static constexpr float BUTTON_MENU_SIZE = 0.25f;
        static constexpr float FIND_RESULTS_LABEL_SIZE = 0.15f;

        static constexpr float HORIZONTAL_PADDING = 1.0f;
        static constexpr float VERTICAL_PADDING = 2.5f;
        static constexpr float BAR_SPACING = 1.5f;
        static constexpr float BUTTON_SIZE = 17.5f;
        static constexpr float BACKGROUND_BUFFER = 5.0f;

        static constexpr float WIDTH = 225.0f;
        static constexpr float HEIGHT = 50.0f;

        void onQueryBarUpdated(const std::string&pStr);
        void onReplaceBarUpdated(const std::string& pStr);
        bool handleBarShortcuts(geode::TextInput* pBar, const std::string& pStr);

        void updateSyntaxWarningLabel();

        void runReplaceExpressions(cocos2d::CCArray* pObjs);

        void onFind(cocos2d::CCObject* pSender);
        void onReplace(cocos2d::CCObject* pSender);
        void onToggleSelectFilter(cocos2d::CCObject* pSender);
        void onFromSelection(cocos2d::CCObject* pSender);
        void onShowSyntaxWarnings(cocos2d::CCObject* pSender);
        void onHelp(cocos2d::CCObject* pSender);

    public:        
        static FindAndReplaceMenu* create();

        bool init();
        
        bool shouldSelectFilter() const;
        bool evaluateObject(GameObject* pObj) const;
    };
}