// #include <internal/utils/utils.hpp>
// #include <internal/utils/parsing.hpp>
// #include "include.hpp"

// using namespace geode::prelude;

// namespace ReplaceObjects {
//     enum class TokenType {
//         Comma,
//         Number,
//         String,
//         RandomRange,
//         SetOperator,
//         AdditionOperator,
//         MinusOperator,
//         MultiplicationOperator,
//         DivisionOperator,
//         ModOperator,
//         VariableOperator,
//         Identifer,
//         GroupIdentifer,
//         StringIdentifer
//     };

//     using Token = Parsing::Token<TokenType>;

//     class Lexer final : public Parsing::Lexer<Parsing::LexingOutput<Token>> {
//     private:
//         virtual Parsing::LexingOutput<Token> impl() override {
//             std::vector<Token> out;

//             while (!isEnd() && errors().empty()) {
//                 skipSpace();

//                 if (is(',')) {
//                     out.emplace_back(TokenType::Comma, std::string{next()}, m_location);
//                 }
//                 else if (is('=')) {
//                     out.emplace_back(TokenType::SetOperator, std::string{next()}, m_location);
//                 }
//                 else if (is('+')) {
//                     out.emplace_back(TokenType::AdditionOperator, std::string{next()}, m_location);
//                 }
//                 else if (is('*')) {
//                     out.emplace_back(TokenType::MultiplicationOperator, std::string{next()}, m_location);
//                 }
//                 else if (is('/')) {
//                     out.emplace_back(TokenType::DivisionOperator, std::string{next()}, m_location);
//                 }
//                 else if (is('%')) {
//                     out.emplace_back(TokenType::ModOperator, std::string{next()}, m_location);
//                 }
//                 else if (is('-')) {
//                     const auto i = start();
//                     next();

//                     if (number()) {
//                         out.emplace_back(TokenType::Number, substr(i), m_location);
//                     }
//                     else {
//                         out.emplace_back(TokenType::MinusOperator, std::string{at(i)}, m_location);
//                     }
//                 }
//                 else if (number()) {
//                     const auto i = start();

//                     if (nextIf('~')) {
//                         if (number()) {
                            
//                         }
//                         else {
//                             error("unclosed random range");    
//                         }
//                     }
//                     else {
//                         out.emplace_back(TokenType::Number, substr(), m_location);
//                     }
//                 }
//                 else if (is('.')) {
//                     out.emplace_back(TokenType::VariableOperator, std::string{next()}, m_location);
//                 }
//                 else if (nextIf('{')) {
//                     start();
                    
//                     while (!isEnd() && current() != '}') {
//                         next();
//                     }

//                     if (isEnd()) {
//                         error("unclosed string declaration");
//                     }
//                     else {
//                         out.emplace_back(TokenType::String, substr(), m_location);
                        
//                         next();
//                     }
//                 }
//                 else if (is('g')) {
//                     out.emplace_back(TokenType::GroupIdentifer, std::string{next()}, m_location);
//                 }
//                 else if (is('s')) {
//                     out.emplace_back(TokenType::StringIdentifer, std::string{next()}, m_location);
//                 }
//                 else if (is('o', 't', 'c', 'i', 'b', 'd', 'x', 'y')) {
//                     out.emplace_back(TokenType::Identifer, std::string{next()}, m_location);
//                 }
//                 else {
//                     error("unknown token", m_location);
//                 }
//             }

//             return {
//                 errors(),
//                 warnings(),
//                 out
//             };
//         }
//     };

//     enum class ActionIdentifer {
//         Group,
//         String,
//         Object,
//         Target,
//         Center,
//         Item,
//         BaseColor,
//         DetailColor,
//         XPos,
//         YPos
//     };
//     static ActionIdentifer identiferFromString(const std::string& pStr) {
//         static utils::StringMap<ActionIdentifer> map{
//             {"g", ActionIdentifer::Group},
//             {"s", ActionIdentifer::String},
//             {"t", ActionIdentifer::Object},
//             {"c", ActionIdentifer::Target},
//             {"i", ActionIdentifer::Center},
//             {"b", ActionIdentifer::BaseColor},
//             {"d", ActionIdentifer::DetailColor},
//             {"x", ActionIdentifer::XPos},
//             {"y", ActionIdentifer::YPos}
//         };

//         return map[pStr];
//     }

//     class ActionVariable {
//     protected:
//         using Number = double;

//         virtual Number get(GameObject*) = 0;
//     public:
//         virtual ~ActionVariable() = default;

//         template<typename T>
//         T get(GameObject* pObj) {
//             if constexpr (std::is_integral_v<T>) {
//                 return static_cast<T>(std::round(get(pObj)));
//             }
//             else {
//                 return static_cast<T>(get(pObj));
//             }
//         }
//     };
//     class ActionVariableNumber final : ActionVariable {
//     private:
//         Number val;
        
//         virtual Number get(GameObject*) override {
//             return val;
//         }
    
//     public:
//         ActionVariableNumber(Number pVal)
//             : val(pVal) {}

//         auto create(std::string_view pStr) {
//             return std::unique_ptr<ActionVariable>(new ActionVariableNumber(utils::numFromString<Number>(pStr).unwrapOrDefault()));
//         }
//     };
//     class ActionVariableType final : ActionVariable {
//     private:
//         ActionIdentifer type;
//         std::optional<int> group;
        
//         virtual Number get(GameObject* pObj) override {
//             switch (type) {
//                 case ActionIdentifer::Group: return;
//                 case ActionIdentifer::Object: return;
//                 case ActionIdentifer::Target: return;
//                 case ActionIdentifer::Center: return;
//                 case ActionIdentifer::Item: return;
//                 case ActionIdentifer::BaseColor: return editor::object::baseColor(pObj).value_or(Number{});
//                 case ActionIdentifer::DetailColor: return editor::object::detailColor(pObj).value_or(Number{});
//                 case ActionIdentifer::XPos: return pObj->getRealPosition().x;
//                 case ActionIdentifer::YPos: return pObj->getRealPosition().y;
//                 default: return Number{};
//             }
//         }
    
//     public:
//         ActionVariableType(ActionIdentifer pType, std::optional<int> pGroup)
//             : type(pType), group(std::move(pGroup)) {}

//         auto create(ActionIdentifer pType, std::optional<int> pGroup = std::nullopt) {
//             return std::unique_ptr<ActionVariable>(new ActionVariableType(pType, std::move(pGroup)));
//         }
//     };

//     class Parser final : Parsing::Parser<Token, ActionExpression, Parser, Parsing::ParsingOutput<std::vector<std::unique_ptr<ActionExpression>>>> {
//     protected:
//         std::unique_ptr<ActionExpression> parseAction() {
//             if (is(TokenType::Identifer, TokenType::GroupIdentifer)) {
//                 auto identifier = next();
 
//                 if (!is(TokenType::SetOperator, TokenType::AdditionOperator, TokenType::MinusOperator, TokenType::MultiplicationOperator, TokenType::DivisionOperator, TokenType::ModOperator)) {
//                     return error("basic identifer not followed by operator");
//                 }

//                 auto op = next().type;

//                 if (isEnd()) {
//                     error("operator not followed by expression");
//                 }

//                 auto expresssion = callNext(__func__);
//             }
//             else if (is(TokenType::StringIdentifer)) {
//                 next();

//                 if (!is(TokenType::AdditionOperator)) {
//                     return error("string concat action not followed by addition operator");
//                 }

//                 next();

//                 if (!is(TokenType::String)) {
//                     return error("string concat action not followed by string");
//                 }
           
//                 next();
//             }
//             else if (auto res = peekNext(); res.has_value() && res.value().get().type == TokenType::GroupIdentifer) {
//                 if (!is(TokenType::Number, TokenType::VariableOperator)) {
//                     return error("invalid group specifier");
//                 }
                
//                 auto specifier = next();
//             }
//             else {
//                 return error("invalid identifer");
//             }
//         }
//         std::unique_ptr<ActionExpression> parseExpression() {
            
//         }
//         std::unique_ptr<ActionExpression> parseVariable() {
            
//         }

//         virtual Parsing::ParsingOutput<std::vector<std::unique_ptr<ActionExpression>>> impl() override {
//             if (m_tokens.empty()) {
//                 return {};
//             }

//             std::vector<std::unique_ptr<ActionExpression>> out;

//             // technically not proper recursive descent but wtv
//             while (!isEnd()) {
//                 out.push_back(std::move(callFirst()));

//                 if (!errors().empty()) {
//                     break;
//                 }

//                 if (is(TokenType::Comma)) {
//                     next();
//                 }
//                 else if (!isEnd()) {
//                     error("expected seperator");

//                     break;
//                 }
//             }

//             return {
//                 errors(),
//                 warnings(),
//                 std::move(out)
//             };
//         }

//     public:
//         Parser() {
//             SE_REGISTER_FUNCTION(parseAction)
//             SE_REGISTER_FUNCTION(parseExpression)
//             SE_REGISTER_FUNCTION(parseVariable)
//         }
//     };

//     Result<std::vector<std::unique_ptr<ActionExpression>>> parseActions(std::string_view pStr) {
//         return Err("");
//     }
// }