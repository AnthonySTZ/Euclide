#include <gtest/gtest.h>
#include "sax/parser.h"

namespace euclide {

TEST(SAXParser, TestNumericLiteral) {
    Parser parser{};
    const std::string script = "42";

    const AST parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree->type, NodeType::NumericLiteral);
    EXPECT_EQ(dynamic_cast<const NumericLiteral*>(parsedTree.get())->value, 42);
}

TEST(SAXParser, TestNumericLiteralWithWhiteSpaces) {
    Parser parser{};
    const std::string script = "    42     ";

    const AST parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree->type, NodeType::NumericLiteral);
    EXPECT_EQ(dynamic_cast<const NumericLiteral*>(parsedTree.get())->value, 42);
}

TEST(SAXParser, TestStringLiteral) {
    Parser parser{};
    const std::string script = "\"Test\"";

    const AST parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree->type, NodeType::StringLiteral);
    EXPECT_EQ(dynamic_cast<const StringLiteral*>(parsedTree.get())->value, "Test");
}

TEST(SAXParser, TestStringLiteralWithWhitespaces) {
    Parser parser{};
    const std::string script = "   \"   Test\"   ";

    const AST parsedTree = parser.parse(script);
    EXPECT_EQ(parsedTree->type, NodeType::StringLiteral);
    EXPECT_EQ(dynamic_cast<const StringLiteral*>(parsedTree.get())->value, "   Test");
}

} // namespace euclide