#pragma once

#include "ast.h"
#include "geometry/attribute_set.h"

namespace euclide {

struct EvalContext {
    AttributeSet& attribs;
};

struct AxiaEvaluator : ASTVisitor {
    EvalContext& context;

    explicit AxiaEvaluator(EvalContext& t_c) : context(t_c) {}

    Value visit(const NumericLiteral& t_node) override { return t_node.value; }
    Value visit(const Identifier& t_node) override {
        const float* ptr = context.attribs.find(t_node.name)->component<float>(0);
        return *ptr;
    }
    Value visit(const Assignment& t_node) override {
        const Identifier* ident = dynamic_cast<const Identifier*>(t_node.identifier.get());
        float* ptr = context.attribs.findOrCreate<float, 1>(ident->name)->component<float>(0);
        *ptr = t_node.value->accept(*this);
    }
};

} // namespace euclide