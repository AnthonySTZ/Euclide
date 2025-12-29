#pragma once

#include <memory>
#include <string>

namespace euclide {

struct ASTVisitor;

enum class NodeType {
    NumericLiteral,
    StringLiteral,
    Identifier,
    MultOp,
    DivOp,
    AddOp,
    SubOp,
    Assignment,
    SemiColonStatement,
    UndefinedNode
};

struct ASTNode {
    NodeType type = NodeType::UndefinedNode;
    explicit ASTNode(const NodeType t_type) : type(t_type) {}
    virtual ~ASTNode() = default;
    virtual Value accept(ASTVisitor&) const = 0;
};

using AST = std::unique_ptr<ASTNode>;

struct StringLiteral : ASTNode {
    std::string value;
    StringLiteral(const std::string t_string) : ASTNode(NodeType::StringLiteral), value(std::move(t_string)) {}
};

struct NumericLiteral : ASTNode {
    double value;
    NumericLiteral(const double t_value) : ASTNode(NodeType::NumericLiteral), value(t_value) {}

    Value accept(ASTVisitor& t_visitor) const override { return t_visitor.visit(*this); }
};

struct Identifier : ASTNode {
    std::string name;
    Identifier(const std::string t_name) : ASTNode(NodeType::Identifier), name(std::move(t_name)) {}

    Value accept(ASTVisitor& t_visitor) const override { return t_visitor.visit(*this); }
};

struct BinaryOp : ASTNode {
    AST left;
    AST right;

    BinaryOp(const NodeType t_op, AST&& t_l, AST&& t_r) : ASTNode(t_op), left(std::move(t_l)), right(std::move(t_r)) {}
};

struct Assignment : ASTNode {
    AST identifier;
    AST value;

    Assignment(AST&& t_l, AST&& t_r)
        : ASTNode(NodeType::Assignment), identifier(std::move(t_l)), value(std::move(t_r)) {}

    Value accept(ASTVisitor& t_visitor) const override { return t_visitor.visit(*this); }
};

struct Statement : ASTNode {
    AST left;

    Statement(const NodeType t_type, AST&& t_l) : ASTNode(t_type), left(std::move(t_l)) {}
};

// --------- AST Visitor -------
using Value = std::variant<float>;
struct ASTVisitor {
    virtual Value visit(const NumericLiteral&) = 0;
    virtual Value visit(const Identifier&) = 0;
    virtual Value visit(const Assignment&) = 0;
};

} // namespace euclide