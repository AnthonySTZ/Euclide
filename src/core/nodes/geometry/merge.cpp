#include "merge.h"

#include "utils/timer.h"

namespace butter {

Merge::Merge() : Node(2, 1, "Merge") {
}

std::shared_ptr<Mesh> Merge::compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) const {
    if (t_inputs[0] == nullptr && t_inputs[1] == nullptr)
        return std::make_shared<Mesh>();
    if (t_inputs[0] == nullptr)
        return std::make_shared<Mesh>(*t_inputs[1]);
    if (t_inputs[1] == nullptr)
        return std::make_shared<Mesh>(*t_inputs[0]);

    auto output = std::make_shared<Mesh>(*t_inputs[0]);
    Timer timer{"Merge"};

    merge(*output, *t_inputs[1]);

    return output;
}

void Merge::merge(Mesh& t_mesh, const Mesh& t_mesh_2) {
    auto& outputPoints = t_mesh.points;
    const size_t numPoints = outputPoints.size();
    size_t pointIdx = numPoints;

    const auto& pointsToMerge = t_mesh_2.points;
    outputPoints.reserve(outputPoints.size() + pointsToMerge.size());
    outputPoints.resize(outputPoints.size() + pointsToMerge.size());
    for (size_t i = 0; i < pointsToMerge.size(); ++i) {
        outputPoints.posX[pointIdx] = pointsToMerge.posX[i];
        outputPoints.posY[pointIdx] = pointsToMerge.posY[i];
        outputPoints.posZ[pointIdx] = pointsToMerge.posZ[i];

        outputPoints.normalX[pointIdx] = pointsToMerge.normalX[i];
        outputPoints.normalY[pointIdx] = pointsToMerge.normalY[i];
        outputPoints.normalZ[pointIdx] = pointsToMerge.normalZ[i];

        outputPoints.colorR[pointIdx] = pointsToMerge.colorR[i];
        outputPoints.colorG[pointIdx] = pointsToMerge.colorG[i];
        outputPoints.colorB[pointIdx] = pointsToMerge.colorB[i];

        pointIdx++;
    }

    auto& outputVertices = t_mesh.vertices;
    const size_t numVertices = outputVertices.size();
    size_t vertexIdx = numVertices;

    const auto& verticesToMerge = t_mesh_2.vertices;
    outputVertices.reserve(outputVertices.size() + verticesToMerge.size());
    outputVertices.resize(outputVertices.size() + verticesToMerge.size());
    for (size_t i = 0; i < verticesToMerge.size(); ++i) {
        Vertex vert = verticesToMerge[i];
        vert.refPoint += numPoints;
        outputVertices[vertexIdx] = vert;
        vertexIdx++;
    }

    auto& outputPrimitives = t_mesh.primitives;
    size_t primIdx = outputPrimitives.size();

    const auto& primsToMerge = t_mesh_2.primitives;
    outputPrimitives.reserve(outputPrimitives.size() + primsToMerge.size());
    outputPrimitives.resize(outputPrimitives.size() + primsToMerge.size());
    for (size_t i = 0; i < primsToMerge.size(); ++i) {
        Primitive prim = primsToMerge[i];
        prim.verticesIndex += numVertices;
        outputPrimitives[primIdx] = prim;
        primIdx++;
    }
}

} // namespace butter