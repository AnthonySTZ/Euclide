#pragma once

#include <string>

namespace euclide {

enum class TokenType { Number, String, Undefined };

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
            while (m_text[m_cursor] != '"' && hasMoreTokens()) {
                m_cursor++;
            }
            return {
                TokenType::String,
                m_text.substr(start + 1, m_cursor - start - 1),
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

  private:
    std::string m_text{};
    size_t m_cursor = 0;
};

} // namespace euclide