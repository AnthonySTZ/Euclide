#include "subdivide.h"

#include "smooth_normals.h"

#include <iostream>

namespace euclide {

Subdivide::Subdivide() : Node(1, 1, "Subdivide") {
    auto divisionsField = std::make_shared<NodeField<int>>(1);
    divisionsField->setMetadata(NodeFieldMetadata{
        displayName : "Divisions",
        min : 0,
        max : 10,
        step : 1,
    });
    addField("divisions", divisionsField);
}

std::shared_ptr<Mesh> Subdivide::compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) {
    if (t_inputs[0] == nullptr)
        return std::make_shared<Mesh>();

    auto output = std::make_shared<Mesh>(*t_inputs[0]);

    const int divisions = getField<NodeField<int>>("divisions")->getValue();

    const SubdivideSettings settings{.divisions = divisions};

    subdivide(*output, settings);
    SmoothNormals::smoothNormals(*output);

    return output;
}

size_t valence(const size_t t_idx, const std::vector<HalfEdge>& t_halfedges) {
    // Number of edges connected to a point
    size_t n = 1;
    size_t h_primeIdx = t_halfedges[t_halfedges[t_idx].twin].next;
    while (h_primeIdx != t_idx) {
        n++;
        h_primeIdx = t_halfedges[t_halfedges[h_primeIdx].twin].next;
    }
    return n;
}

size_t isOnBorder(const size_t t_idx, const std::vector<HalfEdge>& t_halfedges) {
    if (t_halfedges[t_idx].twin == HalfEdge::NO_TWIN)
        return true;

    size_t h_primeIdx = t_halfedges[t_halfedges[t_idx].twin].next;
    while (h_primeIdx != t_idx) {
        if (t_halfedges[h_primeIdx].twin == HalfEdge::NO_TWIN)
            return true;
        h_primeIdx = t_halfedges[t_halfedges[h_primeIdx].twin].next;
    }

    return false;
}

void Subdivide::subdivide(Mesh& t_mesh, const SubdivideSettings& t_settings) {
    // Base on paper: 'A Halfedge Refinement Rule for Parallel Catmull-Clark Subdivision'
    // `https://onrendering.com/data/papers/catmark/HalfedgeCatmullClark.pdf`
    for (size_t iteration = 0; iteration < t_settings.divisions; ++iteration) {
        const auto& points_d = t_mesh.pointAttribs;
        const auto& primitives_d = t_mesh.primitives;

        const std::vector<HalfEdge> halfedges_d = t_mesh.computeHalfEdges();
        const uint32_t numOfEdges = getNumberOfEdgesFromHalfedges(halfedges_d);
        const uint32_t numOfPoints = points_d.size();
        const uint32_t numOfPrims = primitives_d.size();

        // Init new Halfedge
        std::vector<HalfEdge> halfedges_d1(halfedges_d.size() * 4);

        // Init new Points
        AttributeSet points_d1{};
        points_d1.resize(numOfPoints + numOfPrims + numOfEdges);

        // Halfedge refinement
        halfedgeRefinement(halfedges_d1, halfedges_d, numOfPoints, numOfPrims);

        // Face points
        computeFacePoints(halfedges_d, points_d1, points_d, primitives_d, numOfPoints);

        // Smooth edge points
        smoothEdgePoints(halfedges_d, points_d1, points_d, numOfPoints, numOfPrims);

        // Smooth vertex points
        smoothVertexPoints(halfedges_d, points_d1, points_d, numOfPoints, numOfPrims);

        t_mesh.reconstructFromHalfEdges(halfedges_d1, points_d1);
    }
}

void Subdivide::halfedgeRefinement(std::vector<HalfEdge>& t_halfedges_d1, const std::vector<HalfEdge>& t_halfedges_d,
                                   const uint32_t t_numOfPoints, const uint32_t t_numOfPrims) {
    HalfEdge* __restrict he_d1 = t_halfedges_d1.data();

#pragma omp parallel for
    for (size_t h = 0; h < t_halfedges_d.size(); ++h) {
        const HalfEdge& hd = t_halfedges_d[h];
        const size_t newFaceId = 4 * h;

        he_d1[newFaceId].next = newFaceId + 1;
        he_d1[newFaceId].origin = hd.origin;
        he_d1[newFaceId].face = h;

        he_d1[newFaceId + 1].next = newFaceId + 2;
        he_d1[newFaceId + 1].origin = t_numOfPoints + t_numOfPrims + hd.edge;
        he_d1[newFaceId + 1].face = h;

        he_d1[newFaceId + 2].next = newFaceId + 3;
        he_d1[newFaceId + 2].origin = t_numOfPoints + hd.face;
        he_d1[newFaceId + 2].face = h;

        he_d1[newFaceId + 3].origin = t_numOfPoints + t_numOfPrims + t_halfedges_d[hd.prev].edge;
        he_d1[newFaceId + 3].next = newFaceId;
        he_d1[newFaceId + 3].face = h;
    }
}

void Subdivide::computeFacePoints(const std::vector<HalfEdge>& t_halfedges_d, AttributeSet& t_points_d1,
                                  const AttributeSet& t_points_d, const std::vector<Primitive>& t_primitives_d,
                                  const uint32_t t_numOfPoints) {
    const auto pointsD_Positions = t_points_d.find("P");
    auto pointsD1_Positions = t_points_d1.findOrCreate<float, 3>("P");

    float* pointsD1PosX = pointsD1_Positions->component<float>(0);
    float* pointsD1PosY = pointsD1_Positions->component<float>(1);
    float* pointsD1PosZ = pointsD1_Positions->component<float>(2);

    const float* pointsDPosX = pointsD_Positions->component<float>(0);
    const float* pointsDPosY = pointsD_Positions->component<float>(1);
    const float* pointsDPosZ = pointsD_Positions->component<float>(2);

    for (size_t h = 0; h < t_halfedges_d.size(); ++h) {
        const HalfEdge& hd = t_halfedges_d[h];
        const float factor = 1.0f / static_cast<float>(t_primitives_d[hd.face].numVertices);
        const uint32_t i = t_numOfPoints + hd.face;
        pointsD1PosX[i] += pointsDPosX[hd.origin] * factor;
        pointsD1PosY[i] += pointsDPosY[hd.origin] * factor;
        pointsD1PosZ[i] += pointsDPosZ[hd.origin] * factor;
    }
}

void Subdivide::smoothEdgePoints(const std::vector<HalfEdge>& t_halfedges_d, AttributeSet& t_points_d1,
                                 const AttributeSet& t_points_d, const uint32_t t_numOfPoints,
                                 const uint32_t t_numOfPrims) {
    const auto pointsD_Positions = t_points_d.find("P");
    auto pointsD1_Positions = t_points_d1.findOrCreate<float, 3>("P");

    float* pointsD1PosX = pointsD1_Positions->component<float>(0);
    float* pointsD1PosY = pointsD1_Positions->component<float>(1);
    float* pointsD1PosZ = pointsD1_Positions->component<float>(2);

    const float* pointsDPosX = pointsD_Positions->component<float>(0);
    const float* pointsDPosY = pointsD_Positions->component<float>(1);
    const float* pointsDPosZ = pointsD_Positions->component<float>(2);

    for (uint32_t h = 0; h < t_halfedges_d.size(); ++h) {
        const HalfEdge& hd = t_halfedges_d[h];
        const uint32_t i = t_numOfPoints + hd.face;
        const size_t j = t_numOfPoints + t_numOfPrims + hd.edge;
        if (hd.twin == HalfEdge::NO_TWIN) {
            const uint32_t next = t_halfedges_d[hd.next].origin;
            pointsD1PosX[j] = (pointsDPosX[hd.origin] + pointsDPosX[next]) * 0.5f;
            pointsD1PosY[j] = (pointsDPosY[hd.origin] + pointsDPosY[next]) * 0.5f;
            pointsD1PosZ[j] = (pointsDPosZ[hd.origin] + pointsDPosZ[next]) * 0.5f;
            continue;
        }

        pointsD1PosX[j] += (pointsDPosX[hd.origin] + pointsD1PosX[i]) * 0.25f;
        pointsD1PosY[j] += (pointsDPosY[hd.origin] + pointsD1PosY[i]) * 0.25f;
        pointsD1PosZ[j] += (pointsDPosZ[hd.origin] + pointsD1PosZ[i]) * 0.25f;
    }
}

void Subdivide::smoothVertexPoints(const std::vector<HalfEdge>& t_halfedges_d, AttributeSet& t_points_d1,
                                   const AttributeSet& t_points_d, const uint32_t t_numOfPoints,
                                   const uint32_t t_numOfPrims) {
    const auto pointsD_Positions = t_points_d.find("P");
    auto pointsD1_Positions = t_points_d1.findOrCreate<float, 3>("P");

    float* pointsD1PosX = pointsD1_Positions->component<float>(0);
    float* pointsD1PosY = pointsD1_Positions->component<float>(1);
    float* pointsD1PosZ = pointsD1_Positions->component<float>(2);

    const float* pointsDPosX = pointsD_Positions->component<float>(0);
    const float* pointsDPosY = pointsD_Positions->component<float>(1);
    const float* pointsDPosZ = pointsD_Positions->component<float>(2);

    const uint32_t edgeCount = static_cast<uint32_t>(t_halfedges_d.size());

    std::vector<uint8_t> onBorder(edgeCount);
    std::vector<uint32_t> valences(edgeCount);
#pragma omp parallel for
    for (uint32_t h = 0; h < edgeCount; ++h) {
        const bool border = isOnBorder(h, t_halfedges_d);
        onBorder[h] = border ? 1 : 0;
        if (!border) {
            valences[h] = valence(h, t_halfedges_d);
        }
    }

    for (uint32_t h = 0; h < edgeCount; ++h) {
        const HalfEdge& hd = t_halfedges_d[h];
        const uint32_t origin = hd.origin;

        if (onBorder[h]) { // TODO: add checkbox for this
            pointsD1PosX[origin] = pointsDPosX[origin];
            pointsD1PosY[origin] = pointsDPosY[origin];
            pointsD1PosZ[origin] = pointsDPosZ[origin];
            continue;
        }
        const float n = static_cast<float>(valences[h]);
        const float invN = 1.0f / n;
        const float factor = invN * invN;
        const float n_3 = n - 3;
        const uint32_t i = t_numOfPoints + hd.face;
        const size_t j = t_numOfPoints + t_numOfPrims + hd.edge;
        pointsD1PosX[origin] += (4.0f * pointsD1PosX[j] - pointsD1PosX[i] + n_3 * pointsDPosX[origin]) * factor;
        pointsD1PosY[origin] += (4.0f * pointsD1PosY[j] - pointsD1PosY[i] + n_3 * pointsDPosY[origin]) * factor;
        pointsD1PosZ[origin] += (4.0f * pointsD1PosZ[j] - pointsD1PosZ[i] + n_3 * pointsDPosZ[origin]) * factor;
    }
}

} // namespace euclide