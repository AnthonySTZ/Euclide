#pragma once

#include <cstdint>
#include <glad/glad.h>
#include <string>

#include <glm/glm.hpp>

namespace euclide {

/// @brief Encapsulates an OpenGL shader program with vertex and fragment shaders.
class ShaderProgram {
  public:
    /// @brief Construct a shader program from vertex and fragment shader files.
    /// @param t_vertexFile   Path to the vertex shader source file.
    /// @param t_fragmentFile Path to the fragment shader source file.
    ShaderProgram(const std::string& t_vertexFile, const std::string& t_fragmentFile);

    // Disable copy semantics
    ShaderProgram(const ShaderProgram&) = delete;            ///< No copy allowed.
    ShaderProgram& operator=(const ShaderProgram&) = delete; ///< No copy assignment.

    // Enable move semantics
    ShaderProgram(ShaderProgram&& t_other) noexcept;            ///< Move constructor.
    ShaderProgram& operator=(ShaderProgram&& t_other) noexcept; ///< Move assignment.

    /// @brief Destroy the shader program and release OpenGL resources.
    ~ShaderProgram();

    /// @brief Activate this shader program for rendering.
    void use() const;

    /// @brief Bind a 4x4 matrix uniform to the shader program.
    /// @param t_name  Name of the uniform variable in the shader.
    /// @param t_value Value of the matrix to bind.
    void bindUniform(const char* t_name, const glm::mat4x4& t_value);

  private:
    /// @brief Compile a shader from source code.
    /// @param t_shaderSrc Shader source code as a C-string.
    /// @param t_type      Type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER).
    /// @return OpenGL shader ID.
    static const uint32_t createShader(const char* t_shaderSrc, GLenum t_type);

  private:
    uint32_t m_id; ///< OpenGL shader program ID.
};

} // namespace euclide
