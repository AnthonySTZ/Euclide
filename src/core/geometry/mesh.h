#pragma once

#include "points.h"
#include "vertex.h"
#include "primitive.h"

namespace butter {

struct Mesh {
    Points points;
    std::vector<Vertex> vertices;
    std::vector<Primitive> primitives;

    inline uint32_t addPoint(const float t_x, const float t_y, const float t_z) {
        return points.addPoint(t_x, t_y, t_z);
    }

    inline uint32_t addVertex(const uint32_t t_pointIndex, const uint32_t t_primIndex) {
        vertices.push_back({t_pointIndex, t_primIndex});
        return static_cast<uint32_t>(vertices.size() - 1);
    }

    inline uint32_t addPrimitive(const std::vector<uint32_t>& pointIndices) {
        std::vector<uint32_t> vertexIndices;
        vertexIndices.reserve(pointIndices.size());

        const uint32_t primitiveIndex = static_cast<uint32_t>(primitives.size());
        for (const uint32_t pointId: pointIndices) {
            vertexIndices.push_back(addVertex(pointId, primitiveIndex));
        }
        primitives.push_back({vertexIndices});
        return primitiveIndex;
    }

    inline std::vector<uint32_t> getPointIndicesOfPrimitive(const uint32_t t_primitiveId) {
        const Primitive& primitive = primitives[t_primitiveId];
        
        std::vector<uint32_t> pointIndices;
        pointIndices.reserve(primitive.vertices.size());
        for (uint32_t vertexIndex : primitive.vertices) {
            pointIndices.push_back(vertices[vertexIndex].refPoint);
        }
        return pointIndices;
        }
};

}