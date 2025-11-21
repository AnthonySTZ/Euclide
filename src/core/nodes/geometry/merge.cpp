#include "merge.h"

#include "utils/timer.h"

#ifdef USE_SIMD
#include "utils/simd.h"
#endif

namespace butter {
    
Merge::Merge()
    : Node(2, 1, "Merge")
{}

std::shared_ptr<Mesh> Merge::compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>> &t_inputs)
{
    if (t_inputs[0] == nullptr && t_inputs[1] == nullptr) return std::make_shared<Mesh>();
    if (t_inputs[0] == nullptr) return std::make_shared<Mesh>(*t_inputs[1]);
    if (t_inputs[1] == nullptr) return std::make_shared<Mesh>(*t_inputs[0]);

    auto output = std::make_shared<Mesh>(*t_inputs[0]);
    
    Timer timer{"Merge"};

    return output;
}

}