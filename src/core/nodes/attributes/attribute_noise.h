#pragma once

#include "nodes/node.h"

#include "gpu/gpu_manager.h"
#include "gpu/gpu_buffer.h"
#include "gpu/gpu_descriptor.h"
#include "gpu/gpu_pipeline.h"
#include "gpu/gpu_compute_task.h"

namespace euclide {

class AttributeNoise : public Node {
    struct PerlinParams {
        int numPoints;
        int octaves;
        float frequency;
    };

    struct PerlinNoiseSettings {
        int octaves;
        float frequency;
    };

  public:
    enum Kind { POINTS = 0, PRIMITIVES = 1 };

    AttributeNoise();

    static void perlinNoise(Mesh& t_mesh, AttributeSet& t_attribs, const std::string& t_name, const int t_attrSize,
                            const PerlinNoiseSettings& t_settings);

  private:
    std::shared_ptr<Mesh> compute(const size_t t_index,
                                  const std::vector<std::shared_ptr<Mesh>>& t_inputs) const override;

  public:
    static const std::array<int, 512> perlinPermutations;
};

} // namespace euclide
