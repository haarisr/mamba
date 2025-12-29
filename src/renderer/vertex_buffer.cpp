#include "vertex_buffer.hpp"
namespace mamba::Renderer {

VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &m_handle); }

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
    : m_handle(std::exchange(other.m_handle, 0)) {}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept {
    std::swap(m_handle, other.m_handle);
    return *this;
}

} // namespace mamba::Renderer
