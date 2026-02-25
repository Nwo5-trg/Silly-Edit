#include "include.hpp"
#include <internal/utils/utils.hpp>

using namespace geode::prelude;

namespace ReplaceObjects {
    static std::unordered_map<char, std::function<std::unique_ptr<QueryExpression>(std::string_view)>> SEQueryIdentifiers {
        {'g', [] (std::string_view pData) { return QueryObjectGroup::create(pData); }},
        {'o', [] (std::string_view pData) { return QueryObjectID::create(pData); }},
        {'t', [] (std::string_view pData) { return QueryTargetGroup::create(pData); }},
        {'c', [] (std::string_view pData) { return QueryCenterGroup::create(pData); }},
        {'i', [] (std::string_view pData) { return QueryItemID::create(pData); }},
        {'b', [] (std::string_view pData) { return QueryObjectBaseColor::create(pData); }},
        {'d', [] (std::string_view pData) { return QueryObjectDetailColor::create(pData); }}
    };

    static bool isValidQueryIdentifier(char pIdentifier) {
        return SEQueryIdentifiers.find(pIdentifier) != SEQueryIdentifiers.end();
    }

    static std::unique_ptr<QueryExpression> getQueryForIdentifier(char pIdentifier, std::string_view pData) {
        return SEQueryIdentifiers[pIdentifier](pData);
    }

    static int getQueryTokenPrecedence(QueryTokenType pType) {
        switch (pType) {
            case QueryTokenType::NotOperator: return 3;
            case QueryTokenType::AndOperator: return 2;
            case QueryTokenType::OrOperator: return 1;
            default: return 0;
        }
    }

    static Result<std::vector<QueryToken>> lexQuery(std::string_view pStr) {
        std::vector<QueryToken> tokens;

        for (size_t i = 0; i < pStr.size(); i++) {
            const char c = pStr[i];

            if (std::isspace(c)) {
                continue;
            }

            switch (c) {
                case '(': {
                    tokens.emplace_back(QueryTokenType::OpenParentheses, pStr.substr(i, 1), i);
                break;}
                case ')': {
                    tokens.emplace_back(QueryTokenType::ClosingParentheses, pStr.substr(i, 1), i);
                break;}
                case '!': {
                    tokens.emplace_back(QueryTokenType::NotOperator, pStr.substr(i, 1), i);
                break; }
                case '|': [[__fallthrough__]];
                case '&': {
                    if (++i >= pStr.size()) {
                        return Err("lexing error at end : incomplete trailing '{0}{0}' operator", c);
                    }

                    const char next = pStr[i];

                    if (next == c) {
                        tokens.emplace_back(c == '|' ? QueryTokenType::OrOperator : QueryTokenType::AndOperator, pStr.substr(i - 1, 2), i - 1);
                    }
                    else {
                        return Err("lexing error at {} : invalid operator '{}{}'", i - 1, c, next);
                    }
                break; }
                case '{': {
                    if (++i >= pStr.size()) {
                        return Err("lexing error at end : trailing string declaration");
                    }

                    const auto start = i;

                    for (char next = pStr[i]; next != '}'; next = pStr[i]) {
                        if (++i >= pStr.size()) {
                            return Err("lexing error at end : unclosed string declaration");
                        }
                        if (next == '{') {
                            return Err("lexing error at {} : nested string declaration", i);
                        }
                    }

                    const auto length = i - start;

                    if (!length) {
                        return Err("lexing error at {} : empty string", start);
                    }

                    tokens.emplace_back(QueryTokenType::Data, pStr.substr(start, length), start - 1);
                break; }
                default: {
                    if (isValidQueryIdentifier(c)) {
                        tokens.emplace_back(QueryTokenType::Identifier, pStr.substr(i, 1), i);

                        continue;
                    }

                    const auto start = i;

                    while (i < pStr.size() && isProllyData(pStr[i])) {
                        i++;
                    }

                    const auto length = i - start;

                    if (!length) {
                        return Err("lexing error at {} : unknown token '{}'", i, c);
                    }

                    if (i-- >= pStr.size()) {
                        return Err("lexing error at end : unfinished data (ur prolly missing an identifier)");
                    }

                    const auto substr = pStr.substr(start, length);
                    if ( // overly complicated but fun so uwu :3
                        substr.find_first_of("0123456789") == std::string::npos 
                        || Utils::stringCount(substr, '-') > 1 
                        || Utils::stringCount(substr, '.') > 1
                        || (substr.find('-') != std::string::npos && !substr.starts_with('-'))
                    ) {
                        return Err("lexing error at {} : invalid data", start);
                    }
                    
                    tokens.emplace_back(QueryTokenType::Data, substr, start);
                break; }
            }
        }

        return Ok(std::move(tokens));
    }

    Result<std::unique_ptr<QueryExpression>> parseQuery(std::string_view pStr) {
        const auto lexingRes = lexQuery(pStr);

        if (lexingRes.isErr()) {
            return Err(lexingRes.unwrapErr());
        }

        const auto tokens = lexingRes.unwrap();

        if (tokens.empty()) {
            return Err("");
        }

        std::vector<std::unique_ptr<QueryExpression>> queries;
        std::vector<QueryToken> queuedTokens;

        #define I_STILL_CANT_REALLY_WRAP_MY_HEAD_AROUND_SHUNTING_YARD_SO_ILL_MAKE_THIS_NOT_USE_A_FUCKING_MACRO_LATER() \
            const auto queuedToken = queuedTokens.back(); \
            queuedTokens.pop_back(); \
        \
            if (queuedToken.type == QueryTokenType::NotOperator) { \
                if (queries.empty()) { \
                    return Err("parse error at {} : operator missing operands", queuedToken.location); \
                } \
        \
                auto expression = std::move(queries.back()); \
                queries.pop_back(); \
        \
                queries.push_back(QueryConditionNot::create(std::move(expression))); \
            } else { \
                if (queries.size() < 2) { \
                    return Err("parse error at {} : operator missing operands", queuedToken.location); \
                } \
        \
                auto rhs = std::move(queries.back()); \
                queries.pop_back(); \
                auto lhs = std::move(queries.back()); \
                queries.pop_back(); \
        \
                if (queuedToken.type == QueryTokenType::AndOperator) { \
                    queries.push_back(QueryConditionAnd::create(std::move(rhs), std::move(lhs))); \
                } \
                else if (queuedToken.type == QueryTokenType::OrOperator) { \
                    queries.push_back(QueryConditionOr::create(std::move(rhs), std::move(lhs))); \
                }  \
            }

        for (size_t i = 0; i < tokens.size(); i++) {
            const auto& token = tokens[i];

            switch (token.type) {
                case QueryTokenType::OpenParentheses: {
                    queuedTokens.push_back(token);
                break; }
                case QueryTokenType::ClosingParentheses: {
                    while (!queuedTokens.empty() && queuedTokens.back().type != QueryTokenType::OpenParentheses) {
                        I_STILL_CANT_REALLY_WRAP_MY_HEAD_AROUND_SHUNTING_YARD_SO_ILL_MAKE_THIS_NOT_USE_A_FUCKING_MACRO_LATER()
                    }

                    if (queuedTokens.empty()) {
                        return Err("parse error at {} : extra closing parentheses", token.location);
                    }

                    queuedTokens.pop_back();
                break; }
                case QueryTokenType::AndOperator: [[__fallthrough__]];
                case QueryTokenType::OrOperator: [[__fallthrough__]];
                case QueryTokenType::NotOperator: {
                    while (!queuedTokens.empty() && getQueryTokenPrecedence(queuedTokens.back().type) >= getQueryTokenPrecedence(token.type)) {
                        I_STILL_CANT_REALLY_WRAP_MY_HEAD_AROUND_SHUNTING_YARD_SO_ILL_MAKE_THIS_NOT_USE_A_FUCKING_MACRO_LATER()
                    }

                    queuedTokens.push_back(token);
                break; }
                case QueryTokenType::Data: {
                    if (i && tokens[i - 1].type == QueryTokenType::Data) {
                        return Err("parse error at {} : conjoined data without operator", token.location);
                    }

                    if (++i >= tokens.size()) {
                        return Err("super special data parse error that shouldnt happen", token.location);
                    }

                    const auto& next = tokens[i];

                    if (next.type != QueryTokenType::Identifier) {
                        return Err("parse error at {} : data without following identifier", token.location);
                    } 
                    
                    queries.push_back(getQueryForIdentifier(next.data.front(), token.data));
                break; }
                case QueryTokenType::Identifier: {
                    return Err("parse error at {} : double identifier", token.location);
                break; }
            }
        }

        while (!queuedTokens.empty()) {
            const auto& token = queuedTokens.back();
            
            if (token.type == QueryTokenType::OpenParentheses) {
                return Err("parse error at {} : extra open parentheses", token.location);
            }

            I_STILL_CANT_REALLY_WRAP_MY_HEAD_AROUND_SHUNTING_YARD_SO_ILL_MAKE_THIS_NOT_USE_A_FUCKING_MACRO_LATER()
        }

        return Ok(std::move(queries.back()));
    }

    bool QueryConditionAnd::evaluate(GameObject* pObj) const {
        return m_lhs->evaluate(pObj) && m_rhs->evaluate(pObj);
    }
    bool QueryConditionOr::evaluate(GameObject* pObj) const {
        return m_lhs->evaluate(pObj) || m_rhs->evaluate(pObj);
    }
    bool QueryConditionNot::evaluate(GameObject* pObj) const {
        return !m_expression->evaluate(pObj);
    }
    bool QueryObjectGroup::evaluate(GameObject* pObj) const {
        switch (m_group) {
            case -1: return !Editor::Object::hasGroups(pObj);
            case 0: return Editor::Object::hasGroups(pObj);
            default: return Editor::Object::hasGroup(pObj, m_group);
        }
    }
    bool QueryObjectID::evaluate(GameObject* pObj) const {
        return pObj->m_objectID == m_id;
    }
    bool QueryTargetGroup::evaluate(GameObject* pObj) const {
        return pObj->m_isTrigger && static_cast<EffectGameObject*>(pObj)->m_targetGroupID == m_group;
    }
    bool QueryCenterGroup::evaluate(GameObject* pObj) const {
        return pObj->m_isTrigger && static_cast<EffectGameObject*>(pObj)->m_centerGroupID == m_group;
    }
    bool QueryItemID::evaluate(GameObject* pObj) const {
        return pObj->m_isTrigger && static_cast<EffectGameObject*>(pObj)->m_itemID == m_item;
    }
    bool QueryObjectBaseColor::evaluate(GameObject* pObj) const {
        return Editor::Object::hasColor(pObj, m_color, true);
    }
    bool QueryObjectDetailColor::evaluate(GameObject* pObj) const {
        return Editor::Object::hasColor(pObj, m_color, false);
    }
}