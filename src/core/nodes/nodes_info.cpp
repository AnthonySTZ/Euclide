#include "nodes_info.h"

#include "primitive/cube.h"
#include "primitive/grid.h"

#include "geometry/transform.h"
#include "geometry/merge.h"

namespace butter {
    
static const std::vector<std::pair<std::string, std::vector<NodeMenuItem>>> nodeMenuItems = {
    {"Primitive", 
        {
            {"Cube", []() { return std::make_shared<Cube>(); }},
            {"Grid", []() { return std::make_shared<Grid>(); }},
        }
    },
    {"Geometry",
        {
            {"Transform", []() { return std::make_shared<Transform>(); }},
            {"Merge", []() { return std::make_shared<Merge>(); }},
        }
    }
};
    
    // Provide access to the internal static vector
const std::vector<std::pair<std::string, std::vector<NodeMenuItem>>>& NodesInfo::getMenuItems() {
    return nodeMenuItems;
}

}