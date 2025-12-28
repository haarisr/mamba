#include "vertex_array.hpp"
#include "glad/glad.h"
#include <functional>
#include <utility>

namespace mamba {
namespace Renderer {

VertexArray::VertexArray() { glCreateVertexArrays(1, &m_handle); }
VertexArray::~VertexArray() { glDeleteVertexArrays(1, &m_handle); };

VertexArray::VertexArray(VertexArray&& other) noexcept
    : m_handle(std::exchange(other.m_handle, 0)) {}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept {
    std::swap(m_handle, other.m_handle);
    return *this;
}

void VertexArray::bind() { glBindVertexArray(m_handle); }
void VertexArray::unbind() { glBindVertexArray(0); }

void VertexArray::addIndexBuffer(GLuint buffer) { glVertexArrayElementBuffer(m_handle, buffer); }
void VertexArray::addVertexBuffer(const std::function<void(GLuint)> callback) {
    callback(m_handle);
}

} // namespace Renderer
} // namespace mamba
