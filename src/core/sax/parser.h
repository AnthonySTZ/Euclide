#pragma once

#include "tokenizer.h"

namespace euclide {

struct AST {};

class Parser {
  public:
    Parser() = default;

    AST parse(const std::string& t_text);

  private:
    std::string m_text;
};

} // namespace euclide