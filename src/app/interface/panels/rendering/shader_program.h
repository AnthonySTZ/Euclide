#pragma once 

#include <cstdint>
#include <glad/glad.h>
#include <string>

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

private:
    static const std::string readFile(const std::string &t_filepath);
    static const uint32_t createShader(const char *t_shaderSrc, GLenum t_type);

    uint32_t m_id;
};

}