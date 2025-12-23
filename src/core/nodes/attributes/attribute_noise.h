#pragma once

#include "nodes/node.h"

#include "gpu/gpu_manager.h"
#include "gpu/gpu_buffer.h"
#include "gpu/gpu_descriptor.h"
#include "gpu/gpu_pipeline.h"
#include "gpu/gpu_compute_task.h"

namespace euclide {

class PerlinNoise {
  public:
    struct BufferParams {
        int numPoints;
        int octaves;
        float frequency;
    };
    struct PerlinSettings {
        int octaves;
        float frequency;
    };

  public:
    PerlinNoise();

    void applyToMesh(Mesh& t_mesh, AttributeSet& t_attribs, const std::string& t_name, const int t_attrSize,
                     const PerlinSettings& t_settings);

    static const std::array<int, 512> perlinPermutations;

  private:
    GPUDevice& m_device;
    std::unique_ptr<GPUDescriptorSetLayout> m_descriptorSetLayout;
    GPUPipeline m_pipeline;

    GPUBuffer m_inBufPosX;
    GPUBuffer m_inBufPosY;
    GPUBuffer m_inBufPosZ;

    GPUBuffer m_inBufPermutations;
    GPUBuffer m_inBufParams;
    GPUBuffer m_outBuf;
};

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

  private:
    std::shared_ptr<Mesh> compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) override;

    PerlinNoise m_perlinNoise{};
};

} // namespace euclide
