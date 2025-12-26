#include <gtest/gtest.h>
#include "sax/parser.h"

namespace euclide {

TEST(SAXParser, TestNumericLiteral) {
    Parser parser{};
    const std::string script = "42";

    const AST parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree.body.type, LiteralType::NumericLiteral);
    EXPECT_EQ(parsedTree.body.value, "42");
}

TEST(SAXParser, TestNumericLiteralWithWhiteSpaces) {
    Parser parser{};
    const std::string script = "    42     ";

    const AST parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree.body.type, LiteralType::NumericLiteral);
    EXPECT_EQ(parsedTree.body.value, "42");
}

TEST(SAXParser, TestStringLiteral) {
    Parser parser{};
    const std::string script = "\"Test\"";

    const AST parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree.body.type, LiteralType::StringLiteral);
    EXPECT_EQ(parsedTree.body.value, "Test");
}

TEST(SAXParser, TestStringLiteralWithWhitespaces) {
    Parser parser{};
    const std::string script = "   \"   Test\"   ";

    const AST parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree.body.type, LiteralType::StringLiteral);
    EXPECT_EQ(parsedTree.body.value, "   Test");
}

} // namespace euclide