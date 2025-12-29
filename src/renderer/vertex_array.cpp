#include "vertex_array.hpp"

#include <cstdint>
#include <utility>

#include <glad/glad.h>

namespace mamba::Renderer {

namespace {

constexpr std::uint32_t componentCount(ShaderDataType type) {

    switch (type) {
    case ShaderDataType::Float1:
        return 1;
    case ShaderDataType::Float2:
        return 2;
    case ShaderDataType::Float3:
        return 3;
    case ShaderDataType::Float4:
        return 4;
    }
    std::unreachable();
}

constexpr std::uint32_t sizeOf(ShaderDataType type) {

    switch (type) {
    case ShaderDataType::Float1:
    case ShaderDataType::Float2:
    case ShaderDataType::Float3:
    case ShaderDataType::Float4:
        return sizeof(float) * componentCount(type);
    }
    std::unreachable();
}
constexpr GLenum glType(ShaderDataType type) {
    switch (type) {
    case ShaderDataType::Float1:
    case ShaderDataType::Float2:
    case ShaderDataType::Float3:
    case ShaderDataType::Float4:
        return GL_FLOAT;
    }
    std::unreachable();
}

} // namespace

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

void VertexArray::addIndexBuffer(const IndexBuffer& buffer) {
    glVertexArrayElementBuffer(m_handle, buffer.handle());
}

void VertexArray::addVertexBuffer(const VertexBuffer& buffer, const VertexLayout& layout) {

    uint32_t t_stride = 0;
    for (auto [type, pos] : layout) {
        t_stride += sizeOf(type);
    }
    glVertexArrayVertexBuffer(m_handle, 0, buffer.handle(), 0, t_stride);

    uint32_t stride = 0;
    for (auto [type, pos] : layout) {
        glEnableVertexArrayAttrib(m_handle, pos);
        glVertexArrayAttribFormat(m_handle, pos, componentCount(type), glType(type), GL_FALSE,
                                  stride);
        glVertexArrayAttribBinding(m_handle, pos, 0);
        stride += sizeOf(type);
    }
}

} // namespace mamba::Renderer
