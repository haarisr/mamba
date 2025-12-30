#pragma once

#include "glm/fwd.hpp"
#include "renderer/camera.hpp"
#include "renderer/index_buffer.hpp"
#include "renderer/shader.hpp"
#include "renderer/texture.hpp"
#include "renderer/vertex_array.hpp"
#include "renderer/vertex_buffer.hpp"
#include <optional>

namespace mamba {
namespace Renderer {

class Renderer2D {

    struct QuadVertex {
        glm::vec4 position;
        glm::vec2 tex_coords;
        glm::vec4 color;
        int tex_index;
    };

  public:
    Renderer2D();

    void begin(const OrthographicCamera&);
    void clear();
    void setClearColor(const glm::vec4&);
    void drawQuad(const glm::mat4&, const Texture&, const glm::vec4&);
    void flush();

  private:
    std::optional<mamba::Renderer::Shader> m_shader;
    std::optional<mamba::Renderer::IndexBuffer> m_ebo;
    std::optional<mamba::Renderer::VertexBuffer<QuadVertex>> m_vbo;
    mamba::Renderer::VertexArray m_vao;
};

} // namespace Renderer
} // namespace mamba
