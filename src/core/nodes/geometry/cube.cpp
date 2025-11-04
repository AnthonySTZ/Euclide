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
    output->addPrimitive({p0, p1, p2, p3}); // +Y
    output->addPrimitive({p7, p6, p5, p4}); // -Y

    output->addPrimitive({p4, p5, p1, p0}); // -Z
    output->addPrimitive({p3, p2, p6, p7}); // +Z

    output->addPrimitive({p0, p3, p7, p4}); // +X
    output->addPrimitive({p1, p5, p6, p2}); // -X

    return output;
}

}