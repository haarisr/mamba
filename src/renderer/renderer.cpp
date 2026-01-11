#include "renderer.hpp"
#include "shaders/shaders.hpp"

#include "glm/gtc/type_ptr.hpp"
#include "renderer/font.hpp"
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

    m_shader = Shader::createFromSource(Shaders::QUAD_VERT, Shaders::QUAD_FRAG);
    m_text_shader = Shader::createFromSource(Shaders::TEXT_VERT, Shaders::TEXT_FRAG);
    m_circle_shader = Shader::createFromSource(Shaders::CIRCLE_VERT, Shaders::CIRCLE_FRAG);
    m_quad_vertices.reserve(MAX_VERTICES);
    m_text_vertices.reserve(MAX_VERTICES);
    m_circle_vertices.reserve(MAX_VERTICES);

    // Create shared EBO
    m_ebo.emplace(getIndices());

    // Create quad VAO
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
        m_vao.addIndexBuffer(*m_ebo);
    }

    // Create text VAO
    {
        mamba::Renderer::VertexLayout layout = {
            {ShaderDataType::Float4, 0},
            {ShaderDataType::Float2, 1},
            {ShaderDataType::Float4, 2},
        };
        VertexBuffer<TextVertex> buffer{MAX_VERTICES};
        m_text_vbo.emplace(std::move(buffer));
        m_text_vao.addVertexBuffer(*m_text_vbo, layout);
        m_text_vao.addIndexBuffer(*m_ebo);
    }

    {
        mamba::Renderer::VertexLayout layout = {
            {ShaderDataType::Float4, 0}, {ShaderDataType::Float4, 1}, {ShaderDataType::Float4, 2},
            {ShaderDataType::Float1, 3}, {ShaderDataType::Float1, 4},
        };
        VertexBuffer<CircleVertex> buffer{MAX_VERTICES};
        m_circle_vbo.emplace(std::move(buffer));
        m_circle_vao.addVertexBuffer(*m_circle_vbo, layout);
        m_circle_vao.addIndexBuffer(*m_ebo);
    }

    {
        m_shader->bind();
        glUniform1iv(1, MAX_TEXTURES, getSamplers().data());
        m_shader->unbind();
    }

    {
        m_text_shader->bind();
        glUniform1i(1, 0);
        m_text_shader->unbind();
    }

    m_white_texture = Texture::createWhite();
}

void Renderer2D::begin(const OrthographicCamera& camera) {
    const auto& vp = camera.getViewProjectionMatrix();

    m_shader->bind();
    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(vp));
    m_shader->unbind();

    m_text_shader->bind();
    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(vp));
    m_text_shader->unbind();

    m_circle_shader->bind();
    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(vp));
    m_circle_shader->unbind();

    startBatch();
}

void Renderer2D::end() { flush(); }

void Renderer2D::flush() {

    {
        // if (m_quad_vertices.empty())
        //     return;

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

    {
        // if (m_text_vertices.empty())
        //     return;

        m_text_shader->bind();
        m_text_vao.bind();

        glBindTextureUnit(0, m_text_texture_slot);

        m_text_vbo->update(m_text_vertices);
        auto indices_count = m_text_vertices.size() / 4 * 6;

        glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, nullptr);
        m_text_vao.unbind();
        m_text_shader->unbind();
    }

    {
        // if (m_text_vertices.empty())
        //     return;

        m_circle_shader->bind();
        m_circle_vao.bind();

        m_circle_vbo->update(m_circle_vertices);
        auto indices_count = m_circle_vertices.size() / 4 * 6;

        glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, nullptr);
        m_circle_vao.unbind();
        m_circle_shader->unbind();
    }
}
void Renderer2D::drawCircle(const glm::mat4& transform, const glm::vec4& color) {

    if (m_circle_vertices.size() >= MAX_VERTICES) {
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
    float fade = 0.005;
    float thickness = 1.0f;

    for (size_t i = 0; i < vertex_count; i++) {
        CircleVertex vertex{
            .world_position = transform * quad_vertices[i],
            .local_position = quad_vertices[i] * 2.0f,
            .color = color,
            .fade = fade,
            .thickness = thickness,
        };
        m_circle_vertices.emplace_back(vertex);
    }
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
    drawQuad(transform, *m_white_texture, color);
}

void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size,
                          const glm::vec4& color) {
    glm::mat4 transform(1.0f);
    transform = glm::translate(transform, glm::vec3(position, 0.0f));
    transform = glm::scale(transform, glm::vec3(size, 1.0f));
    drawQuad(transform, color);
}

void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const Texture& texture,
                          const glm::vec4& tint) {
    glm::mat4 transform(1.0f);
    transform = glm::translate(transform, glm::vec3(position, 0.0f));
    transform = glm::scale(transform, glm::vec3(size, 1.0f));
    drawQuad(transform, texture, tint);
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

    m_text_vertices.clear();
    m_circle_vertices.clear();
}

void Renderer2D::nextBatch() {
    flush();
    startBatch();
}

void Renderer2D::drawText(std::string_view text, const Font& font, const glm::vec2& position,
                          float scale, const glm::vec4& color) {
    if (m_text_vertices.size() >= MAX_VERTICES) {
        nextBatch();
    }
    const auto& atlas = font.getAtlasTexture();
    m_text_texture_slot = atlas.handle();

    const auto& geometry = font.getFontGeometry();
    const auto& metrics = geometry.getMetrics();

    double x = position.x;
    double y = position.y;

    double fsScale = scale / metrics.lineHeight;

    for (auto c : text) {
        if (c == '\n') {
            x = position.x;
            y -= metrics.lineHeight * fsScale;
            continue;
        }

        auto glyph = geometry.getGlyph(c);
        if (!glyph)
            continue;

        // Get glyph bounds in atlas (normalized 0-1)
        double al, ab, ar, at; // atlas left, bottom, right, top
        glyph->getQuadAtlasBounds(al, ab, ar, at);

        // Get glyph bounds in output (em units)
        double pl, pb, pr, pt; // plane left, bottom, right, top
        glyph->getQuadPlaneBounds(pl, pb, pr, pt);

        // Convert to screen coordinates
        float x0 = static_cast<float>(x + pl * fsScale);
        float y0 = static_cast<float>(y + pb * fsScale);
        float x1 = static_cast<float>(x + pr * fsScale);
        float y1 = static_cast<float>(y + pt * fsScale);

        // Normalize atlas coords
        float u0 = static_cast<float>(al / atlas.width());
        float v0 = static_cast<float>(ab / atlas.height());
        float u1 = static_cast<float>(ar / atlas.width());
        float v1 = static_cast<float>(at / atlas.height());

        // Create quad (4 vertices)
        m_text_vertices.push_back({{x0, y0, 0.0f, 1.0f}, {u0, v0}, color});
        m_text_vertices.push_back({{x1, y0, 0.0f, 1.0f}, {u1, v0}, color});
        m_text_vertices.push_back({{x1, y1, 0.0f, 1.0f}, {u1, v1}, color});
        m_text_vertices.push_back({{x0, y1, 0.0f, 1.0f}, {u0, v1}, color});

        // Advance cursor
        double advance = glyph->getAdvance();
        x += advance * fsScale;
    }
}
} // namespace mamba::Renderer
