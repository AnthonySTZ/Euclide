#pragma once

#include <cstdint>
#include <glad/glad.h>

namespace butter {

/// @brief Encapsulates an OpenGL framebuffer with multisample support.
class FrameBuffer {
  public:
    /// @brief Construct a framebuffer with default settings.
    FrameBuffer();

    /// @brief Destroy the framebuffer and release OpenGL resources.
    ~FrameBuffer() noexcept;

    // Disable copy semantics
    FrameBuffer(const FrameBuffer&) = delete;            ///< Copy constructor disabled.
    FrameBuffer& operator=(const FrameBuffer&) = delete; ///< Copy assignment disabled.

    // Enable move semantics
    FrameBuffer(FrameBuffer&& t_other) noexcept;            ///< Move constructor.
    FrameBuffer& operator=(FrameBuffer&& t_other) noexcept; ///< Move assignment.

    /// @brief Get the OpenGL texture ID of the render target.
    /// @return OpenGL texture ID.
    [[nodiscard]] uint32_t getRenderTexture() const noexcept;

    /// @brief Blit the framebuffer content to the default framebuffer or another target.
    /// @param t_width  Width of the target framebuffer.
    /// @param t_height Height of the target framebuffer.
    void blit(const uint32_t t_width, const uint32_t t_height);

    /// @brief Resize the framebuffer and its attachments.
    /// @param t_screenWidth  New width.
    /// @param t_screenHeight New height.
    void resize(const uint32_t t_screenWidth, const uint32_t t_screenHeight);

    /// @brief Bind this framebuffer for rendering.
    void bind() const noexcept;

    /// @brief Unbind the framebuffer and revert to the default framebuffer.
    void unbind() const noexcept;

  private:
    /// @brief Initialize the multisampled framebuffer object (FBO).
    void initMultiSampledFBO();

    /// @brief Initialize the multisampled color texture attachment.
    void initMultiSampledColorTexture();

    /// @brief Initialize the multisampled depth renderbuffer.
    void initMultiSampledDepthRenderBuffer();

    /// @brief Initialize the single-sample render texture.
    void initRenderTexture();

    /// @brief Resize the framebuffer and all attachments internally.
    /// @param t_screenWidth  New width of the framebuffer.
    /// @param t_screenHeight New height of the framebuffer.
    void resizeFrameBuffer(const uint32_t t_screenWidth, const uint32_t t_screenHeight);

  private:
    uint32_t m_renderTexture = 0;                  ///< Single-sample render texture.
    uint32_t m_FBO = 0;                            ///< Multisampled framebuffer object ID.
    uint32_t m_textureColorBufferMultiSampled = 0; ///< Multisample color attachment.
    uint32_t m_rboDepth = 0;                       ///< Depth renderbuffer ID.
    uint32_t m_intermediateFBO = 0;                ///< Intermediate framebuffer for blitting.
    uint32_t m_samples = 4;                        ///< Number of samples for multisampling.

    static constexpr uint32_t DEFAULT_WIDTH = 1600; ///< Default framebuffer width.
    static constexpr uint32_t DEFAULT_HEIGHT = 900; ///< Default framebuffer height.
};

} // namespace butter
