#pragma once

#include "points.h"
#include "vertex.h"
#include "primitive.h"

namespace butter {

struct Mesh {
    Points points;
    std::vector<Vertex> vertices;
    std::vector<Primitive> primitives;

    size_t addPoint(const float t_x, const float t_y, const float t_z) {
        return points.addPoint(t_x, t_y, t_z);
    }

    size_t addVertex(const size_t t_pointIndex, const size_t t_primIndex) {
        vertices.push_back({t_pointIndex, t_primIndex});
        return vertices.size() - 1;
    }

    size_t addPrimitive(const std::vector<size_t>& pointIndices) {
        std::vector<size_t> vertexIndices;
        vertexIndices.reserve(pointIndices.size());

        const size_t primitiveIndex = primitives.size();
        for (const size_t pointId: pointIndices) {
            vertexIndices.push_back(addVertex(pointId, primitiveIndex));
        }
        primitives.push_back({vertexIndices});
        return primitiveIndex;
    }
};

}