#include "nodes_info.h"

#include "primitive/cube.h"
#include "geometry/transform.h"

namespace butter {
    
static const std::vector<std::pair<std::string, std::vector<NodeMenuItem>>> nodeMenuItems = {
    {"Primitive", 
        {
            {"Cube", []() { return std::make_shared<Cube>(); }},
        }
    },
    {"Geometry",
        {
            {"Transform", []() { return std::make_shared<Transform>(); }}
        }
    }
};
    
    // Provide access to the internal static vector
const std::vector<std::pair<std::string, std::vector<NodeMenuItem>>>& NodesInfo::getMenuItems() {
    return nodeMenuItems;
}

}