#include "tokenizer.h"

namespace euclide {

const std::vector<TokenPattern> Tokenizer::PATTERNS = {
    {TokenType::Statement, std::regex("^;")},
    {TokenType::Assignment, std::regex("^=")},
    {TokenType::Type, std::regex("(^float)")},
    {TokenType::Identifier, std::regex("^[[:alpha:]]+")},
    {TokenType::LParen, std::regex("^\\(")},
    {TokenType::RParen, std::regex("^\\)")},
    {TokenType::BinaryOp, std::regex("^(\\+|-|\\*|\\/)")},
    {TokenType::Number, std::regex("^\\d+\\.?\\d*")},
    {TokenType::String, std::regex("^\"[^\"]*\"")},
    {TokenType::String, std::regex("^'[^']*'")},
};

void Tokenizer::initialize(const std::string& t_text) {
    m_text = t_text;
    m_cursor = 0;
}

Token Tokenizer::getNextToken() {
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

    std::string subtext = m_text.substr(m_cursor);
    for (const auto& [type, pattern] : PATTERNS) {
        std::regex_search(subtext, m_match, pattern);
        if (m_match.empty())
            continue;
        m_cursor += m_match[0].length();
        return {type, m_match[0].str()};
    }

    return {
        TokenType::Undefined,
        "",
    };
}

} // namespace euclide