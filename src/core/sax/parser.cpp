#include "parser.h"

namespace euclide {

AST Parser::parse(const std::string& t_text) {
    m_text = t_text;

    return AST();
}

} // namespace euclide