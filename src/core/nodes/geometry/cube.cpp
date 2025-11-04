#include "cube.h"

namespace butter {
    
std::shared_ptr<Mesh> Cube::compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>> &t_inputs)
{
    auto output = std::make_shared<Mesh>();

    py::vec3 position = getField<Float3Field>("position")->getValue();
    py::vec3 size = getField<Float3Field>("size")->getValue();

    size *= 0.5;
    /* Top Face */
    size_t p0 = output->addPoint(size[0] + position[0], size[1] + position[1], -size[2] + position[3]);
    size_t p1 = output->addPoint(-size[0] + position[0], size[1] + position[1], -size[2] + position[3]);
    size_t p2 = output->addPoint(-size[0] + position[0], size[1] + position[1], size[2] + position[3]);
    size_t p3 = output->addPoint(size[0] + position[0], size[1] + position[1], size[2] + position[3]);
    
    /* Bottom Face */
    size_t p4 = output->addPoint(size[0] + position[0], -size[1] + position[1], -size[2] + position[3]);
    size_t p5 = output->addPoint(-size[0] + position[0], -size[1] + position[1], -size[2] + position[3]);
    size_t p6 = output->addPoint(-size[0] + position[0], -size[1] + position[1], size[2] + position[3]);
    size_t p7 = output->addPoint(size[0] + position[0], -size[1] + position[1], size[2] + position[3]);

    /* Clockwise order */
    output->addPrimitive({0, 1, 2, 3}); // Top face
    output->addPrimitive({7, 6, 5, 4}); // Bottom face

    output->addPrimitive({4, 5, 1, 0});
    output->addPrimitive({3, 2, 6, 7});

    output->addPrimitive({0, 3, 7, 4});
    output->addPrimitive({1, 5, 6, 2});

    return output;
}

}