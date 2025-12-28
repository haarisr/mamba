#include "shader.hpp"

#include <fstream>
#include <iostream>
#include <optional>
#include <utility>
#include <vector>

#include <glad/glad.h>

namespace {
std::string readFile(const std::filesystem::path& path) {
    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << path.string() << std::endl;
        return {};
    }

    std::ostringstream stream;
    stream << file.rdbuf();
    return stream.str();
}

GLuint compileShader(const char* source, GLuint shader_type) {

    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        GLint length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        std::vector<GLchar> log(length);
        glGetShaderInfoLog(shader, length, &length, log.data());

        std::cerr << log.data() << std::endl;
        glDeleteShader(shader);
    }
    return shader;
}
} // namespace

namespace mamba {

namespace Renderer {

Shader::Shader(Shader&& other) noexcept : m_program(std::exchange(other.m_program, 0)) {}

Shader& Shader::operator=(Shader&& other) noexcept {
    std::swap(m_program, other.m_program);
    return *this;
}

void Shader::bind() { glUseProgram(m_program); }
void Shader::unbind() { glUseProgram(0); }

std::optional<Shader> Shader::create(const std::filesystem::path& vertex_path,
                                     const std::filesystem::path& fragment_path) {
    std::string vertex_shader_src = readFile(vertex_path);
    std::string fragment_shader_src = readFile(fragment_path);

    GLuint vertex_shader = compileShader(vertex_shader_src.c_str(), GL_VERTEX_SHADER);
    GLuint fragment_shader = compileShader(fragment_shader_src.c_str(), GL_FRAGMENT_SHADER);

    // Program linking
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        GLint length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

        std::vector<GLchar> log(length);
        glGetProgramInfoLog(program, length, &length, &log[0]);

        std::cerr << log.data() << std::endl;

        glDeleteProgram(program);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return Shader(program);
}

} // namespace Renderer
} // namespace mamba
