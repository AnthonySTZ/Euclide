#include "semantic.h"

namespace euclide {

Symbol* AxiaSemantic::defineSymbol(const std::string& t_type, const std::string& t_name) {
    if (current->symbols.count(t_name))
        throw std::runtime_error("Redeclaration of " + t_name);

    return &current->symbols.emplace(t_name, Symbol{t_name, AttributeTypeFromString(t_type)}).first->second;
}

Value AxiaSemantic::visit(Identifier& t_node) {
    Symbol* symbol = current->lookup(t_node.name);
    if (!symbol)
        throw std::runtime_error("Undefined identifier: " + t_node.name);

    t_node.symbol = symbol;
    return {};
}

} // namespace euclide