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

        return program();
    };

  private:
    inline AST program() { return factor(); }

    inline AST factor() {
        switch (m_nextToken.type) {
        case TokenType::Number: {
            const Token token = consume(TokenType::Number);
            return std::make_unique<NumericLiteral>(std::stod(token.value));
        }

        case TokenType::String: {
            const Token token = consume(TokenType::String);
            return std::make_unique<StringLiteral>(token.value);
        }

        default:
            break;
        }

        std::runtime_error("Unexpected Literal: " + m_nextToken.value + " !");
        return AST{};
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