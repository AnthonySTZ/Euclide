#pragma once

#include <iostream>
#include <vector>

#include "tokenizer.h"
#include "ast.h"

namespace euclide {

class Parser {
  public:
    Parser() = default;

    /// @brief Parse a given text to an Abstract Syntax Tree
    /// @param t_text The script text to parse
    /// @return The AST constructed from the given text
    std::vector<AST> parse(const std::string& t_text);

  private:
    /// @brief Consume the next Token and check its type
    /// @param t_tokenType The next token type we expect
    /// @return The next Token
    Token consume(const TokenType t_tokenType);

    /// @brief Check if the next token is a primary type and return the AST of it.
    ///        Primary types are : literals (Number, String), Identifier (variables), function calls, parenthesis
    ///        expression
    /// @return The AST if primary found
    AST primary();

    /// @brief Check if the next token is a term type and return the AST of it.
    ///        Term types are : Multiplication and Division which have higher priority than addition and subtraction
    /// @return The AST if term found
    AST term();

    /// @brief Check if the next token is an Expression type and return the AST of it.
    ///        Expression types are : Addition and Subtraction
    /// @return The AST if expression found
    AST expression();

    /// @brief Check if the next token is an Assignment type and return the AST of it.
    ///        Assignment type is in form of `Identifier` = `expression`
    /// @return The AST if assignment found
    AST assignment();

    /// @brief Check if the next token is an Statement type and return the AST of it.
    ///        Statement type is `;`
    /// @return The AST if statement found else nullptr
    AST statement();

  private:
    Tokenizer m_tokenizer{};
    Token m_nextToken{};
};

} // namespace euclide