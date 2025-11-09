#pragma once 

#include <cstdint>
#include <glad/glad.h>

namespace butter {

class FrameBuffer {

public:
    FrameBuffer();
    ~FrameBuffer() noexcept;

    // No copy
    FrameBuffer(const FrameBuffer &) = delete;
    FrameBuffer &operator=(const FrameBuffer &) = delete;

    // Movable
    FrameBuffer(FrameBuffer &&t_other) noexcept;
    FrameBuffer &operator=(FrameBuffer &&t_other) noexcept;

    [[nodiscard]] uint32_t getRenderTexture() const noexcept;

    void blit(const uint32_t t_width, const uint32_t t_height);
    void resize(const uint32_t t_screenWidth, const uint32_t t_screenHeight);
    void bind() const noexcept;
    void unbind() const noexcept;

private:
    void initMultiSampledFBO();
    void initMultiSampledColorTexture();
    void initMultiSampledDepthRenderBuffer();
    void initRenderTexture();

    void resizeFrameBuffer(const uint32_t t_screenWidth, const uint32_t t_screenHeight);

    static constexpr uint32_t s_defaultWidth = 1600;
    static constexpr uint32_t s_defaultHeight = 900;

    uint32_t m_renderTexture = 0;
    uint32_t m_FBO = 0;
    uint32_t m_textureColorBufferMultiSampled = 0;
    uint32_t m_rboDepth = 0;
    uint32_t m_intermediateFBO = 0;
    uint32_t m_samples = 4;

};


}
