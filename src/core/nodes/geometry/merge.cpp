#include "merge.h"

#include "utils/timer.h"

namespace euclide {

Merge::Merge() : Node(2, 1, "Merge") {
}

std::shared_ptr<Mesh> Merge::compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) {
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
    AttributeSet& pointAttribs = t_mesh.pointAttribs;
    const AttributeSet& otherPointAttribs = t_mesh_2.pointAttribs;

    const size_t oldSize = pointAttribs.size();
    const size_t numPointsToMerge = otherPointAttribs.size();

    pointAttribs.resize(oldSize + numPointsToMerge);
    t_mesh.primAttribs.resize(oldSize + numPointsToMerge);

#pragma omp parallel for
    for (size_t attrIdx = 0; attrIdx < pointAttribs.count(); ++attrIdx) {
        Attribute* attribute = pointAttribs.get(attrIdx);
        const Attribute* otherAttribute = otherPointAttribs.find(attribute->name());
        if (otherAttribute == nullptr) {
            continue;
        }
        attribute->copyAt(*otherAttribute, oldSize);
    }

    auto& outputVertices = t_mesh.vertices;
    const size_t numVertices = outputVertices.size();
    size_t vertexIdx = numVertices;

    const auto& verticesToMerge = t_mesh_2.vertices;
    outputVertices.reserve(outputVertices.size() + verticesToMerge.size());
    outputVertices.resize(outputVertices.size() + verticesToMerge.size());
    for (size_t i = 0; i < verticesToMerge.size(); ++i) {
        Vertex vert = verticesToMerge[i];
        vert.refPoint += oldSize;
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

} // namespace euclide