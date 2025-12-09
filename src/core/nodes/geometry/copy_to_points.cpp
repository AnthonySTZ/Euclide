#include "copy_to_points.h"

#include "utils/timer.h"

namespace euclide {

CopyToPoints::CopyToPoints() : Node(2, 1, "CopyToPoints") {
}

std::shared_ptr<Mesh> CopyToPoints::compute(const size_t t_index,
                                            const std::vector<std::shared_ptr<Mesh>>& t_inputs) const {
    if (t_inputs[0] == nullptr && t_inputs[1] == nullptr)
        return std::make_shared<Mesh>();
    if (t_inputs[0] == nullptr)
        return std::make_shared<Mesh>(*t_inputs[1]);
    if (t_inputs[1] == nullptr)
        return std::make_shared<Mesh>(*t_inputs[0]);

    auto output = std::make_shared<Mesh>(*t_inputs[0]);
    Timer timer{"CopyToPoints"};

    copyToPoints(*output, *t_inputs[1]);

    return output;
}

void CopyToPoints::copyToPoints(Mesh& t_mesh, const Mesh& t_points) {
}

} // namespace euclide