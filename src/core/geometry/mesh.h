#pragma once

#include "vertex.h"
#include "primitive.h"
#include "halfedge.h"
#include "attributes.h"
#include "utils/types.h"

#include <vector>

namespace euclide {
/// @brief Represents a 3D mesh composed of points, vertices, and primitives.
struct Mesh {
    std::vector<Vertex> vertices;      ///< Vertices referencing points.
    std::vector<Primitive> primitives; ///< Faces (polygons) of the mesh.
    AttributeSet pointAttribs;
    AttributeSet vertexAttribs;
    AttributeSet primAttribs; // TODO: resize it when add new prims, same for vertex
    AttributeSet detailAttribs;

    /// @brief Add a point to the mesh.
    /// @param t_x X-coordinate.
    /// @param t_y Y-coordinate.
    /// @param t_z Z-coordinate.
    /// @return Index of the new point.
    inline uint32_t addPoint(const float t_x, const float t_y, const float t_z) {
        uint32_t index = pointAttribs.size();
        pointAttribs.resize(index + 1);
        Attribute* positions = pointAttribs.findOrCreate("P", 3, AttributeType::ATTR_TYPE_FLOAT);
        float* posDataX = positions->component<float>(0);
        float* posDataY = positions->component<float>(1);
        float* posDataZ = positions->component<float>(2);

        posDataX[index] = t_x;
        posDataY[index] = t_y;
        posDataZ[index] = t_z;

        return index;
    }

    /// @brief Add a point to the mesh.
    /// @param t_position Position vector of the point.
    /// @return Index of the new point.
    inline uint32_t addPoint(const float3 t_position) { return addPoint(t_position[0], t_position[1], t_position[2]); }

    /// @brief Add a vertex referencing an existing point.
    /// @param t_pointIndex Index of the point this vertex references.
    /// @return Index of the new vertex.
    inline uint32_t addVertex(const uint32_t t_pointIndex) {
        vertices.push_back(Vertex{t_pointIndex});
        return static_cast<uint32_t>(vertices.size() - 1);
    }

    /// @brief Add a primitive (face) to the mesh.
    /// @param pointIndices Vector of point indices forming the primitive.
    /// @return Index of the new primitive.
    inline uint32_t addPrimitive(const std::vector<uint32_t>& pointIndices) {
        const uint32_t primitiveIndex = static_cast<uint32_t>(primitives.size());

        primitives.emplace_back(
            Primitive{static_cast<uint32_t>(vertices.size()), static_cast<uint32_t>(pointIndices.size())});

        vertices.reserve(pointIndices.size());
        for (const uint32_t point : pointIndices) {
            vertices.emplace_back(Vertex{point});
        }

        return primitiveIndex;
    }

    /// @brief Get point indices of a given primitive.
    /// @param t_primId Index of the primitive.
    /// @return Vector of point indices forming the primitive.
    inline std::vector<uint32_t> getPointIndicesOfPrimitive(const uint32_t t_primId) {
        const Primitive& prim = primitives[t_primId];
        std::vector<uint32_t> ids;
        ids.reserve(prim.numVertices);
        for (size_t i = prim.verticesIndex; i < (prim.verticesIndex + prim.numVertices); ++i) {
            ids.push_back(vertices[i].refPoint);
        }
        return ids;
    }

    /// @brief Compute half-edges for the mesh.
    /// @return Vector of half-edge structures representing the mesh connectivity.
    std::vector<HalfEdge> computeHalfEdges() const;

    /// @brief Reconstruct the mesh from a set of half-edges and points.
    /// @param t_halfedges Half-edges describing the mesh.
    /// @param t_points Points for the mesh.
    void reconstructFromHalfEdges(const std::vector<HalfEdge>& t_halfedges, const AttributeSet& t_pointAttribs);

    /// @brief Compute the geometric center of the mesh points.
    /// @return Center position as float3.
    float3 center();
};

} // namespace euclide
