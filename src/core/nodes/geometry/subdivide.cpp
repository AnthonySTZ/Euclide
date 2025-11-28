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

void Subdivide::subdivide(Mesh &t_mesh, const SubdivideSettings& t_settings)
{   
    // Base on paper: 'A Halfedge Refinement Rule for Parallel Catmull-Clark Subdivision'
    // `https://onrendering.com/data/papers/catmark/HalfedgeCatmullClark.pdf`
    auto& points = t_mesh.points;
    auto& primitives = t_mesh.primitives;
    auto& vertices = t_mesh.vertices;
    
    const std::vector<HalfEdge> halfEdges_d = t_mesh.computeHalfEdges();
    size_t numOfEdges = 0;
    for (size_t h = 0; h < halfEdges_d.size(); ++h) {
        const HalfEdge& hd = halfEdges_d[h];
        if (hd.twin > h) numOfEdges++;
    }

    // Halfedge refinement
    std::vector<HalfEdge> halfEdges_d1(halfEdges_d.size() * 4); // Next division
    for (size_t h = 0; h < halfEdges_d.size(); ++h) {
        const HalfEdge& hd = halfEdges_d[h];
        const size_t newFaceId = 4 * h;
        halfEdges_d1[newFaceId].twin = hd.twin == HalfEdge::NO_TWIN ? HalfEdge::NO_TWIN : 4 * halfEdges_d[hd.twin].next + 3;
        halfEdges_d1[newFaceId + 1].twin = 4 * hd.next + 2;
        halfEdges_d1[newFaceId + 2].twin = 4 * hd.prev + 1;
        halfEdges_d1[newFaceId + 3].twin = hd.twin == HalfEdge::NO_TWIN ? HalfEdge::NO_TWIN : 4 * halfEdges_d[hd.twin].prev;
    }

    // Face points
    size_t numOfPoints = points.size();
    Points points_d1{};
    points_d1.resize(numOfPoints + primitives.size() + numOfEdges);
    std::fill(std::begin(points_d1.colorR), std::end(points_d1.colorR), 1.0);
    std::fill(std::begin(points_d1.colorG), std::end(points_d1.colorG), 1.0);
    std::fill(std::begin(points_d1.colorB), std::end(points_d1.colorB), 1.0);

    std::fill(std::begin(points_d1.normalX), std::end(points_d1.normalX), 0.0);
    std::fill(std::begin(points_d1.normalY), std::end(points_d1.normalY), 1.0);
    std::fill(std::begin(points_d1.normalZ), std::end(points_d1.normalZ), 0.0);



    for (size_t h = 0; h < halfEdges_d.size(); ++h) {
        const HalfEdge& hd = halfEdges_d[h];
        const float factor = 1.0f / static_cast<float>(primitives[hd.face].numVertices);
        const uint32_t i = numOfPoints + hd.face;
        points_d1.posX[i] += points.posX[hd.origin] * factor; 
        points_d1.posY[i] += points.posY[hd.origin] * factor; 
        points_d1.posZ[i] += points.posZ[hd.origin] * factor; 
    }

    // Smooth edge points
    for (uint32_t h = 0; h < halfEdges_d.size(); ++h) {
        const HalfEdge& hd = halfEdges_d[h];
        const uint32_t i = numOfPoints + hd.face;
        const size_t j = numOfPoints + primitives.size() + hd.edge;
        points_d1.posX[j] += (points.posX[hd.origin] + points_d1.posX[i]) * 0.25f;
        points_d1.posY[j] += (points.posY[hd.origin] + points_d1.posY[i]) * 0.25f;
        points_d1.posZ[j] += (points.posZ[hd.origin] + points_d1.posZ[i]) * 0.25f;
    }

    // Smooth vertex points
    for (uint32_t h = 0; h < halfEdges_d.size(); ++h) {
        const HalfEdge& hd = halfEdges_d[h];
        // TODO: check if on border
        const float n = static_cast<float>(valence(h, halfEdges_d));
        const float factor = 1.0f / (n * n);
        const float n_3 = n - 3;
        const uint32_t i = numOfPoints + hd.face;
        const size_t j = numOfPoints + primitives.size() + hd.edge;
        points_d1.posX[hd.origin] += (4.0f * points_d1.posX[j] - points_d1.posX[i] + n_3 * points.posX[hd.origin]) * factor;
        points_d1.posY[hd.origin] += (4.0f * points_d1.posY[j] - points_d1.posY[i] + n_3 * points.posY[hd.origin]) * factor;
        points_d1.posZ[hd.origin] += (4.0f * points_d1.posZ[j] - points_d1.posZ[i] + n_3 * points.posZ[hd.origin]) * factor;
    }

    points = points_d1;
}



}