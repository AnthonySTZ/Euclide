#pragma once

#include <iostream>

#include "tokenizer.h"
#include "ast.h"

namespace euclide {

class Parser {
  public:
    Parser() = default;

    /// @brief Parse a given text to an Abstract Syntax Tree
    /// @param t_text The script text to parse
    /// @return The AST constructed from the given text
    inline AST parse(const std::string& t_text) {
        m_tokenizer.initialize(t_text);
        m_nextToken = m_tokenizer.getNextToken();

        return expression();
    };

  private:
    inline AST primary() {
        switch (m_nextToken.type) {
        case TokenType::Number: {
            const Token token = consume(TokenType::Number);
            return std::make_unique<NumericLiteral>(std::stod(token.value));
        }

        case TokenType::String: {
            const Token token = consume(TokenType::String);
            return std::make_unique<StringLiteral>(token.value);
        }

        case TokenType::Identifier: {
            const Token token = consume(TokenType::Identifier);
            return std::make_unique<Identifier>(token.value);
        }

        default:
            break;
        }

        std::runtime_error("Unexpected Literal: " + m_nextToken.value + " !");
        return AST{};
    }

    inline AST term() {
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

    inline AST expression() {
        AST node = term(); // 2 + 3 * 5

        if (m_nextToken.type != TokenType::BinaryOp)
            return node;

        while (m_nextToken.value == "+" || m_nextToken.value == "-") {
            NodeType opType = m_nextToken.value == "+" ? NodeType::AddOp : NodeType::SubOp;
            consume(TokenType::BinaryOp);
            node = std::make_unique<BinaryOp>(opType, std::move(node), term());
        }

        return node;
    }

    inline Token consume(const TokenType t_tokenType) {
        const Token token = m_nextToken;
        if (token.type == TokenType::Undefined)
            throw std::runtime_error("Unexpected end of input !");

        if (token.type != t_tokenType)
            throw std::runtime_error("Unexpected token !");

        m_nextToken = m_tokenizer.getNextToken();
        return token;
    }

  private:
    Tokenizer m_tokenizer{};
    Token m_nextToken{};
};

} // namespace euclide