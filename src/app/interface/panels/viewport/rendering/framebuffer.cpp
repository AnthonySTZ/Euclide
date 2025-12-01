#include "framebuffer.h"

#include <iostream>

namespace euclide {

FrameBuffer::FrameBuffer() {
    initMultiSampledFBO();
    initMultiSampledColorTexture();
    initMultiSampledDepthRenderBuffer();
    initRenderTexture();
}

FrameBuffer::~FrameBuffer() noexcept {
    if (m_renderTexture) {
        glDeleteTextures(1, &m_renderTexture);
    }
    if (m_textureColorBufferMultiSampled) {
        glDeleteTextures(1, &m_textureColorBufferMultiSampled);
    }
    if (m_rboDepth) {
        glDeleteRenderbuffers(1, &m_rboDepth);
    }
    if (m_FBO) {
        glDeleteFramebuffers(1, &m_FBO);
    }
    if (m_intermediateFBO) {
        glDeleteFramebuffers(1, &m_intermediateFBO);
    }
}

FrameBuffer::FrameBuffer(FrameBuffer&& t_other) noexcept
    : m_renderTexture(t_other.m_renderTexture), m_FBO(t_other.m_FBO),
      m_textureColorBufferMultiSampled(t_other.m_textureColorBufferMultiSampled), m_rboDepth(t_other.m_rboDepth),
      m_intermediateFBO(t_other.m_intermediateFBO), m_samples(t_other.m_samples) {
    t_other.m_renderTexture = 0;
    t_other.m_FBO = 0;
    t_other.m_textureColorBufferMultiSampled = 0;
    t_other.m_rboDepth = 0;
    t_other.m_intermediateFBO = 0;
}

FrameBuffer& FrameBuffer::operator=(FrameBuffer&& t_other) noexcept {
    if (this != &t_other) {
        if (m_renderTexture)
            glDeleteTextures(1, &m_renderTexture);
        if (m_textureColorBufferMultiSampled)
            glDeleteTextures(1, &m_textureColorBufferMultiSampled);
        if (m_rboDepth)
            glDeleteRenderbuffers(1, &m_rboDepth);
        if (m_FBO)
            glDeleteFramebuffers(1, &m_FBO);
        if (m_intermediateFBO)
            glDeleteFramebuffers(1, &m_intermediateFBO);

        m_renderTexture = t_other.m_renderTexture;
        m_FBO = t_other.m_FBO;
        m_textureColorBufferMultiSampled = t_other.m_textureColorBufferMultiSampled;
        m_rboDepth = t_other.m_rboDepth;
        m_intermediateFBO = t_other.m_intermediateFBO;
        m_samples = t_other.m_samples;

        t_other.m_renderTexture = 0;
        t_other.m_FBO = 0;
        t_other.m_textureColorBufferMultiSampled = 0;
        t_other.m_rboDepth = 0;
        t_other.m_intermediateFBO = 0;
    }
    return *this;
}

uint32_t FrameBuffer::getRenderTexture() const noexcept {
    return m_renderTexture;
}

void FrameBuffer::bind() const noexcept {
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}

void FrameBuffer::unbind() const noexcept {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::blit(const uint32_t t_width, const uint32_t t_height) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_intermediateFBO);
    glBlitFramebuffer(0, 0, t_width, t_height, 0, 0, t_width, t_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void FrameBuffer::resize(const uint32_t t_screenWidth, const uint32_t t_screenHeight) {
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_textureColorBufferMultiSampled);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples, GL_RGB, t_screenWidth, t_screenHeight, GL_TRUE);

    glBindTexture(GL_TEXTURE_2D, m_renderTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t_screenWidth, t_screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glBindRenderbuffer(GL_RENDERBUFFER, m_rboDepth);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_samples, GL_DEPTH_COMPONENT, t_screenWidth, t_screenHeight);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::resizeFrameBuffer(const uint32_t t_width, const uint32_t t_height) {
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_textureColorBufferMultiSampled);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples, GL_RGB, t_width, t_height, GL_TRUE);

    glBindTexture(GL_TEXTURE_2D, m_renderTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t_width, t_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glBindRenderbuffer(GL_RENDERBUFFER, m_rboDepth);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_samples, GL_DEPTH_COMPONENT, t_width, t_height);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::initMultiSampledFBO() {
    glGenFramebuffers(1, &m_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}

void FrameBuffer::initMultiSampledColorTexture() {
    glGenTextures(1, &m_textureColorBufferMultiSampled);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_textureColorBufferMultiSampled);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples, GL_RGB, DEFAULT_WIDTH, DEFAULT_HEIGHT, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE,
                           m_textureColorBufferMultiSampled, 0);
}

void FrameBuffer::initMultiSampledDepthRenderBuffer() {
    // Create multisampled depth renderbuffer (easier for depth)
    glGenRenderbuffers(1, &m_rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rboDepth);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_samples, GL_DEPTH_COMPONENT, DEFAULT_WIDTH, DEFAULT_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rboDepth);
}

void FrameBuffer::initRenderTexture() {
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Multisampled framebuffer not complete!");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenFramebuffers(1, &m_intermediateFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_intermediateFBO);

    glGenTextures(1, &m_renderTexture);
    glBindTexture(GL_TEXTURE_2D, m_renderTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, DEFAULT_WIDTH, DEFAULT_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_renderTexture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Intermediate framebuffer not complete!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

} // namespace euclide