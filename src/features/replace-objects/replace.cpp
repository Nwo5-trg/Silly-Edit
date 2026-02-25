#include "include.hpp"
#include <internal/utils/utils.hpp>

using namespace geode::prelude;

namespace ReplaceObjects {
    static std::unordered_map<char, std::function<std::unique_ptr<ReplaceExpression>(std::string_view, std::string_view, std::string_view)>> SEReplaceIdentifiers {
        {'g', [] (std::string_view pData, std::string_view pAction, std::string_view pMod) { return ReplaceObjectGroup::create(pData, pAction, pMod); }},
        {'o', [] (std::string_view pData, std::string_view pAction, std::string_view pMod) { return ReplaceObjectID::create(pData, pAction, pMod); }},
        {'t', [] (std::string_view pData, std::string_view pAction, std::string_view pMod) { return ReplaceTargetGroup::create(pData, pAction, pMod); }},
        {'c', [] (std::string_view pData, std::string_view pAction, std::string_view pMod) { return ReplaceCenterGroup::create(pData, pAction, pMod); }},
        {'i', [] (std::string_view pData, std::string_view pAction, std::string_view pMod) { return ReplaceItemID::create(pData, pAction, pMod); }},
        {'s', [] (std::string_view pData, std::string_view pAction, std::string_view pMod) { return ReplaceObjectString::create(pData, pAction, pMod); }},
    };

    static bool isValidReplaceIdentifier(char pIdentifier) {
        return SEReplaceIdentifiers.find(pIdentifier) != SEReplaceIdentifiers.end();
    }

    static std::unique_ptr<ReplaceExpression> getReplaceForIdentifier(char pIdentifier, std::string_view pData, std::string_view pAction, std::string_view pMod) {
        return SEReplaceIdentifiers[pIdentifier](pData, pAction, pMod);
    }

    static bool isProbablierObjectStringSegment(std::string_view pStr) {
        if (pStr.find_first_of("1234567890") != 0) {
            return false;
        }

        if (pStr.ends_with(',') || pStr.ends_with('.')) {
            return false;
        }

        if (const auto count = Utils::stringCount(pStr, ','); !(count % 2)) {
            return false;
        }

        if (pStr.contains(';')) {
            return false;
        }

        return true;
    }

    static size_t getObjectStringKey(std::string_view pStr, int pKey, bool pOffset) {
        const auto strKey = utils::numToString(pKey);

        for (size_t i = 0; i < pStr.size();) {
            const auto find = pStr.find(',', i);

            if (pStr.substr(i, find - i) == strKey) {
                return pOffset ? find + 1 : i;
            }

            i = pStr.find(',', find + 1) + 1;
        }

        return std::string::npos;
    }

    static Result<std::vector<ReplaceToken>> lexReplace(std::string_view pStr) {
        std::vector<ReplaceToken> tokens;

        for (size_t i = 0; i < pStr.size(); i++) {
            const char c = pStr[i];

            if (std::isspace(c)) {
                continue;
            }

            switch (c) {
                case ',': {
                    tokens.emplace_back(ReplaceTokenType::Seperator, pStr.substr(i, 1), i);
                break; }
                case '>': [[__fallthrough__]];
                case '<': [[__fallthrough__]];
                case '=': {
                    if (++i >= pStr.size() || pStr[i] != '>') {
                        return Err("lexing error at {} : unknown token '{}{}'", i - 1, c, pStr[i]);
                    }
                    
                    tokens.emplace_back(ReplaceTokenType::ActionOperator, pStr.substr(i - 1, 2), i - 1);
                break; }
                // pretty much exact copy of query parser but if it works it works
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

                    tokens.emplace_back(ReplaceTokenType::Data, pStr.substr(start, length), start - 1);
                break; }
                default: {
                    if (isValidReplaceIdentifier(c)) {
                        tokens.emplace_back(ReplaceTokenType::Identifier, pStr.substr(i, 1), i);

                        continue;
                    }

                    const auto start = i;

                    while (i < pStr.size() && isProllyData(pStr[i])) {
                        i++;
                    }

                    const auto length = i-- - start;

                    if (!length) {
                        return Err("lexing error at {} : unknown token '{}'", start, c);
                    }

                    const auto substr = pStr.substr(start, length);
                    if (
                        substr.find_first_of("0123456789") == std::string::npos && !Utils::stringCount(substr, '.')
                        || Utils::stringCount(substr, '-') > 1 
                        || (substr.find('-') != std::string::npos && !substr.starts_with('-'))
                    ) {
                        return Err("lexing error at {} : invalid data", start);
                    }
                    
                    tokens.emplace_back(ReplaceTokenType::Data, substr, start);
                break; }
            }
        }

        return Ok(std::move(tokens));
    }

    Result<std::vector<std::unique_ptr<ReplaceExpression>>> parseReplace(std::string_view pStr) {
        const auto lexingRes = lexReplace(pStr);

        if (lexingRes.isErr()) {
            return Err(lexingRes.unwrapErr());
        }

        const auto tokens = lexingRes.unwrap();

        if (pStr.empty()) {
            return Err("");
        }

        std::vector<std::unique_ptr<ReplaceExpression>> expressions;
        std::vector<ReplaceToken> queue;

        for (size_t i = 0; i < tokens.size(); i++) {
            const auto& token = tokens[i];
            const auto lastToken = i ? queue.back().type : ReplaceTokenType{};

            switch (token.type) {
                case ReplaceTokenType::ActionOperator: {
                    if (i && lastToken != ReplaceTokenType::Identifier) {
                        return Err("parse error at {} : action operator not preceeded by identifier", token.location);
                    }

                    queue.push_back(token);
                break; }
                case ReplaceTokenType::Seperator: {
                    if (queue.size() > 4) {
                        return Err("parse error at {} : missing comma", queue[4].location);
                    }
                    else if (queue.size() < 4) {
                        return Err("parse error at {} : incomplete replace action before comma", token.location);
                    }
                    else {
                        expressions.push_back(std::move(getReplaceForIdentifier(queue[1].data.front(), queue[0].data, queue[2].data, queue[3].data)));
                    }
                break; }
                case ReplaceTokenType::Data: {
                    if (i && lastToken != ReplaceTokenType::ActionOperator) {
                        return Err("parse error at {} : data not preceeded by action operator or starting replace action", token.location);
                    }

                    queue.push_back(token);
                break; }
                case ReplaceTokenType::Identifier: {
                    if (i && lastToken != ReplaceTokenType::Data) {
                        return Err("parse error at {} : identifier not preceeded by data", token.location);
                    }

                    queue.push_back(token);
                break; }
            }
        }

        if (queue.size() > 4) {
            return Err("parse error at {} : missing comma", queue[4].location);
        }
        else if (queue.size() < 4) {
            return Err("parse error at end : incomplete replace action");
        }
        else {
            expressions.push_back(std::move(getReplaceForIdentifier(queue[1].data.front(), queue[0].data, queue[2].data, queue[3].data)));
        }

        return Ok(std::move(expressions));
    }

    void ReplaceObjectGroup::run(std::string& pStr) const {
        const auto i = getObjectStringKey(pStr, 57, true);

        if (i == std::string::npos) {
            return;
        }

        std::unordered_set<int> groups;
        std::unordered_set<int> parentGroups;

        for (const auto& group : string::split(pStr.substr(i, pStr.find(',', i)), ".")) {
            groups.insert(utils::numFromString<int>(group).unwrapOrDefault());
        }

        if (groups.empty() || (!m_generic && !groups.contains(m_group))) {
            return;
        }

        pStr.erase(i, pStr.find(',', i) - i);

        if (const auto j = getObjectStringKey(pStr, 274, true); j != std::string::npos) {
            for (const auto& group : string::split(pStr.substr(j, pStr.find(',', j)), ".")) {
                const auto num = utils::numFromString<int>(group).unwrapOrDefault();
                groups.erase(num);
                parentGroups.insert(num);
            }
        }

        for (const auto group : m_generic ? std::vector<int>{groups.begin(), groups.end()} : std::vector<int>{m_group}) {
            groups.erase(group);

            switch (m_actionType) {
                case ReplaceActionOperator::Set: {
                    groups.insert(m_mod);
                break; }
                case ReplaceActionOperator::Add: {
                    groups.insert(group + m_mod);
                break; }
                case ReplaceActionOperator::Sub: {
                    groups.insert(group - m_mod);
                break; }
            }
        }

        std::string newGroupString = ",";

        for (const auto group : groups) {
            newGroupString.append(fmt::format("{}.", group));
        }
        for (const auto group : parentGroups) {
            newGroupString.append(fmt::format("{}.", group));
        }

        if (newGroupString.size() > 1) {
            newGroupString.pop_back();
        }

        pStr.replace(i, 1, newGroupString);
    }
    #define SE_SETUP_BASIC_REPLACE_ACTION(pGroup, pID) \
        const auto i = getObjectStringKey(pStr, pID , true); \
\
        if (i == std::string::npos) { \
            return; \
        } \
\
        const auto length = pStr.find(',', i) - i; \
        const auto val = utils::numFromString<int>(pStr.substr(i, length)).unwrapOrDefault(); \
\
        if (!m_generic && val != pGroup ) { \
            return; \
        } \
\
        switch (m_actionType) { \
            case ReplaceActionOperator::Set: { \
                pStr.replace(i, length, utils::numToString(m_mod)); \
            break; } \
            case ReplaceActionOperator::Add: { \
                pStr.replace(i, length, utils::numToString(val + m_mod)); \
            break; } \
            case ReplaceActionOperator::Sub: { \
                pStr.replace(i, length, utils::numToString(val - m_mod)); \
            break; } \
        };
    void ReplaceObjectID::run(std::string& pStr) const {
        SE_SETUP_BASIC_REPLACE_ACTION(m_id, 1)
    }
    void ReplaceTargetGroup::run(std::string& pStr) const {
        SE_SETUP_BASIC_REPLACE_ACTION(m_group, 51)
    }
    void ReplaceCenterGroup::run(std::string& pStr) const {
        SE_SETUP_BASIC_REPLACE_ACTION(m_group, 71)
    }
    void ReplaceItemID::run(std::string& pStr) const {
        SE_SETUP_BASIC_REPLACE_ACTION(m_item, 80)
    }
    void ReplaceObjectString::run(std::string& pStr) const {
        if (!isProbablierObjectStringSegment(m_string)) {
            return;
        };

        switch (m_actionType) {
            case ReplaceActionOperator::Set: {
                pStr = m_string;
            break; }
            case ReplaceActionOperator::Add: {
                pStr.push_back(',');
                pStr.append(m_string);
            break; }
            case ReplaceActionOperator::Sub: {
                const auto i = pStr.find(fmt::format(",{}", m_string));
                
                if (i != std::string::npos) {
                    pStr.replace(i, m_string.size(), "");
                }
            break; }
        } 
    }
}