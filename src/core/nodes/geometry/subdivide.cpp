#include "subdivide.h"

#include <iostream>

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

    auto& points = t_mesh.points;
    auto& primitives = t_mesh.primitives;
    auto& vertices = t_mesh.vertices;

    size_t totalFace = 0;
    for (const auto& prim: primitives) {
        if (prim.numVertices > 2) totalFace++;
    }

    facePosX.resize(totalFace);
    facePosY.resize(totalFace);
    facePosZ.resize(totalFace);

    const std::vector<HalfEdge> halfEdges = t_mesh.computeHalfEdges();
    size_t faceIdx = 0;
    for (size_t i = 0; i < halfEdges.size(); ++i) {
        const size_t primId = halfEdges[i].face;
        const auto& prim = primitives[primId];
        const size_t startIndex = prim.verticesIndex;
        const size_t numVertices = prim.numVertices;

        float3 faceCenter{0.0, 0.0, 0.0};
        for (size_t vertIdx = startIndex; vertIdx < startIndex + numVertices; ++vertIdx){
            const size_t pointIdx = vertices[vertIdx].refPoint;
            faceCenter[0] += points.posX[pointIdx];
            faceCenter[1] += points.posY[pointIdx];
            faceCenter[2] += points.posZ[pointIdx];
        }

        const float factor = 1.0f / static_cast<float>(numVertices);

        facePosX[faceIdx] = faceCenter[0] * factor;
        facePosY[faceIdx] = faceCenter[1] * factor;
        facePosZ[faceIdx] = faceCenter[2] * factor;
        faceIdx++;

        i += numVertices - 1;
    }

    {   // TODO: DEBUG ONLY
        points.reserve(facePosX.size());
        for(size_t i = 0; i < facePosX.size(); ++i) {
            points.addPoint(facePosX[i], facePosY[i], facePosZ[i]);
        }
    }

}

}