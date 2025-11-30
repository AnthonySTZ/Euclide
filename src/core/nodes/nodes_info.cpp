#include "nodes_info.h"

#include "primitive/cube.h"
#include "primitive/grid.h"
#include "primitive/cylinder.h"

#include "geometry/transform.h"
#include "geometry/merge.h"
#include "geometry/subdivide.h"

namespace butter {

/// @brief Static list of all node menu items categorized.
static const std::vector<std::pair<std::string, std::vector<NodeMenuItem>>> nodeMenuItems = {
    {"Primitive",
     {
         {"Cube", []() { return std::make_shared<Cube>(); }},
         {"Grid", []() { return std::make_shared<Grid>(); }},
         {"Cylinder", []() { return std::make_shared<Cylinder>(); }},
     }},
    {"Geometry",
     {
         {"Transform", []() { return std::make_shared<Transform>(); }},
         {"Merge", []() { return std::make_shared<Merge>(); }},
         {"Subdivide", []() { return std::make_shared<Subdivide>(); }},
     }}};

const std::vector<std::pair<std::string, std::vector<NodeMenuItem>>>& NodesInfo::getMenuItems() {
    return nodeMenuItems;
}

} // namespace butter