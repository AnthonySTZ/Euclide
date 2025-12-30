#pragma once

#include <cstdint>

#include "parser.h"
#include "geometry/attribute_set.h"

namespace euclide {

struct EvalContext {
    AttributeSet& attribs;
    uint32_t index;
};

struct AxiaEvaluator : ASTVisitor {
    EvalContext& context;

    explicit AxiaEvaluator(EvalContext& t_c) : context(t_c) {}

    Value visit(StringLiteral& t_node) override { return t_node.value; }
    Value visit(NumericLiteral& t_node) override { return t_node.value; }
    Value visit(VarDecl& t_node) override { return t_node.name; }
    Value visit(BinaryOp& t_node) override { // TODO:
        const float lhs = std::get<float>(t_node.left->accept(*this));
        const float rhs = std::get<float>(t_node.right->accept(*this));
        switch (t_node.type) {
        case NodeType::AddOp:
            return lhs + rhs;
        case NodeType::SubOp:
            return lhs - rhs;
        case NodeType::MultOp:
            return lhs * rhs;
        case NodeType::DivOp:
            return lhs / rhs;
        default:
            break;
        }
        return 0.0f;
    }
    Value visit(Identifier& t_node) override {
        const float* ptr = context.attribs.find(t_node.name)->component<float>(0);
        return ptr[context.index];
    }
    Value visit(Assignment& t_node) override {
        const Identifier* ident = dynamic_cast<const Identifier*>(t_node.identifier.get());
        float* ptr = context.attribs.findOrCreate<float, 1>(ident->name)->component<float>(0);
        Value value = t_node.value->accept(*this);
        ptr[context.index] = std::get<float>(value);
        return value;
    }
    Value visit(Statement& t_node) override { return t_node.right->accept(*this); }
};

} // namespace euclide