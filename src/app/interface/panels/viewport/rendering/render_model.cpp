#include "render_model.h"

#include "utils/timer.h"

#include <numeric>

namespace euclide {

RenderModel::RenderModel() {
    initBuffers();
}

RenderModel::~RenderModel() {
    if (m_vbo != 0) {
        glDeleteBuffers(1, &m_vbo);
        m_vbo = 0;
    }
    if (m_eboVertex != 0) {
        glDeleteBuffers(1, &m_eboVertex);
        m_eboVertex = 0;
    }
    if (m_eboEdges != 0) {
        glDeleteBuffers(1, &m_eboEdges);
        m_eboEdges = 0;
    }
    if (m_vaoTriangles != 0) {
        glDeleteVertexArrays(1, &m_vaoTriangles);
        m_vaoTriangles = 0;
    }
    if (m_vaoEdges != 0) {
        glDeleteVertexArrays(1, &m_vaoEdges);
        m_vaoEdges = 0;
    }
}

void RenderModel::initBuffers() {
    glGenVertexArrays(1, &m_vaoTriangles);
    glGenVertexArrays(1, &m_vaoEdges);

    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_eboVertex);
    glGenBuffers(1, &m_eboEdges);

    glBindVertexArray(m_vaoTriangles);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(RenderVertex),
                          (void*)offsetof(RenderVertex, position)); // Position
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(RenderVertex),
                          (void*)offsetof(RenderVertex, color)); // Color
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(RenderVertex),
                          (void*)offsetof(RenderVertex, normal)); // Normal

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboVertex);
    glBindVertexArray(0);

    glBindVertexArray(m_vaoEdges);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(RenderVertex),
                          (void*)offsetof(RenderVertex, position)); // Position

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboEdges);
    glBindVertexArray(0);
}

void RenderModel::updateWithMesh(const Mesh& t_mesh) {
    Timer timer{"Build Render Model"}; // 855.484ms for QuadSphere 11

    m_numOfPrims = t_mesh.primitives.size();

    computePoints(t_mesh.pointAttribs);
    computeEdgesAndPrims(t_mesh);
}

void RenderModel::computePoints(const AttributeSet& t_pointAttribs) {
    Timer timer{"Points"}; // 206 ms with omp for QuadSphere 11

    m_numOfPoints = t_pointAttribs.size();
    const auto positions = t_pointAttribs.find("P");
    const auto normals = t_pointAttribs.find("N");
    const auto colors = t_pointAttribs.find("Cd");

    const bool hasPos = isFloat3(positions);
    const bool hasNormals = isFloat3(normals);
    const bool hasCol = isFloat3(colors);

    const float* __restrict points_posX = nullptr;
    const float* __restrict points_posY = nullptr;
    const float* __restrict points_posZ = nullptr;

    const float* __restrict points_normalX = nullptr;
    const float* __restrict points_normalY = nullptr;
    const float* __restrict points_normalZ = nullptr;

    const float* __restrict points_colorR = nullptr;
    const float* __restrict points_colorG = nullptr;
    const float* __restrict points_colorB = nullptr;

    if (hasPos) {
        points_posX = positions->component<float>(0);
        points_posY = positions->component<float>(1);
        points_posZ = positions->component<float>(2);
    }

    if (hasNormals) {
        points_normalX = normals->component<float>(0);
        points_normalY = normals->component<float>(1);
        points_normalZ = normals->component<float>(2);
    }

    if (hasCol) {
        points_colorR = colors->component<float>(0);
        points_colorG = colors->component<float>(1);
        points_colorB = colors->component<float>(2);
    }

    {
        std::vector<RenderVertex> vertices;
        vertices.resize(m_numOfPoints);
        RenderVertex* __restrict verticesPtr = vertices.data();

#pragma omp parallel for
        for (size_t i = 0; i < m_numOfPoints; ++i) {
            RenderVertex& vertex = verticesPtr[i];
            vertex.position[0] = hasPos ? points_posX[i] : 0.0f;
            vertex.position[1] = hasPos ? points_posY[i] : 0.0f;
            vertex.position[2] = hasPos ? points_posZ[i] : 0.0f;

            vertex.color[0] = hasCol ? points_colorR[i] : 1.0f;
            vertex.color[1] = hasCol ? points_colorG[i] : 1.0f;
            vertex.color[2] = hasCol ? points_colorB[i] : 1.0f;

            vertex.normal[0] = hasNormals ? points_normalX[i] : 0.0f;
            vertex.normal[1] = hasNormals ? points_normalY[i] : 0.0f;
            vertex.normal[2] = hasNormals ? points_normalZ[i] : 0.0f;
        }

        bindVBO(vertices);
    }
}

void RenderModel::computeEdgesAndPrims(const Mesh& t_mesh) {
    size_t totalTriangles = 0;
    size_t totalEdges = 0;
    for (const auto& prim : t_mesh.primitives) {
        const uint32_t numVertices = prim.numVertices;
        if (numVertices <= 1)
            continue;
        totalEdges += numVertices == 2 ? 1 : numVertices;
        if (numVertices <= 2)
            continue;
        totalTriangles += numVertices - 2;
    }
    std::vector<uint32_t> vertexIndices(totalTriangles * 3);
    std::vector<uint32_t> edges(totalEdges * 2);
    uint32_t* vertexIndicesPtr = vertexIndices.data();
    uint32_t* edgesPtr = edges.data();

    {
        Timer timer{"Prim"}; // 200ms for QuadSphere 11
        const Vertex* vertices = t_mesh.vertices.data();
        for (const auto& prim : t_mesh.primitives) {
            // Edges
            const uint32_t numVertices = prim.numVertices;
            if (numVertices <= 1)
                continue;

            const uint32_t vertexIndex = prim.verticesIndex;
            for (size_t i = 0; i < numVertices; ++i) {
                *edgesPtr++ = vertices[vertexIndex + i].refPoint;
                *edgesPtr++ = vertices[(i + 1) == numVertices ? vertexIndex : vertexIndex + i + 1].refPoint;
            }

            // Vertices
            if (numVertices <= 2)
                continue;
            const uint32_t v0 = vertices[vertexIndex].refPoint;
            for (size_t i = 1; i + 1 < numVertices; ++i) {
                *vertexIndicesPtr++ = v0;
                *vertexIndicesPtr++ = vertices[vertexIndex + i].refPoint;
                *vertexIndicesPtr++ = vertices[vertexIndex + i + 1].refPoint;
            }
        }
    }

    m_numOfVertexIndices = vertexIndices.size();
    bindEBOVertex(vertexIndices);

    m_numOfEdgesIndices = edges.size();
    bindEBOEdges(edges);
}

void RenderModel::bindVBO(const std::vector<RenderVertex>& vertices) {
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    size_t newSize = vertices.size() * sizeof(RenderVertex);
    glBufferData(GL_ARRAY_BUFFER, newSize, nullptr, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, newSize, vertices.data());
}

void RenderModel::bindEBOVertex(const std::vector<uint32_t>& vertexIndices) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboVertex);
    size_t newSize = vertexIndices.size() * sizeof(uint32_t);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, newSize, nullptr, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, newSize, vertexIndices.data());
}

void RenderModel::bindEBOEdges(const std::vector<uint32_t>& edges) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboEdges);
    size_t newSize = m_numOfEdgesIndices * sizeof(uint32_t);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, newSize, nullptr, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, newSize, edges.data());
}

void RenderModel::draw() const {
    glBindVertexArray(m_vaoTriangles);
    glDrawElements(GL_TRIANGLES, m_numOfVertexIndices, GL_UNSIGNED_INT, 0);
}

void RenderModel::drawPoints() const {
    glBindVertexArray(m_vaoTriangles);
    glDrawArrays(GL_POINTS, 0, m_numOfPoints);
}

void RenderModel::drawEdges() const {
    glLineWidth(edgesLineWidth);
    glBindVertexArray(m_vaoEdges);
    glDrawElements(GL_LINES, m_numOfEdgesIndices, GL_UNSIGNED_INT, 0);
}

} // namespace euclide