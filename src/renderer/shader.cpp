#include "shader.hpp"

#include <fstream>
#include <iostream>
#include <optional>
#include <string_view>
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

GLuint compileShader(std::string_view source, GLuint shader_type) {

    GLuint shader = glCreateShader(shader_type);
    const char* source_ptr = source.data();
    GLint source_len = static_cast<GLint>(source.size());
    glShaderSource(shader, 1, &source_ptr, &source_len);
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

    return createFromSource(vertex_shader_src.c_str(), fragment_shader_src.c_str());
}

std::optional<Shader> Shader::createFromSource(std::string_view vertex_source,
                                               std::string_view fragment_source) {
    GLuint vertex_shader = compileShader(vertex_source, GL_VERTEX_SHADER);
    GLuint fragment_shader = compileShader(fragment_source, GL_FRAGMENT_SHADER);

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
