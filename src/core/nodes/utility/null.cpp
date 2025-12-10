#include "null.h"

namespace euclide {

Null::Null() : Node(1, 1, "Null") {
}

std::shared_ptr<Mesh> Null::compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) const {
    if (t_inputs[0] == nullptr)
        return std::make_shared<Mesh>();

    auto output = std::make_shared<Mesh>(*t_inputs[0]);

    return output;
}

} // namespace euclide