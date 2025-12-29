#include "index_buffer.hpp"
#include <utility>

namespace mamba::Renderer {

IndexBuffer::IndexBuffer(std::span<const unsigned int> indices)
    : m_count(static_cast<std::uint32_t>(indices.size())) {
    glCreateBuffers(1, &m_handle);
    glNamedBufferStorage(m_handle, indices.size_bytes(), indices.data(), 0);
}
IndexBuffer::~IndexBuffer() { glDeleteBuffers(1, &m_handle); }

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
    : m_handle(std::exchange(other.m_handle, 0)), m_count(std::exchange(other.m_count, 0)) {}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept {
    std::swap(m_handle, other.m_handle);
    std::swap(m_count, other.m_count);
    return *this;
}

} // namespace mamba::Renderer
