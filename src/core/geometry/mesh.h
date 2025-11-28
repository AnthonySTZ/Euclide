#pragma once

#include "points.h"
#include "vertex.h"
#include "primitive.h"
#include "halfedge.h"
#include "utils/types.h"

namespace butter {

struct Mesh {
    Points points;
    std::vector<Vertex> vertices;
    std::vector<Primitive> primitives;

    inline uint32_t addPoint(const float t_x, const float t_y, const float t_z) {
        return points.addPoint(t_x, t_y, t_z);
    }

    inline uint32_t addPoint(const float3 t_position) {
        return points.addPoint(t_position[0], t_position[1], t_position[2]);
    }

    inline uint32_t addVertex(const uint32_t t_pointIndex) {
        vertices.push_back(Vertex{t_pointIndex});
        return static_cast<uint32_t>(vertices.size() - 1);
    }

    inline uint32_t addPrimitive(const std::vector<uint32_t>& pointIndices) {

        const uint32_t primitiveIndex = static_cast<uint32_t>(primitives.size());

        primitives.emplace_back(Primitive{
            static_cast<uint32_t>(vertices.size()), static_cast<uint32_t>(pointIndices.size())
        });

        vertices.reserve(pointIndices.size());
        for (const uint32_t point: pointIndices) {
            vertices.emplace_back(Vertex{point});
        }

        return primitiveIndex;
    }

    inline std::vector<uint32_t> getPointIndicesOfPrimitive(const uint32_t t_primId) {
        const Primitive& prim = primitives[t_primId];
        std::vector<uint32_t> ids;
        ids.reserve(prim.numVertices);
        for (size_t i = prim.verticesIndex; i < (prim.verticesIndex + prim.numVertices); ++i) {
            ids.push_back(vertices[i].refPoint);
        }
        return ids;
    }

    std::vector<HalfEdge> computeHalfEdges() const;
    void reconstructFromHalfEdges(const std::vector<HalfEdge>& t_halfedges, const Points& t_points);
    float3 center() const;
};

}