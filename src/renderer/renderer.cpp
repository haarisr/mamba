#include "renderer.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <array>
#include <cstdint>
#include <numeric>

namespace mamba::Renderer {

constexpr static uint32_t MAX_TEXTURES = 16;

consteval std::array<int32_t, MAX_TEXTURES> getSamplers() {
    std::array<int32_t, MAX_TEXTURES> arr;
    std::iota(arr.begin(), arr.end(), 0);
    return arr;
}

Renderer2D::Renderer2D() {

    // Create shader
    m_shader = Shader::create("src/renderer/shaders/quad.vert", "src/renderer/shaders/quad.frag");

    // Create VAO
    {
        mamba::Renderer::VertexLayout layout = {
            {ShaderDataType::Float4, 0},
            {ShaderDataType::Float2, 1},
            {ShaderDataType::Float4, 2},
            {ShaderDataType::Int1, 3},
        };
        VertexBuffer<QuadVertex> buffer{4};
        m_vbo.emplace(std::move(buffer));
        m_vao.addVertexBuffer(*m_vbo, layout);
    }

    // Create EBO
    {
        const std::array<unsigned int, 6> indices{
            0, 1, 2, // first triangle
            2, 3, 0  // second triangle
        };
        m_ebo.emplace(indices);
        m_vao.addIndexBuffer(*m_ebo);
    }

    {
        m_shader->bind();
        glUniform1iv(1, MAX_TEXTURES, getSamplers().data());
        m_shader->unbind();
    }
}

void Renderer2D::begin(const OrthographicCamera& camera) {
    m_shader->bind();
    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(camera.getViewProjectionMatrix()));
    m_shader->unbind();
}

void Renderer2D::flush() {
    m_shader->bind();
    m_vao.bind();
    glDrawElements(GL_TRIANGLES, m_ebo->count(), GL_UNSIGNED_INT, nullptr);
    m_vao.unbind();
    m_shader->unbind();
}

void Renderer2D::drawQuad(const glm::mat4& transform, const Texture& texture,
                          const glm::vec4& tint_color)

{
    constexpr size_t vertex_count = 4;
    constexpr glm::vec2 texture_coords[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};
    constexpr glm::vec4 quad_vertices[] = {
        {-0.5, -0.5, 0.0, 1.0},
        {0.5, -0.5, 0.0, 1.0},
        {0.5, 0.5, 0.0, 1.0},
        {-0.5, 0.5, 0.0, 1.0},
    };

    const int texture_idx = 0;
    glBindTextureUnit(texture_idx, texture.handle());

    std::array<QuadVertex, 4> vertices;
    for (size_t i = 0; i < vertex_count; i++) {
        QuadVertex vertex{.position = transform * quad_vertices[i],
                          .tex_coords = texture_coords[i],
                          .color = tint_color,
                          .tex_index = texture_idx};
        vertices[i] = vertex;
    }
    m_vbo->update(vertices);

    flush();
}

void Renderer2D::setClearColor(const glm::vec4& color) {
    glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer2D::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
} // namespace mamba::Renderer
