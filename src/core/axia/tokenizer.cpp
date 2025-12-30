#include "tokenizer.h"

namespace euclide {

const std::vector<TokenPattern> Tokenizer::PATTERNS = {
    {TokenType::Statement, std::regex("^;")},
    {TokenType::Assignment, std::regex("^=")},
    {TokenType::Identifier, std::regex("^[[:alpha:]]+")},
    {TokenType::LParen, std::regex("^\\(")},
    {TokenType::RParen, std::regex("^\\)")},
    {TokenType::BinaryOp, std::regex("^(\\+|-|\\*|\\/)")},
    {TokenType::Number, std::regex("^\\d+\\.?\\d*")},
    {TokenType::String, std::regex("^\"[^\"]*\"")},
    {TokenType::String, std::regex("^'[^']*'")},
};

}