#pragma once

#include <cstdint>

#include "parser.h"
#include "geometry/attribute_set.h"
#include "semantic.h"

namespace euclide {

struct EvalContext {
    AttributeSet& attribs;
    uint32_t index;
};

struct AxiaEvaluator : ASTVisitor {
    EvalContext& context;

    std::unordered_map<Symbol*, Value> locals;

    explicit AxiaEvaluator(EvalContext& t_c) : context(t_c) {}

    Value visit(StringLiteral& t_node) override;
    Value visit(NumericLiteral& t_node) override;
    Value visit(VarDecl& t_node) override;
    Value visit(BinaryOp& t_node) override;
    Value visit(AttributeIdentifier& t_node) override;
    Value visit(Identifier& t_node) override;
    Value visit(Assignment& t_node) override;
    Value visit(Statement& t_node) override;
};

} // namespace euclide