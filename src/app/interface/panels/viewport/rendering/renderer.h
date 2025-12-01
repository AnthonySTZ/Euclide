#pragma once

#include "interface/utils/imgui_utils.h"
#include "framebuffer.h"
#include "shader_program.h"
#include "camera.h"
#include "render_model.h"

#include <cstdint>
#include <memory>

namespace butter {

/// @brief Responsible for rendering a RenderModel using OpenGL, managing shaders, camera, and framebuffer.
class Renderer {
  public:
    /// @brief Construct the renderer and initialize default shaders and OpenGL state.
    Renderer();

    /// @brief Default destructor.
    ~Renderer() = default;

    // Disable copy semantics
    Renderer(const Renderer&) = delete;            ///< No copy allowed.
    Renderer& operator=(const Renderer&) = delete; ///< No copy assignment.

    // Enable move semantics
    Renderer(Renderer&&) noexcept = default;            ///< Move constructor.
    Renderer& operator=(Renderer&&) noexcept = default; ///< Move assignment.

    /// @brief Render a RenderModel according to its current draw toggles (primitives, edges, points).
    /// @param t_model The render model to draw.
    void draw(const RenderModel& t_model);

    /// @brief Get the ImGui texture handle corresponding to the framebuffer render target.
    /// @return ImTextureID for use in ImGui.
    [[nodiscard]] ImTextureID getRenderTexture() const {
        return (ImTextureID)(intptr_t)m_frameBuffer.getRenderTexture();
    }

    /// @brief Resize the internal framebuffer and update the camera aspect ratio.
    /// @param t_screenWidth  New framebuffer width.
    /// @param t_screenHeight New framebuffer height.
    void resizeFrameBuffer(const uint32_t t_screenWidth, const uint32_t t_screenHeight);

    /// @brief Assign the camera used for rendering.
    /// @param t_camera Shared pointer to a Camera object.
    void setCamera(std::shared_ptr<Camera> t_camera);

    /// @brief Prepare the framebuffer and viewport for rendering.
    /// @param t_screenWidth  Width of the viewport.
    /// @param t_screenHeight Height of the viewport.
    void beginFrame(const uint32_t t_screenWidth, const uint32_t t_screenHeight);

    /// @brief Finish rendering and blit the framebuffer content to the screen.
    /// @param t_screenWidth  Width of the framebuffer.
    /// @param t_screenHeight Height of the framebuffer.
    void endFrame(const uint32_t t_screenWidth, const uint32_t t_screenHeight);

    /// @brief Clear the current framebuffer using the background color.
    void clearFrame() const noexcept;

  private:
    /// @brief Upload camera projection and view matrices to the given shader.
    /// @param t_shaderProgram Shader program to bind camera uniforms to.
    void bindCameraUniforms(ShaderProgram& t_shaderProgram);

    /// @brief Compute the aspect ratio based on screen dimensions.
    /// @param t_screenWidth  Width of the screen.
    /// @param t_screenHeight Height of the screen.
    /// @return Aspect ratio as width / height.
    [[nodiscard]] float getAspectRatio(const uint32_t t_screenWidth, const uint32_t t_screenHeight) const;

  private:
    const float m_edgesLineWidth = 1.0f; ///< Line width for edge rendering.

    ShaderProgram m_faceShaderProgram;  ///< Shader for rendering filled triangles.
    ShaderProgram m_pointShaderProgram; ///< Shader for rendering points.
    ShaderProgram m_edgeShaderProgram;  ///< Shader for rendering edges (wireframe).

    FrameBuffer m_frameBuffer{};    ///< Offscreen framebuffer for rendering.
    std::weak_ptr<Camera> m_camera; ///< Camera used for view/projection matrices.

    static constexpr struct { float r, g, b, a; } BG_COLOR{0.3f, 0.3f, 0.3f, 1.0f}; ///< Default background color.
};

} // namespace butter
