#pragma once

#include "glad/glad.h"
#include <span>

namespace mamba {
namespace Renderer {

class VertexBuffer {

  public:
    template <typename T, std::size_t Extent> VertexBuffer(std::span<const T, Extent> indices) {
        glCreateBuffers(1, &m_handle);
        glNamedBufferStorage(m_handle, indices.size_bytes(), indices.data(), 0);
    }
    ~VertexBuffer();

    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;
    VertexBuffer(VertexBuffer&&) noexcept;
    VertexBuffer& operator=(VertexBuffer&&) noexcept;

    GLuint handle() const { return m_handle; }

  private:
    GLuint m_handle{0};
};

} // namespace Renderer
} // namespace mamba
