#include "parser.h"

namespace euclide {

std::vector<AST> Parser::parse(const std::string& t_text) {
    m_tokenizer.initialize(t_text);
    m_nextToken = m_tokenizer.getNextToken();

    std::vector<AST> statements;
    while (AST nextStatement = statement()) { // Check if next statement is not a nullptr
        statements.push_back(std::move(nextStatement));
    }
    return statements;
}

Token Parser::consume(const TokenType t_tokenType) {
    const Token token = m_nextToken;
    if (token.type == TokenType::Undefined)
        throw std::runtime_error("Unexpected end of input !");

    if (token.type != t_tokenType)
        throw std::runtime_error("Unexpected token !");

    m_nextToken = m_tokenizer.getNextToken();
    return token;
}

AST Parser::primary() {
    switch (m_nextToken.type) {
    case TokenType::Number: {
        const Token token = consume(TokenType::Number);
        return std::make_unique<NumericLiteral>(std::stof(token.value));
    }

    case TokenType::String: {
        const Token token = consume(TokenType::String);
        return std::make_unique<StringLiteral>(token.value.substr(1, token.value.length() - 2)); // Remove quotes
    }
    case TokenType::Type: {
        const Token typeToken = consume(TokenType::Type);
        const Token identifierToken =
            consume(TokenType::Identifier); // Type should be before an identifier i.e float myvar
        return std::make_unique<VarDecl>(typeToken.value, identifierToken.value);
    }
    case TokenType::Identifier: {
        const Token token = consume(TokenType::Identifier);
        return std::make_unique<Identifier>(token.value);
    }
    case TokenType::AttributeIdentifier: {
        const Token token = consume(TokenType::AttributeIdentifier);
        const size_t length = token.value.length();
        size_t typeCursor = 0;
        while (typeCursor < length && token.value[typeCursor] != '@') {
            typeCursor++;
        }
        size_t dotCursor = typeCursor;
        while (dotCursor < length && token.value[dotCursor] != '.') {
            dotCursor++;
        }
        int component = 0;
        if (dotCursor != length - 2) {
            component = -1;
        } else {
            char c = token.value[dotCursor + 1];
            if (c == 'x')
                component = 0;
            else if (c == 'y')
                component = 1;
            else if (c == 'z')
                component = 2;
            else if (c == 'w')
                component = 3;
        }

        return std::make_unique<AttributeIdentifier>(token.value.substr(0, typeCursor),
                                                     token.value.substr(typeCursor + 1, dotCursor - typeCursor - 1),
                                                     component);
    }
    case TokenType::LParen: {
        consume(TokenType::LParen);
        AST expr = expression();
        if (m_nextToken.type != TokenType::RParen) // Check if the parenthesis is closed
            std::runtime_error("Unexpected Literal: " + m_nextToken.value + ". Expected ')'");
        consume(TokenType::RParen);
        return expr;
    }

    default:
        break;
    }

    std::runtime_error("Unexpected Literal: " + m_nextToken.value + " !");
    return nullptr;
}

AST Parser::term() {
    AST node = primary();

    if (m_nextToken.type != TokenType::BinaryOp)
        return node;

    while (m_nextToken.value == "*" || m_nextToken.value == "/") {
        NodeType opType = m_nextToken.value == "*" ? NodeType::MultOp : NodeType::DivOp;
        consume(TokenType::BinaryOp);
        node = std::make_unique<BinaryOp>(opType, std::move(node), primary());
    }

    return node;
}

AST Parser::expression() {
    AST node = term();

    if (m_nextToken.type != TokenType::BinaryOp)
        return node;

    while (m_nextToken.value == "+" || m_nextToken.value == "-") {
        NodeType opType = m_nextToken.value == "+" ? NodeType::AddOp : NodeType::SubOp;
        consume(TokenType::BinaryOp);
        node = std::make_unique<BinaryOp>(opType, std::move(node), term());
    }

    return node;
}

AST Parser::assignment() {
    AST node = expression();

    if (m_nextToken.type != TokenType::Assignment)
        return node;

    if ((node->type != NodeType::Identifier) && (node->type != NodeType::VarDecl) &&
        (node->type != NodeType::AttributeIdentifier))
        throw std::runtime_error("Unexpected expression instead of Identifier before assignment!");

    consume(TokenType::Assignment);
    return std::make_unique<Assignment>(std::move(node), expression());
}

AST Parser::statement() {
    AST node = assignment();

    if (m_nextToken.type == TokenType::Statement) {
        Token token = consume(TokenType::Statement);
        if (token.value == ";") {
            return std::make_unique<Statement>(NodeType::SemiColonStatement, std::move(node));
        } else {
            throw std::runtime_error("Statement not supported yet!");
        }
    }

    return nullptr; // Maybe return nullptr if not statement because each "lines" should be a statement.
}

} // namespace euclide