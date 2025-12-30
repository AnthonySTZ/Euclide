#pragma once

#include "ast.h"
#include "geometry/attribute_types.h"
#include <unordered_map>
#include <iostream>

namespace euclide {

struct LocalVar {
    std::string name;
    AttributeType type;
    Value type;
};

struct Scope {
    Scope* parent;
    std::unordered_map<std::string, LocalVar> localvars;

    LocalVar* lookup(const std::string& name) {
        if (localvars.count(name))
            return &localvars[name];
        return parent ? parent->lookup(name) : nullptr;
    }
};

struct SemanticVisitor : ASTVisitor {
    Scope* current;

    inline void enterScope() { current = new Scope{current}; }
    inline void exitScope() { current = current->parent; }
    inline LocalVar* defineLocalVar(const std::string& t_type, const std::string& t_name) {
        if (current->localvars.count(t_name))
            throw std::runtime_error("Redeclaration of " + t_name);

        return &current->localvars.emplace(t_name, LocalVar{t_name, AttributeTypeFromString(t_type), {}}).first->second;
    }

    Value visit(Identifier& t_node) override {
        LocalVar* localVar = current->lookup(t_node.name);
        if (!localVar)
            throw std::runtime_error("Undefined identifier: " + t_node.name);

        t_node.localvar = localVar;
        return {};
    }

    Value visit(Assignment& t_node) override {
        t_node.identifier->accept(*this);
        t_node.value->accept(*this);
        return {};
    };

    Value visit(VarDecl& t_node) override {
        LocalVar* localVar = defineLocalVar(t_node.type, t_node.name);
        t_node.localvar = localVar;

        return {};
    };
};

} // namespace euclide