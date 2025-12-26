#pragma once

#include <memory>
#include <string>

namespace euclide {

enum class NodeType { NumericLiteral, StringLiteral, Identifier, MultOp, DivOp, AddOp, SubOp, UndefinedNode };

struct ASTNode {
    NodeType type = NodeType::UndefinedNode;
    explicit ASTNode(const NodeType t_type) : type(t_type) {}
    virtual ~ASTNode() = default;
};

using AST = std::unique_ptr<ASTNode>;

struct StringLiteral : ASTNode {
    std::string value;
    StringLiteral(const std::string t_string) : ASTNode(NodeType::StringLiteral), value(std::move(t_string)) {}
};

struct NumericLiteral : ASTNode {
    double value;
    NumericLiteral(const double t_value) : ASTNode(NodeType::NumericLiteral), value(t_value) {}
};

struct Identifier : ASTNode {
    std::string name;
    Identifier(const std::string t_name) : ASTNode(NodeType::Identifier), name(std::move(t_name)) {}
};

struct BinaryOp : ASTNode {
    AST left;
    AST right;

    BinaryOp(const NodeType t_op, AST&& t_l, AST&& t_r) : ASTNode(t_op), left(std::move(t_l)), right(std::move(t_r)) {}
};

} // namespace euclide