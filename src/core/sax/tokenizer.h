#pragma once

#include <string>

namespace euclide {

enum TokenType { Number, String, Undefined };

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

        // Numbers
        if (isNum(m_text[m_cursor])) {
            const size_t start = m_cursor;
            m_cursor++;
            while (isNum(m_text[m_cursor]) && hasMoreTokens()) {
                m_cursor++;
            }
            return {
                TokenType::Number,
                m_text.substr(start, m_cursor),
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
                m_text.substr(start + 1, m_cursor - 1),
            };
        }

        return {
            TokenType::Undefined,
            "",
        };
    };

  private:
    inline bool hasMoreTokens() const noexcept { return m_cursor < m_text.length(); }
    inline bool isNum(const char t_letter) const noexcept { return t_letter >= '0' && t_letter <= '9'; }

  private:
    std::string m_text{};
    size_t m_cursor = 0;
};

} // namespace euclide