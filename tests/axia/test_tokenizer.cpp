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

TEST(AxiaTokenizer, TestParenthesis) {
    Tokenizer tokenizer;
    tokenizer.initialize("()");
    expectToken(tokenizer.getNextToken(), TokenType::LParen, "(");
    expectToken(tokenizer.getNextToken(), TokenType::RParen, ")");
}

TEST(AxiaTokenizer, TestIdentifier) {
    Tokenizer tokenizer;
    tokenizer.initialize("myvar");
    expectToken(tokenizer.getNextToken(), TokenType::Identifier, "myvar");
}

TEST(AxiaTokenizer, TestAttributeIdentifier) {
    Tokenizer tokenizer;
    tokenizer.initialize("f@myvar v@test @P");
    expectToken(tokenizer.getNextToken(), TokenType::AttributeIdentifier, "f@myvar");
    expectToken(tokenizer.getNextToken(), TokenType::AttributeIdentifier, "v@test");
    expectToken(tokenizer.getNextToken(), TokenType::AttributeIdentifier, "@P");
}

TEST(AxiaTokenizer, TestAttributeIdentifierWithComponent) {
    Tokenizer tokenizer;
    tokenizer.initialize("f@myvar.x v@test.x @P.y");
    expectToken(tokenizer.getNextToken(), TokenType::AttributeIdentifier, "f@myvar.x");
    expectToken(tokenizer.getNextToken(), TokenType::AttributeIdentifier, "v@test.x");
    expectToken(tokenizer.getNextToken(), TokenType::AttributeIdentifier, "@P.y");
}

TEST(AxiaTokenizer, TestAssignment) {
    Tokenizer tokenizer;
    tokenizer.initialize("=");
    expectToken(tokenizer.getNextToken(), TokenType::Assignment, "=");
}

TEST(AxiaTokenizer, TestStatement) {
    Tokenizer tokenizer;
    tokenizer.initialize(";");
    expectToken(tokenizer.getNextToken(), TokenType::Statement, ";");
}

TEST(AxiaTokenizer, TestType) {
    Tokenizer tokenizer;
    tokenizer.initialize("float myvar");
    expectToken(tokenizer.getNextToken(), TokenType::Type, "float");
    expectToken(tokenizer.getNextToken(), TokenType::Identifier, "myvar");
}

TEST(AxiaTokenizer, TestComplex) {
    Tokenizer tokenizer;
    tokenizer.initialize("myvar = 2 + 5 * ( 3 + 2);");
    expectToken(tokenizer.getNextToken(), TokenType::Identifier, "myvar");
    expectToken(tokenizer.getNextToken(), TokenType::Assignment, "=");
    expectToken(tokenizer.getNextToken(), TokenType::Number, "2");
    expectToken(tokenizer.getNextToken(), TokenType::BinaryOp, "+");
    expectToken(tokenizer.getNextToken(), TokenType::Number, "5");
    expectToken(tokenizer.getNextToken(), TokenType::BinaryOp, "*");
    expectToken(tokenizer.getNextToken(), TokenType::LParen, "(");
    expectToken(tokenizer.getNextToken(), TokenType::Number, "3");
    expectToken(tokenizer.getNextToken(), TokenType::BinaryOp, "+");
    expectToken(tokenizer.getNextToken(), TokenType::Number, "2");
    expectToken(tokenizer.getNextToken(), TokenType::RParen, ")");
    expectToken(tokenizer.getNextToken(), TokenType::Statement, ";");
}

} // namespace euclide