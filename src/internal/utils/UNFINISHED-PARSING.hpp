// #pragma once

// // sooooo i made this for a school project and ik i can (and prolly should) 
// // do some cmake shenanigans and not js copy paste directly into the project
// // butttt i dont rly know how to do that and im lazy and js wanna get
// // replace object done :3c

// namespace Parsing {
//     struct Location final {
//         size_t line = 0;
//         size_t column = 0;

//         Location() = default;
//         Location(size_t pLine)
//             : line(pLine), column(0) {}
//         Location(size_t pLine, size_t pColumn)
//             : line(pLine), column(pColumn) {}
//     };

//     struct ErrorMessage {
//         std::string m_message;
//         Location m_location;
//     };

//     template<typename Token>
//     struct LexingOutput {
//         std::vector<ErrorMessage> m_errors;
//         std::vector<ErrorMessage> m_warnings;

//         std::vector<Token> m_output;

//         bool hasErrors() {
//             return !m_errors.empty();
//         }
//         bool hasWarnings() {
//             return !m_warnings.empty();
//         }
//     };
        
//     template<typename Output>
//     class Lexer {
//     private:
//         std::vector<ErrorMessage> m_errors;
//         std::vector<ErrorMessage> m_warnings;

//     protected:
//         std::string_view m_str;
//         Location m_location;
//         size_t m_tokenStart = 0;
//         size_t m_current = 0;

//         const auto& errors() {
//             return m_errors;
//         }
//         /// error message
//         /// @param pError message
//         void error(std::string_view pError) {
//             m_errors.emplace_back(pError, m_location);
//         }
//         /// error message
//         /// @param pError message
//         /// @param pLocation location
//         void error(std::string_view pError, Location pLocation) {
//             m_errors.emplace_back(pError, pLocation);
//         }
//         const auto& warnings() {
//             return m_warnings;
//         }
//         /// warn message
//         /// @param pError message
//         void warn(std::string_view pWarning) {
//             m_warnings.emplace_back(pWarning, m_location);
//         }
//         /// warn message
//         /// @param pError message
//         /// @param pLocation location
//         void warn(std::string_view pWarning, Location pLocation) {
//             m_warnings.emplace_back(pWarning, pLocation);
//         }

//         /// get current char
//         /// @return current char or null terminator if isEnd
//         char current() {
//             return isEnd() ? '\0' : m_str[m_current];
//         }
//         /// update token start
//         /// @returns current token index
//         size_t start() {
//             return m_tokenStart = m_current;
//         }
//         /// convert current char to str
//         /// @return str
//         std::string currentStr() {
//             return current();
//         }
//         /// m_str[pIndex]
//         char at(size_t pIndex) {
//             return m_str[pIndex];
//         }
//         /// get token start to current token
//         /// if input is "123", m_tokenStart = 0, and m_current = 2, output will be "12"
//         /// @return substr
//         std::string substr() {
//             if (m_tokenStart >= m_str.size()) {
//                 return "";
//             }

//             return std::string{m_tokenStart, m_current - m_tokenStart};
//         }
//         /// get token start to current token
//         /// @param pStart position to start at
//         /// @return substr
//         std::string substr(ssize_t pStart) {
//             if (pStart >= m_str.size()) {
//                 return "";
//             }
            
//             return std::string{pStart, m_current - pStart};
//         }
//         /// move to next char
//         /// updates column and current token accordingly
//         /// @returns current char
//         char next() {
//             m_location.column++;

//             return m_str[m_current++];
//         }
//         /// move to next char or error if current char is last
//         /// updates column and current token accordingly
//         /// @param pError error message if current char is last
//         /// @returns current char
//         char next(std::string_view pError) {
//             if (isEnd()) {
//                 error(pError);
//             }
            
//             m_location.column++;
            
//             return m_str[m_current++];
//         }
//         /// move to next char
//         /// updates column and current token accordingly
//         /// @param pLookaheadAmount how many chars to look ahead (default is 1 which is next token)
//         /// @returns current char
//         std::optional<char> peekNext(size_t pLookaheadAmount = 1) {
//             if (m_current + pLookaheadAmount >= m_str.size()) {
//                 return std::nullopt;
//             }

//             return m_str[m_current + pLookaheadAmount];
//         }
//         /// move after substr if substr starts from current
//         /// updates column and current token
//         /// @note if substr contains newline this doesnt accordingly update line so keep in mind !
//         /// @return if substr was found
//         bool nextIf(std::string_view pSubstr) {   
//             if (pSubstr.empty()) {
//                 return false;
//             }

//             if (m_current + pSubstr.size() > m_str.size()) {
//                 return false;
//             }

//             for (size_t i = 0; i < pSubstr.size(); i++) {
//                 if (m_current + i >= m_str.size() || m_str[m_current + i] != pSubstr[i]) {
//                     return false;
//                 }
//             }

//             m_current += pSubstr.size();
//             m_location.column += pSubstr.size();

//             return true;
//         }
//         /// move after substr if substr starts from current or error otherwise
//         /// updates columna and current token
//         /// @param pError error message if substr isnt found
//         /// @note if substr contains newline this doesnt accordingly update line so keep in mind !
//         /// @return if substr was found
//         bool nextIf(std::string_view pSubstr, std::string_view pError) {   
//             if (pSubstr.empty()) {
//                 error(pError);

//                 return false;
//             }

//             if (m_current + pSubstr.size() > m_str.size()) {
//                 error(pError);

//                 return false;
//             }

//             for (size_t i = 0; i < pSubstr.size(); i++) {
//                 if (m_current + i >= m_str.size() || m_str[m_current + i] != pSubstr[i]) {
//                     error(pError);
                    
//                     return false;
//                 }
//             }

//             m_current += pSubstr.size();
//             m_location.column += pSubstr.size();

//             return true;
//         }
//         /// move after char if current matches
//         /// updates column and current token
//         /// @return if char was found
//         bool nextIf(char pChar) {
//             if (isEnd() || m_str[m_current] != pChar) {
//                 return false;
//             }

//             m_current++;
//             m_location.column++;

//             return true;
//         }
//         /// move after char if current matches or error otherwise
//         /// updates columm amd current token accordingly
//         /// @param pError error message if current char doesnt match
//         /// @return if char was found
//         bool nextIf(char pChar, std::string_view pError) {
//             if (isEnd() || m_str[m_current] != pChar) {
//                 error(pError);

//                 return false;
//             }

//             m_current++;
//             m_location.column++;

//             return true;
//         }
//         /// advance a newline
//         /// updates line, column, and current token
//         /// @returns if current character (presumably newline) is *not* final
//         bool newline() {
//             m_location.line++;
//             m_location.column = 0;

//             m_current++;

//             return !isEnd();
//         }
//         /// check if current char is char
//         /// @param pChar
//         /// @returns if current char is char
//         bool is(char pChar) {
//             return current() == pChar;
//         }
//         /// check if current char is one of args
//         /// @returns if current char is one of args
//         template<typename... Args>
//         bool is(Args... pTypes) {
//             return (is(pTypes) || ...);
//         }
//         /// check if current char is the final one
//         /// @returns if current char is final
//         bool isFinal() {
//             return m_str.size() ? m_current >= m_str.size() - 1 : false;
//         }
//         /// check if current char is invalid (after final char)
//         /// @returns if current char is end
//         bool isEnd() {
//             return m_current >= m_str.size();
//         }

//         /// skip chars until one doesnt match
//         template<typename... Args>
//         void skip(Args... pTypes) {
//             while (!isEnd() && is(pTypes...)) {
//                 next();
//             }
//         }
//         /// skip whitespace (std::isblank)
//         void skipSpace() {
//             while (!isEnd() && std::isblank(current())) {
//                 next();
//             }
//         }
//         /// move after a number
//         /// updates current and column accordingly
//         /// @param pFloats whether to lex floats
//         /// @param pAllowStartWithPeriod whether to allow floats to start with a period
//         /// @note does not check for negatives
//         /// @returns if a number was found
//         bool number(bool pFloats = true, bool pAllowStartWithPeriod = true) {
//             bool period = false;

//             if (is('.')) {
//                 period = true;

//                 if (!pFloats || isFinal()) {
//                     return false;
//                 }
//             }

//             if (!std::isdigit(current())) {
//                 return false;
//             }

//             const auto start = m_current;

//             while (!isEnd() && (std::isdigit(current()) || (pFloats && is('.') && !period))) {
//                 if (next() == '.') {
//                     period = true;
//                 }
//             }

//             return true;
//         }

//         virtual Output impl() = 0;
        
//     public:
//         void reset() {
//             m_errors.clear();
//             m_warnings.clear();

//             m_str = "";
//             m_location = {};
//             m_tokenStart = 0;
//             m_current = 0;
//         }

//         Output lex(const std::string& pStr) {
//             reset();

//             m_str = pStr;

//             return impl();
//         }
//     };

//     template<typename Enum, typename Data = std::string>
//     requires std::is_scoped_enum_v<Enum>
//     struct Token {
//         Enum type;
//         Data data;
//         Location location;

//         Token(Enum pType, const Data& pData, Location pLocation)
//             : type(pType), data(pData), location(pLocation) {}

//         virtual ~Token() = default;
//     };

//     template<typename Enum, typename Data>
//     bool tokenIsType(const Token<Enum, Data>& pToken, Enum pType) {
//         return pToken.type == pType;
//     }
//     template<typename Enum, typename Data>
//     Location getTokenLocation(const Token<Enum, Data>& pToken) {
//         return pToken.location;
//     }

//     /// register a new function with higher (will be later in the decent tree) priority than the previous
//     #define SE_REGISTER_FUNCTION(pName) \
//     registerFunction( #pName , &std::remove_pointer_t<decltype(this)>:: pName , functions().size());
//     /// register a new function with the same priorty as the previous
//     #define SE_REGISTER_FUNCTION_BRANCH(pName) \
//     registerFunction( #pName , &std::remove_pointer_t<decltype(this)>:: pName , functions().empty() ? 0 : functions().back().priority);
//     /// register a new function with a set priority
//     #define SE_REGISTER_FUNCTION_PRIORITY(pName, pPriorty) \
//     registerFunction( #pName , &std::remove_pointer_t<decltype(this)>:: pName , pPriority );

//     #define SE_CALL_NEXT(pVariable) \
//     pVariable = callNext(__func__); \
//     \
//     if (! pVariable ) { \
//         return nullptr; \
//     }
//     #define SE_CALL_NEXT_BRANCH(pVariable, pName) \
//     pVariable = pName ();  \
//     \
//     if (! pVariable ) { \
//         return nullptr; \
//     }

//     template<typename Node>
//     struct ParsingOutput {
//         std::vector<ErrorMessage> m_errors;
//         std::vector<ErrorMessage> m_warnings;

//         Node m_output;

//         bool hasErrors() {
//             return !m_errors.empty();
//         }
//         bool hasWarnings() {
//             return !m_warnings.empty();
//         }
//     };

//     template<typename Token, typename Node, typename Self, typename Output = ParsingOutput<std::unique_ptr<Node>>>
//     class Parser {
//     private:
//         std::vector<ErrorMessage> m_errors;
//         std::vector<ErrorMessage> m_warnings;

//         using FuncPtr = std::unique_ptr<Node>(Self::*)();
//         struct Func {
//             std::string name;
//             FuncPtr ptr;
//             size_t priority;
//         };

//         std::unordered_map<std::string, size_t> m_funcMap;
//         std::vector<Func> m_funcVec;

//     protected:
//         std::span<const Token> m_tokens;
//         size_t m_currentToken = 0;

//         const auto& functions() {
//             return m_funcVec;
//         }
//         std::unique_ptr<Node> callFirst() {
//             if (m_funcVec.empty()) {
//                 return nullptr;
//             }


//             return std::invoke(m_funcVec.front().ptr, reinterpret_cast<Self*>(this));
//         }
//         /// call registered func with name
//         /// @param pName func to call
//         /// @returns function output or nullptr if function doesnt exist
//         std::unique_ptr<Node> call(const std::string& pName) {
//             auto it = m_funcMap.find(pName);

//             if (it == m_funcMap.end()) {
//                 return nullptr;
//             }

//             return std::invoke(m_funcVec[(*it).second].ptr, reinterpret_cast<Self*>(this));
//         };
//         /// call registered func after func with name
//         /// @param pName func name
//         /// @returns function output or nullptr if failed (function name invalid, no functions after function, multiple functions after function)
//         std::unique_ptr<Node> callNext(const std::string& pName) {
//             auto it = m_funcMap.find(pName);

//             if (it == m_funcMap.end()) {
//                 return nullptr;
//             }

//             const auto i = (*it).second;

//             if (i + 1 == m_funcVec.size()) {
//                 return nullptr;
//             }

//             if (i + 2 == m_funcVec.size() || m_funcVec[i + 1].priority != m_funcVec[i + 2].priority) {
//                 return std::invoke(m_funcVec[i + 1].ptr, reinterpret_cast<Self*>(this));
//             }

//             return nullptr;
//         };
//         void registerFunction(std::string pName, FuncPtr pPtr, size_t pPriority) {
//             m_funcVec.emplace_back(std::move(pName), pPtr, pPriority);
//             std::ranges::sort(m_funcVec, [] (const auto& pA, const auto& pB) {
//                 return pA.priority < pB.priority;
//             });

//             m_funcMap.clear();
//             // not the most efficient think but wtv
//             for (size_t i = 0; i < m_funcVec.size(); i++) {
//                 m_funcMap[m_funcVec[i].name] = i;
//             }
//         }

//         const auto& errors() {
//             return m_errors;
//         }
//         /// error message
//         /// @param pError message
//         /// @tparam Return default of return type
//         template<typename Return = std::nullptr_t>
//         Return error(std::string_view pError) {
//             m_errors.emplace_back(pError, getTokenLocation(current()));

//             return Return{};
//         }
//         /// error message
//         /// @param pError message
//         /// @param pLocation location
//         /// @tparam Return default of return type
//         template<typename Return = std::nullptr_t>
//         Return error(std::string_view pError, Location pLocation) {
//             m_errors.emplace_back(pError, pLocation);
            
//             return Return{};
//         }
//         const auto& warnings() {
//             return m_warnings;
//         }
//         /// warn message
//         /// @param pError message
//         template<typename Return = std::nullptr_t>
//         Return warn(std::string_view pWarning) {
//             m_warnings.emplace_back(pWarning, getTokenLocation(current()));

//             return Return{};
//         }
//         /// warn message
//         /// @param pError message
//         /// @param pLocation location
//         template<typename Return = std::nullptr_t>
//         Return warn(std::string_view pWarning, Location pLocation) {
//             m_warnings.emplace_back(pWarning, pLocation);

//             return Return{};
//         }
        
//         /// get previous token
//         /// @param pLookbackAmount how many tokens to look back (default is 1 which is previous token)
//         /// @returns previous token or nullopt
//         std::optional<std::reference_wrapper<const Token>> peekPrevious(size_t pLookbackAmount = 1) {
//             if (m_currentToken >= pLookbackAmount) {
//                 return std::cref(m_tokens[m_currentToken - pLookbackAmount]);
//             }
//             else {
//                 return std::nullopt;
//             }
//         }
//         /// get current token
//         /// @returns current token
//         const Token& current() {
//             return m_tokens[m_currentToken];
//         }
//         /// m_tokens[pIndex]
//         const Token& at(size_t pIndex) {
//             return m_tokens[pIndex];
//         }
//         /// move to next token
//         /// @returns current token
//         const Token& next() {
//             return m_tokens[m_currentToken++];
//         }
//         /// peek at next token
//         /// @param pLookaheadAmount how many tokens to look ahead (default is 1 which is next token)
//         /// @returns next token
//         std::optional<std::reference_wrapper<const Token>> peekNext(size_t pLookaheadAmount = 1) {
//             if (m_currentToken + pLookaheadAmount < m_tokens.size()) {
//                 return std::cref(m_tokens[m_currentToken + pLookaheadAmount]);
//             }
//             else {
//                 return std::nullopt;
//             }
//         }

//         /// check if current token is the final one
//         /// @returns if current token is final
//         bool isFinal() {
//             return m_tokens.size() ? m_currentToken >= m_tokens.size() - 1 : false;
//         }
//         /// check if current token is invalid (after final token)
//         /// @returns if current token is end
//         bool isEnd() {
//             return m_currentToken >= m_tokens.size();
//         }
//         /// return if token is of a type(s)
//         /// @returns if token is of a type
//         template<typename... Args>
//         bool is(Args... pTypes) {
//             return !isEnd() && (tokenIsType(current(), pTypes) || ...);
//         }

//         /// skip tokens until a token type matches one of Args (useful for error handling)
//         /// @returns breaking token
//         template<typename... Args>
//         const Token& resolve(Args... pTypes) {
//             next();

//             while (!is(pTypes...)) {
//                 next();
//             }

//             return current();
//         }

//         template<typename Callback, typename... Args>
//         std::unique_ptr<Node> binaryOperator(const std::string& pFunctionName, std::string_view pNextError, const Callback& pCallback, Args... pTypes) {
//             auto out = callNext(pFunctionName);
 
//             if (!out) {
//                 return nullptr;
//             }

//             while(is(pTypes...)) {
//                 auto op = next();

//                 if (isEnd()) {
//                     return error(pNextError);
//                 }

//                 auto rhs = callNext(pFunctionName);
                
//                 if (!rhs) {
//                     return nullptr;
//                 }

//                 out = pCallback(std::move(out), op, std::move(rhs));
//             }

//             return out;
//         }
//         template<typename Callback, typename... Args>
//         std::unique_ptr<Node> urnaryOperator(const std::string& pFunctionName, const Callback& pCallback, Args... pTypes) {
//             if (!isEnd() && is(pTypes...)) {
//                 auto op = next();

//                 auto child = call(pFunctionName);
                
//                 if (!child) {
//                     return nullptr;
//                 }

//                 return pCallback(op, std::move(child));
//             }
            
//             return callNext(pFunctionName);
//         }

//         void reset() {
//             m_errors.clear();
//             m_warnings.clear();

//             m_tokens = {};
//             m_currentToken = 0;
//         }
        
//         virtual Output impl() = 0;
    
//     public:
//         Output parse(const std::vector<Token>& pTokens) {
//             reset();
            
//             m_tokens = pTokens;

//             return impl();
//         }
//     };
// }