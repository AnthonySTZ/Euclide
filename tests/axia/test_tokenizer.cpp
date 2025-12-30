#include <gtest/gtest.h>
#include "axia/tokenizer.h"

namespace euclide {

void expectToken(const Token& t_token, TokenType t_type, std::string t_value) {
    EXPECT_EQ(t_token.type, t_type);
    EXPECT_EQ(t_token.value, t_value);
}

TEST(AxiaTokenizer, TestNumber) {
    Tokenizer tokenizer;
    tokenizer.initialize("42");
    expectToken(tokenizer.getNextToken(), TokenType::Number, "42");
}

TEST(AxiaTokenizer, TestString) {
    Tokenizer tokenizer;
    tokenizer.initialize(" \"Test\" ");
    expectToken(tokenizer.getNextToken(), TokenType::String, "\"Test\"");
}

TEST(AxiaTokenizer, TestStringSingleQuote) {
    Tokenizer tokenizer;
    tokenizer.initialize(" 'Test' ");
    expectToken(tokenizer.getNextToken(), TokenType::String, "'Test'");
}

TEST(AxiaTokenizer, TestOp) {
    Tokenizer tokenizer;
    tokenizer.initialize("+-*/");
    expectToken(tokenizer.getNextToken(), TokenType::BinaryOp, "+");
    expectToken(tokenizer.getNextToken(), TokenType::BinaryOp, "-");
    expectToken(tokenizer.getNextToken(), TokenType::BinaryOp, "*");
    expectToken(tokenizer.getNextToken(), TokenType::BinaryOp, "/");
}

} // namespace euclide