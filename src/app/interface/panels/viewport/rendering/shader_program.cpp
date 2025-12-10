#include "shader_program.h"

#include "interface/utils/string_utils.h"

#include <iostream>
#include <vector>

#include <glm/gtc/type_ptr.hpp>

namespace euclide {

ShaderProgram::ShaderProgram(const std::string& t_vertexFile, const std::string& t_fragmentFile) {
    const std::string vertCode = readFile(t_vertexFile);
    const std::string fragCode = readFile(t_fragmentFile);

    const uint32_t vertexShader = createShader(vertCode.c_str(), GL_VERTEX_SHADER);
    const uint32_t fragmentShader = createShader(fragCode.c_str(), GL_FRAGMENT_SHADER);

    m_id = glCreateProgram();
    glAttachShader(m_id, vertexShader);
    glAttachShader(m_id, fragmentShader);
    glLinkProgram(m_id);

    int success;
    char infoLog[512];
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_id, 512, 0, infoLog);
        std::cout << infoLog << "\n";
        throw std::runtime_error("Shader program link failed");
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

ShaderProgram::ShaderProgram(ShaderProgram&& t_other) noexcept : m_id(t_other.m_id) {
    t_other.m_id = 0; // Prevent the other shaderProgram to delete the current program
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& t_other) noexcept {
    if (this != &t_other) {
        if (m_id != 0) {
            glDeleteProgram(m_id);
        }

        m_id = t_other.m_id;
        t_other.m_id = 0;
    }
    return *this;
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(m_id);
}

void ShaderProgram::use() const {
    glUseProgram(m_id);
}

void ShaderProgram::bindUniform(const char* t_name, const glm::mat4x4& t_value) {
    GLuint location = glGetUniformLocation(m_id, t_name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(t_value));
}

void ShaderProgram::bindUniform(const char* t_name, const glm::vec3& t_value) {
    GLuint location = glGetUniformLocation(m_id, t_name);
    glUniform3fv(location, 1, glm::value_ptr(t_value));
}

const uint32_t ShaderProgram::createShader(const char* t_shaderSrc, GLenum t_type) {
    const uint32_t shader = glCreateShader(t_type);
    glShaderSource(shader, 1, &t_shaderSrc, 0);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, 0, infoLog);
        std::cout << infoLog << "\n";
        throw std::runtime_error("Shader compilation failed");
    }

    return shader;
}

} // namespace euclide