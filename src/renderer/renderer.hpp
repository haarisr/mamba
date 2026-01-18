#pragma once

#include "glm/fwd.hpp"
#include "renderer/camera.hpp"
#include "renderer/font.hpp"
#include "renderer/gpu_buffer.hpp"
#include "renderer/shader.hpp"
#include "renderer/texture.hpp"
#include "renderer/vertex_array.hpp"

#include <array>
#include <cstdint>
#include <optional>
#include <string_view>

namespace mamba {
namespace Renderer {

class Renderer2D {

    struct QuadVertex {
        glm::vec4 position;
        glm::vec2 tex_coords;
        glm::vec4 color;
        int tex_index;
    };

    struct TextVertex {
        glm::vec4 position;
        glm::vec2 tex_coords;
        glm::vec4 color;
    };

    struct CircleVertex {
        glm::vec4 world_position;
        glm::vec4 local_position;
        glm::vec4 color;
        float fade;
        float thickness;
    };

    struct CameraData {
        glm::mat4 view_projection;
    };

  public:
    Renderer2D();

    void begin(const OrthographicCamera&);
    void end();
    void clear();
    void setClearColor(const glm::vec4&);
    void setViewPort(uint32_t, uint32_t);
    void drawQuad(const glm::mat4&, const Texture&, const glm::vec4&);
    void drawQuad(const glm::mat4&, const glm::vec4&);
    void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
    void drawQuad(const glm::vec2& position, const glm::vec2& size, const Texture& texture,
                  const glm::vec4& tint);
    void drawText(std::string_view text, const Font& font, const glm::vec2& position, float scale,
                  const glm::vec4& color);
    void drawCircle(const glm::mat4& transform, const glm::vec4& color);

  private:
    int insertTexture(const Texture& texture);
    void startBatch();
    void nextBatch();
    void flush();

  private:
    std::optional<mamba::Renderer::Texture> m_white_texture;
    std::optional<mamba::Renderer::UniformBuffer<CameraData>> m_ubo;

    // Quad rendering
    std::optional<mamba::Renderer::Shader> m_shader;
    std::optional<mamba::Renderer::IndexBuffer<std::uint32_t>> m_ebo;
    std::optional<mamba::Renderer::VertexBuffer<QuadVertex>> m_vbo;
    mamba::Renderer::VertexArray m_vao;
    std::vector<QuadVertex> m_quad_vertices;
    std::array<GLuint, 16> m_texture_slots;
    size_t m_texture_idx;

    // Text rendering
    std::optional<mamba::Renderer::Shader> m_text_shader;
    std::optional<mamba::Renderer::VertexBuffer<TextVertex>> m_text_vbo;
    mamba::Renderer::VertexArray m_text_vao;
    std::vector<TextVertex> m_text_vertices;
    GLuint m_text_texture_slot;

    // Circle rendering
    std::optional<mamba::Renderer::Shader> m_circle_shader;
    std::optional<mamba::Renderer::VertexBuffer<CircleVertex>> m_circle_vbo;
    mamba::Renderer::VertexArray m_circle_vao;
    std::vector<CircleVertex> m_circle_vertices;
};

} // namespace Renderer
} // namespace mamba
