#pragma once

#include <filesystem>
#include <glad/glad.h>
#include <optional>

namespace mamba {

namespace Renderer {

struct Shader {
    ~Shader() { glDeleteProgram(m_program); }
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&&) noexcept;
    Shader& operator=(Shader&&) noexcept;

    void bind();
    void unbind();

    static std::optional<Shader> create(const std::filesystem::path& vertex_path,
                                        const std::filesystem::path& fragment_path);

  private:
    Shader(GLuint program) : m_program(program) {};

  private:
    GLuint m_program{0};
};

} // namespace Renderer

} // namespace mamba
