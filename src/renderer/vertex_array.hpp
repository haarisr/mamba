#pragma once

#include "renderer/index_buffer.hpp"
#include "renderer/vertex_buffer.hpp"

#include <glad/glad.h>
#include <vector>

namespace mamba {
namespace Renderer {

enum class ShaderDataType { Float1, Float2, Float3, Float4 };
using VertexLayout = std::vector<std::pair<ShaderDataType, uint32_t>>;

class VertexArray {

  public:
    VertexArray();
    ~VertexArray();

    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;
    VertexArray(VertexArray&&) noexcept;
    VertexArray& operator=(VertexArray&&) noexcept;

    void bind();
    void unbind();

    void addVertexBuffer(const VertexBuffer&, const VertexLayout&);
    void addIndexBuffer(const IndexBuffer&);

  private:
    GLuint m_handle{0};
};

} // namespace Renderer
} // namespace mamba
