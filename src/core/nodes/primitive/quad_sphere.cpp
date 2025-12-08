#include "quad_sphere.h"

#include "nodes/primitive/cube.h"
#include "nodes/geometry/subdivide.h"

namespace euclide {

QuadSphere::QuadSphere() : Node(0, 1, "QuadSphere") {
    auto positionField = std::make_shared<Float3Field>(0.0, 0.0, 0.0);
    positionField->setMetadata(NodeFieldMetadata{
        displayName : "Position",
        step : 0.02f,
    });
    addField("position", positionField);

    auto sizeField = std::make_shared<Float3Field>(1.0, 1.0, 1.0);
    sizeField->setMetadata(NodeFieldMetadata{
        displayName : "Size",
        min : 0.0f,
        step : 0.02f,
    });
    addField("size", sizeField);

    auto divisionsField = std::make_shared<NodeField<int>>(1);
    divisionsField->setMetadata(NodeFieldMetadata{
        displayName : "Divisions",
        min : 0,
        max : 10,
        step : 1,
    });
    addField("divisions", divisionsField);
}

std::shared_ptr<Mesh> QuadSphere::compute(const size_t t_index,
                                          const std::vector<std::shared_ptr<Mesh>>& t_inputs) const {
    auto output = std::make_shared<Mesh>();

    const float3 position = getField<Float3Field>("position")->getValue();
    const float3 size = getField<Float3Field>("size")->getValue();
    const int divisions = getField<NodeField<int>>("divisions")->getValue();

    const QuadSphereSettings settings{position, size, divisions};

    createQuadSphere(*output, settings);

    return output;
}

void QuadSphere::createQuadSphere(Mesh& t_mesh, const QuadSphereSettings& t_settings) {
    Cube::createCube(t_mesh, CubeSettings{t_settings.position, t_settings.size});
    Subdivide::subdivide(t_mesh, SubdivideSettings{t_settings.divisions});
}

} // namespace euclide