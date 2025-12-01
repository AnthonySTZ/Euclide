#pragma once

#include <glad/glad.h>
#include "geometry/mesh.h"
#include <memory>

namespace euclide {

/// @brief Structure representing a single vertex in the render model.
struct RenderVertex {
    float position[3]; ///< Vertex position in 3D space.
    float color[3];    ///< Vertex color (RGB).
    float normal[3];   ///< Vertex normal for lighting calculations.
};

/// @brief Encapsulates OpenGL buffers for rendering a mesh, including vertices, edges, and points.
class RenderModel {
  public:
    /// @brief Construct an empty RenderModel and initialize OpenGL buffers.
    RenderModel();

    /// @brief Destroy the RenderModel and release OpenGL resources.
    ~RenderModel();

    /// @brief Update the render buffers with mesh data.
    /// @param t_mesh Mesh containing points and primitives to render.
    void updateWithMesh(const Mesh& t_mesh);

    /// @brief Draw the mesh triangles.
    void draw() const;

    /// @brief Draw the mesh points as GL_POINTS.
    void drawPoints() const;

    /// @brief Draw the mesh edges as GL_LINES.
    void drawEdges() const;

    /// @brief Toggle visibility of primitives (triangles).
    void tooglePrimitives() noexcept { showPrimitives = !showPrimitives; }

    /// @brief Toggle visibility of the wireframe (edges).
    void toogleWireframe() noexcept { showWireframe = !showWireframe; }

    /// @brief Toggle visibility of points.
    void tooglePoints() noexcept { showPoints = !showPoints; }

    bool showPrimitives = true; ///< Whether triangles should be drawn.
    bool showWireframe = true;  ///< Whether edges should be drawn.
    bool showPoints = false;    ///< Whether points should be drawn.

    [[nodiscard]] inline int numOfPoints() const noexcept { return m_numOfPoints; }
    [[nodiscard]] inline int numOfPrims() const noexcept { return m_numOfPrims; }

  private:
    /// @brief Initialize OpenGL VAO, VBO, and EBOs.
    void initBuffers();

    /// @brief Upload vertex data to the VBO.
    /// @param vertices Vector of vertices to upload.
    void bindVBO(const std::vector<RenderVertex>& vertices);

    /// @brief Upload triangle indices to the element buffer.
    /// @param vertexIndices Indices of vertices forming triangles.
    void bindEBOVertex(const std::vector<uint32_t>& vertexIndices);

    /// @brief Upload point indices to the element buffer.
    /// @param pointIndices Indices of vertices forming points.
    void bindEBOPoints(const std::vector<uint32_t>& pointIndices);

    /// @brief Upload edge indices to the element buffer.
    /// @param edges Indices of vertices forming edges.
    void bindEBOEdges(const std::vector<uint32_t>& edges);

  private:
    uint32_t m_vao = 0;       ///< Vertex Array Object ID.
    uint32_t m_vbo = 0;       ///< Vertex Buffer Object ID.
    uint32_t m_eboVertex = 0; ///< Element Buffer Object for triangle indices.
    uint32_t m_eboPoints = 0; ///< Element Buffer Object for point indices.
    uint32_t m_eboEdges = 0;  ///< Element Buffer Object for edge indices.

    size_t m_numOfPoints = 0;        ///< Total number of points.
    size_t m_numOfVertexIndices = 0; ///< Total number of triangle indices.
    size_t m_numOfEdgesIndices = 0;  ///< Total number of edge indices.

    size_t m_numOfPrims = 0;
};

} // namespace euclide
