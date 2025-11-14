#pragma once

#include "interface/utils/imgui_utils.h"
#include "framebuffer.h"
#include "shader_program.h"
#include "camera.h"

#include "render_model.h"

#include <cstdint>
#include <memory>

namespace butter {

class Renderer {

public:
    Renderer();
    ~Renderer() = default;

    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;

    Renderer(Renderer &&) noexcept = default;
    Renderer &operator=(Renderer &&) noexcept = default;

    void draw(const uint32_t t_width, const uint32_t t_height);
    ImTextureID getRenderTexture() const {
        return (ImTextureID)(intptr_t)m_frameBuffer.getRenderTexture();
    }

    void resizeFrameBuffer(const uint32_t t_screenWidth, const uint32_t t_screenHeight);

    void updateMesh(std::shared_ptr<Mesh> t_mesh);
    void setCamera(std::shared_ptr<Camera> t_camera);

    void tooglePrimitives() noexcept { m_showPrimitives = !m_showPrimitives; }
    void toogleWireframe() noexcept { m_showWireframe = !m_showWireframe; }
    void tooglePoints() noexcept { m_showPoints = !m_showPoints; }

private:
    void beginFrame(const uint32_t t_screenWidth, const uint32_t t_screenHeight);
    void endFrame(const uint32_t t_screenWidth, const uint32_t t_screenHeight);
    
    void bindCameraUniforms(ShaderProgram& t_shaderProgram);

    void clearFrame() const noexcept;

    
    float getAspectRatio(const uint32_t t_screenWidth, const uint32_t t_screenHeight) const;

    static constexpr struct { float r, g, b, a; } s_bgColor{0.3f, 0.3f, 0.3f, 1.0f};

    const float m_edgesLineWidth = 1.0f;

    ShaderProgram m_faceShaderProgram;
    ShaderProgram m_pointShaderProgram;
    ShaderProgram m_edgeShaderProgram;

    FrameBuffer m_frameBuffer{};
    std::weak_ptr<Camera> m_camera;

    RenderModel m_model;

    bool m_showPrimitives = true;
    bool m_showWireframe = true;
    bool m_showPoints = true;
};

}