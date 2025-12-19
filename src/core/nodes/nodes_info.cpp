#include "nodes_info.h"

#include "primitive/cube.h"
#include "primitive/grid.h"
#include "primitive/cylinder.h"
#include "primitive/quad_sphere.h"

#include "geometry/transform.h"
#include "geometry/merge.h"
#include "geometry/subdivide.h"
#include "geometry/smooth_normals.h"
#include "geometry/copy_to_points.h"

#include "attributes/attribute_create.h"
#include "attributes/attribute_randomize.cpp"
#include "attributes/attribute_rename.cpp"
#include "attributes/attribute_noise.cpp"

#include "utility/null.h"

namespace euclide {

/// @brief Static list of all node menu items categorized.
static const std::vector<std::pair<std::string, std::vector<NodeMenuItem>>> nodeMenuItems = {
    {"Primitive",
     {
         {"Cube", []() { return std::make_shared<Cube>(); }},
         {"Grid", []() { return std::make_shared<Grid>(); }},
         {"Cylinder", []() { return std::make_shared<Cylinder>(); }},
         {"QuadSphere", []() { return std::make_shared<QuadSphere>(); }},
     }},
    {"Geometry",
     {
         {"Transform", []() { return std::make_shared<Transform>(); }},
         {"Merge", []() { return std::make_shared<Merge>(); }},
         {"Subdivide", []() { return std::make_shared<Subdivide>(); }},
         {"CopyToPoints", []() { return std::make_shared<CopyToPoints>(); }},
         {"SmoothNormals", []() { return std::make_shared<SmoothNormals>(); }},
     }},
    {"Attribute",
     {
         {"Attribute Create", []() { return std::make_shared<AttributeCreate>(); }},
         {"Attribute Randomize", []() { return std::make_shared<AttributeRandomize>(); }},
         {"Attribute Rename", []() { return std::make_shared<AttributeRename>(); }},
         {"Attribute Noise", []() { return std::make_shared<AttributeNoise>(); }},
     }},
    {"Utility",
     {
         {"Null", []() { return std::make_shared<Null>(); }},
     }}};

const std::vector<std::pair<std::string, std::vector<NodeMenuItem>>>& NodesInfo::getMenuItems() {
    return nodeMenuItems;
}

} // namespace euclide