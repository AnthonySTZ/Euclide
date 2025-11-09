#pragma once

#include "interface/utils/imgui_utils.h"
#include "framebuffer.h"
#include "shader_program.h"

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

    void useShader(const std::string &t_vertexFile, const std::string &t_fragmentFile);

    void draw(const uint32_t t_width, const uint32_t t_height);
    ImTextureID getRenderTexture() const {
        return (ImTextureID)(intptr_t)m_frameBuffer->getRenderTexture();
    }

    void resizeFrameBuffer(const uint32_t t_screenWidth, const uint32_t t_screenHeight);

private:
    void beginFrame(const uint32_t t_screenWidth, const uint32_t t_screenHeight);
    void endFrame(const uint32_t t_screenWidth, const uint32_t t_screenHeight);
    
    void clearFrame() const noexcept;
    
    float getAspectRatio(const uint32_t t_screenWidth, const uint32_t t_screenHeight) const;

    static constexpr struct { float r, g, b, a; } s_bgColor{0.3f, 0.3f, 0.3f, 1.0f};

    const float m_edgesLineWidth = 1.0f;

    std::unique_ptr<ShaderProgram> m_faceShaderProgram;
    std::unique_ptr<ShaderProgram> m_pointShaderProgram;
    std::unique_ptr<ShaderProgram> m_edgeShaderProgram;

    std::unique_ptr<FrameBuffer> m_frameBuffer;
};

}