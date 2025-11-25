#include "subdivide.h"

namespace butter {
    
Subdivide::Subdivide()
    : Node(1, 1, "Subdivide")
{
    auto divisionsField = std::make_shared<NodeField<int>>(1);
    divisionsField->setMetadata(NodeFieldMetadata{
        displayName: "Divisions",
        min: 0,
        step: 1,
    });
    addField("divisions", divisionsField);
}

std::shared_ptr<Mesh> Subdivide::compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>> &t_inputs) const
{
    if (t_inputs[0] == nullptr) return std::make_shared<Mesh>();

    auto output = std::make_shared<Mesh>(*t_inputs[0]);

    const int divisions = getField<NodeField<int>>("divisions")->getValue();

    const SubdivideSettings settings{
        .divisions = divisions
    };

    subdivide(*output, settings);

    return output;
}

void Subdivide::subdivide(Mesh &t_mesh, const SubdivideSettings& t_settings)
{    
    std::vector<float, AlignedAllocator<float, 32>> facePosX;
    std::vector<float, AlignedAllocator<float, 32>> facePosY;
    std::vector<float, AlignedAllocator<float, 32>> facePosZ;

    const auto& primitives = t_mesh.primitives;
    const size_t numOfPrims = primitives.size();
    facePosX.resize(numOfPrims);
    facePosY.resize(numOfPrims);
    facePosZ.resize(numOfPrims);
    
    const auto& vertices = t_mesh.vertices;
    auto& points = t_mesh.points;
    float3 faceCenter{0.0, 0.0, 0.0};
    for(size_t i = 0; i < numOfPrims; ++i) {
        faceCenter[0] = 0.0;
        faceCenter[1] = 0.0;
        faceCenter[2] = 0.0;

        const size_t startIndex = primitives[i].verticesIndex;
        const int numVertices = primitives[i].numVertices;
        for (size_t vertIdx = startIndex; vertIdx < startIndex + numVertices; ++vertIdx){
            const size_t pointIdx = vertices[vertIdx].refPoint;
            faceCenter[0] += points.posX[pointIdx];
            faceCenter[1] += points.posY[pointIdx];
            faceCenter[2] += points.posZ[pointIdx];
        }

        const float factor = 1.0f / static_cast<float>(numVertices);

        facePosX[i] = faceCenter[0] * factor;
        facePosY[i] = faceCenter[1] * factor;
        facePosZ[i] = faceCenter[2] * factor;
    }

    {   // TODO: DEBUG ONLY
        points.reserve(numOfPrims);
        for(size_t i = 0; i < numOfPrims; ++i) {
            points.addPoint(facePosX[i], facePosY[i], facePosZ[i]);
        }
    }

    const std::vector<HalfEdge> halfEdges = t_mesh.computeHalfEdges();

}

}