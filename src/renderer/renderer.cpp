#include "renderer.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "renderer/texture.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <numeric>

namespace mamba::Renderer {

constexpr static uint32_t MAX_TEXTURES = 16;
constexpr static uint32_t MAX_QUADS = 20000;
constexpr static const uint32_t MAX_VERTICES = MAX_QUADS * 4;
constexpr static const uint32_t MAX_INDICES = MAX_QUADS * 6;

consteval std::array<int32_t, MAX_TEXTURES> getSamplers() {
    std::array<int32_t, MAX_TEXTURES> arr;
    std::iota(arr.begin(), arr.end(), 0);
    return arr;
}

consteval std::array<uint32_t, MAX_INDICES> getIndices() {
    std::array<uint32_t, MAX_INDICES> arr;
    for (size_t idx = 0, vtx = 0; idx < MAX_INDICES; idx += 6, vtx += 4) {
        arr[idx] = vtx + 0;
        arr[idx + 1] = vtx + 1;
        arr[idx + 2] = vtx + 2;
        arr[idx + 3] = vtx + 2;
        arr[idx + 4] = vtx + 3;
        arr[idx + 5] = vtx + 0;
    }
    return arr;
}

Renderer2D::Renderer2D() {

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Create shader
    m_shader = Shader::create("src/renderer/shaders/quad.vert", "src/renderer/shaders/quad.frag");
    m_quad_vertices.reserve(MAX_VERTICES);

    // Create VAO
    {
        mamba::Renderer::VertexLayout layout = {
            {ShaderDataType::Float4, 0},
            {ShaderDataType::Float2, 1},
            {ShaderDataType::Float4, 2},
            {ShaderDataType::Int1, 3},
        };
        VertexBuffer<QuadVertex> buffer{MAX_VERTICES};
        m_vbo.emplace(std::move(buffer));
        m_vao.addVertexBuffer(*m_vbo, layout);
    }

    // Create EBO
    {
        m_ebo.emplace(getIndices());
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
    startBatch();
}

void Renderer2D::end() { flush(); }

void Renderer2D::flush() {
    if (m_quad_vertices.empty())
        return;

    m_shader->bind();
    m_vao.bind();

    for (size_t i = 0; i < m_texture_idx; i++)
        glBindTextureUnit(i, m_texture_slots[i]);

    m_vbo->update(m_quad_vertices);
    auto indices_count = m_quad_vertices.size() / 4 * 6;

    glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, nullptr);
    m_vao.unbind();
    m_shader->unbind();
}

void Renderer2D::drawQuad(const glm::mat4& transform, const Texture& texture,
                          const glm::vec4& tint_color) {

    if (m_quad_vertices.size() >= MAX_VERTICES) {
        nextBatch();
    }

    constexpr size_t vertex_count = 4;
    constexpr glm::vec2 texture_coords[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};
    constexpr glm::vec4 quad_vertices[] = {
        {-0.5, -0.5, 0.0, 1.0},
        {0.5, -0.5, 0.0, 1.0},
        {0.5, 0.5, 0.0, 1.0},
        {-0.5, 0.5, 0.0, 1.0},
    };

    auto tex_idx = insertTexture(texture);

    for (size_t i = 0; i < vertex_count; i++) {
        QuadVertex vertex{.position = transform * quad_vertices[i],
                          .tex_coords = texture_coords[i],
                          .color = tint_color,
                          .tex_index = tex_idx};
        m_quad_vertices.emplace_back(vertex);
    }
}

void Renderer2D::drawQuad(const glm::mat4& transform, const glm::vec4& color) {
    static auto white = Texture::createWhite();
    drawQuad(transform, white, color);
}

int Renderer2D::insertTexture(const Texture& texture) {

    auto end = m_texture_slots.begin() + m_texture_idx;
    auto iter = std::ranges::find(m_texture_slots.begin(), end, texture.handle());
    if (iter != end) {
        return std::distance(m_texture_slots.begin(), iter);
    }
    if (m_texture_idx >= MAX_TEXTURES) {
        nextBatch();
    }
    m_texture_slots[m_texture_idx] = texture.handle();
    return m_texture_idx++;
}

void Renderer2D::setClearColor(const glm::vec4& color) {
    glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer2D::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void Renderer2D::setViewPort(uint32_t width, uint32_t height) { glViewport(0, 0, width, height); }

void Renderer2D::startBatch() {
    m_quad_vertices.clear();
    m_texture_idx = 0;
}

void Renderer2D::nextBatch() {
    flush();
    startBatch();
}
} // namespace mamba::Renderer
