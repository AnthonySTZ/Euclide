#pragma once 

#include <cstdint>
#include <glad/glad.h>
#include <string>

#include <glm/glm.hpp>

namespace butter {

class ShaderProgram {

public:
    ShaderProgram(const std::string &t_vertexFile, const std::string &t_fragmentFile);

    ShaderProgram(const ShaderProgram &) = delete;
    ShaderProgram &operator=(const ShaderProgram &) = delete;

    ShaderProgram(ShaderProgram &&t_other) noexcept;
    ShaderProgram &operator=(ShaderProgram &&t_other) noexcept;

    ~ShaderProgram();

    void use() const;
    void bindUniform(const char* t_name, const glm::mat4x4& t_value);

private:
    static const std::string readFile(const std::string &t_filepath);
    static const uint32_t createShader(const char *t_shaderSrc, GLenum t_type);

    uint32_t m_id;
};

}