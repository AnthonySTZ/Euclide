#pragma once

#include <string>

namespace euclide {

enum class TokenType { Number, String, Identifier, BinaryOp, Assignment, Statement, LParen, RParen, Undefined };

struct Token {
    TokenType type = TokenType::Undefined;
    std::string value{};
};

class Tokenizer {
  public:
    /// @brief Initialize Tokenizer to be used again with another text
    /// @param t_text
    void initialize(const std::string& t_text) {
        m_text = t_text;
        m_cursor = 0;
    }

    inline Token getNextToken() {
        if (!hasMoreTokens())
            return {
                TokenType::Undefined,
                "",
            };

        // Skip Whitespaces
        if (isWhitespace(m_text[m_cursor])) {
            m_cursor++;
            return getNextToken();
        }

        // ; statement
        if (m_text[m_cursor] == ';') {
            m_cursor++;
            return {TokenType::Statement, ";"};
        }

        // Assignment
        if (m_text[m_cursor] == '=') {
            m_cursor++;
            return {TokenType::Assignment, ""};
        }

        // Identifier
        if (isalpha(m_text[m_cursor])) {
            const size_t start = m_cursor;
            m_cursor++;
            while (isalpha(m_text[m_cursor]) && hasMoreTokens()) {
                m_cursor++;
            }
            return {
                TokenType::Identifier,
                m_text.substr(start, m_cursor - start),
            };
        }

        // Parenthesis
        if (m_text[m_cursor] == '(') {
            m_cursor++;
            return {TokenType::LParen, ""};
        }
        if (m_text[m_cursor] == ')') {
            m_cursor++;
            return {TokenType::RParen, ""};
        }

        // Binary Op
        if (isBinaryOp(m_text[m_cursor])) {
            char op = m_text[m_cursor];
            m_cursor++;
            return {
                TokenType::BinaryOp,
                std::string(1, op),
            };
        }

        // Numbers
        if (isdigit(m_text[m_cursor])) {
            const size_t start = m_cursor;
            m_cursor++;
            while (isdigit(m_text[m_cursor]) && hasMoreTokens()) {
                m_cursor++;
            }
            return {
                TokenType::Number,
                m_text.substr(start, m_cursor - start),
            };
        }

        // String
        if (m_text[m_cursor] == '"') {
            const size_t start = m_cursor;
            m_cursor++;
            while (m_text[m_cursor++] != '"' && hasMoreTokens()) {
            }
            return {
                TokenType::String,
                m_text.substr(start + 1, m_cursor - start - 2),
            };
        }

        return {
            TokenType::Undefined,
            "",
        };
    };

  private:
    inline bool hasMoreTokens() const noexcept { return m_cursor < m_text.length(); }
    inline bool isWhitespace(const char t_letter) const noexcept {
        return t_letter == ' ' || t_letter == '\n' || t_letter == '\t';
    }
    inline bool isBinaryOp(const char t_letter) const noexcept {
        return t_letter == '+' || t_letter == '-' || t_letter == '*' || t_letter == '/';
    }

  private:
    std::string m_text{};
    size_t m_cursor = 0;
};

} // namespace euclide