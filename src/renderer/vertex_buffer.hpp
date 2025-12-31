#pragma once

#include "glad/glad.h"
#include <span>
#include <utility>

namespace mamba {
namespace Renderer {

template <typename T> class VertexBuffer {

  public:
    VertexBuffer(size_t max_count) {
        glCreateBuffers(1, &m_handle);
        glNamedBufferStorage(m_handle, stride() * max_count, nullptr, GL_DYNAMIC_STORAGE_BIT);
    }
    ~VertexBuffer() { glDeleteBuffers(1, &m_handle); }

    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    VertexBuffer(VertexBuffer&& other) noexcept : m_handle(std::exchange(other.m_handle, 0)) {}
    VertexBuffer& operator=(VertexBuffer&& other) noexcept {
        std::swap(m_handle, other.m_handle);
        return *this;
    }

    GLuint handle() const { return m_handle; }

    void update(std::span<const T> vertices) {
        glNamedBufferSubData(m_handle, 0, vertices.size_bytes(), vertices.data());
    }

    static consteval size_t stride() { return sizeof(T); }

  private:
    GLuint m_handle{0};
};

} // namespace Renderer
} // namespace mamba
