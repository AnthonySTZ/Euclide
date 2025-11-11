#include "Renderer.h"

#include <iostream>

namespace butter {

Renderer::Renderer()
    : m_faceShaderProgram(ShaderProgram("shaders/faceStandard.vert", "shaders/faceStandard.frag")),
      m_pointShaderProgram(ShaderProgram("shaders/pointStandard.vert", "shaders/pointStandard.frag")),
      m_edgeShaderProgram(ShaderProgram("shaders/edgeStandard.vert", "shaders/edgeStandard.frag")) {
    glEnable(GL_PROGRAM_POINT_SIZE);
    glLineWidth(m_edgesLineWidth);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}

void Renderer::draw(const uint32_t t_screenWidth, const uint32_t t_screenHeight) {
    beginFrame(t_screenWidth, t_screenHeight);

    m_faceShaderProgram.use();
    bindCameraUniforms(m_faceShaderProgram);
    m_model.draw();

    m_edgeShaderProgram.use();
    bindCameraUniforms(m_edgeShaderProgram);
    m_model.drawEdges();
    
    m_pointShaderProgram.use();
    bindCameraUniforms(m_pointShaderProgram);
    m_model.drawPoints();

    clearFrame();
    endFrame(t_screenWidth, t_screenHeight);
}

float Renderer::getAspectRatio(const uint32_t t_screenWidth, const uint32_t t_screenHeight) const {
    if (t_screenHeight == 0) {
        return 1.0f;
    }
    return static_cast<float>(t_screenWidth) / static_cast<float>(t_screenHeight);
};

void Renderer::resizeFrameBuffer(const uint32_t t_screenWidth, const uint32_t t_screenHeight) {
    m_frameBuffer.resize(t_screenWidth, t_screenHeight);
    if (auto camera = m_camera.lock()) {
        camera->updateAspectRatio(getAspectRatio(t_screenWidth, t_screenHeight));
    }
}

void Renderer::updateMesh(std::shared_ptr<Mesh> t_mesh)
{
    m_model.updateWithMesh(t_mesh);
}

void Renderer::setCamera(std::shared_ptr<Camera> t_camera)
{
    m_camera = t_camera;
}

void Renderer::beginFrame(const uint32_t t_screenWidth, const uint32_t t_screenHeight) {
    m_frameBuffer.bind();
    glViewport(0, 0, t_screenWidth, t_screenHeight);
}

void Renderer::endFrame(const uint32_t t_screenWidth, const uint32_t t_screenHeight) {
    glUseProgram(0);
    m_frameBuffer.blit(t_screenWidth, t_screenHeight);
    m_frameBuffer.unbind();
}

void Renderer::bindCameraUniforms(ShaderProgram &t_shaderProgram)
{
    if (const auto camera = m_camera.lock()) {
        t_shaderProgram.bindUniform("projection", camera->getProjection());
        t_shaderProgram.bindUniform("view", camera->getView());
    }
}

void Renderer::clearFrame() const noexcept {
    glClearColor(s_bgColor.r, s_bgColor.g, s_bgColor.b, s_bgColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

}