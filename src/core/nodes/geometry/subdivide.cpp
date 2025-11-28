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
    points.reserve(points.size() + primitives.size() + numOfEdges);
    points.resize(points.size() + primitives.size() + numOfEdges);
    for (size_t h = 0; h < halfEdges_d.size(); ++h) {
        const HalfEdge& hd = halfEdges_d[h];
        const float factor = 1.0f / static_cast<float>(primitives[hd.face].numVertices);
        const uint32_t i = numOfPoints + hd.face;
        points.posX[i] += points.posX[hd.origin] * factor; 
        points.posY[i] += points.posY[hd.origin] * factor; 
        points.posZ[i] += points.posZ[hd.origin] * factor; 
    }
}

}