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
    Token consume(const TokenType t_tokenType);

    AST primary();
    AST term();
    AST expression();
    AST assignment();
    AST statement();

  private:
    Tokenizer m_tokenizer{};
    Token m_nextToken{};
};

} // namespace euclide