#pragma once

#include <string>
#include <regex>

namespace euclide {

enum class TokenType { Number, String, Identifier, BinaryOp, Assignment, Statement, LParen, RParen, Undefined };

struct Token {
    TokenType type = TokenType::Undefined;
    std::string value{};
};

struct TokenPattern {
    TokenType type;
    std::regex pattern;
};

class Tokenizer {
  public:
    /// @brief Initialize Tokenizer to be used again with another text
    /// @param t_text
    void initialize(const std::string& t_text);

    /// @brief Get the next available token based on regex pattern set in PATTERNS
    /// @return The next `Token` containing its type and value
    Token getNextToken();

  private:
    inline bool hasMoreTokens() const noexcept { return m_cursor < m_text.length(); }
    inline bool isWhitespace(const char t_letter) const noexcept {
        return t_letter == ' ' || t_letter == '\n' || t_letter == '\t';
    }

  private:
    std::string m_text{};
    size_t m_cursor = 0;
    std::smatch m_match;

    static const std::vector<TokenPattern> PATTERNS;
};

} // namespace euclide