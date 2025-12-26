#pragma once

#include <iostream>

#include "tokenizer.h"

namespace euclide {

enum LiteralType { NumericLiteral, StringLiteral, UndefinedLiteral };

struct Literal {
    LiteralType type;
    std::string value;
};

/// @brief Abstract Syntax Tree defining a Program
struct AST {
    Literal body;
};

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
    inline AST program() { return AST{literal()}; }

    inline Literal literal() {
        switch (m_nextToken.type) {
        case TokenType::Number: {
            const Token token = consume(TokenType::Number);
            return Literal{LiteralType::NumericLiteral, token.value};
        }

        case TokenType::String: {
            const Token token = consume(TokenType::String);
            return Literal{LiteralType::StringLiteral, token.value};
        }

        default:
            break;
        }

        std::runtime_error("Unexpected Literal: " + m_nextToken.value + " !");
        return {LiteralType::UndefinedLiteral, ""};
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