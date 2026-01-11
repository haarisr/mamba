#pragma once

#include "renderer/gpu_buffer.hpp"

#include <cstdint>
#include <glad/glad.h>
#include <utility>
#include <vector>

namespace mamba {
namespace Renderer {

enum class ShaderDataType { Float1, Float2, Float3, Float4, Int1 };
using VertexLayout = std::vector<std::pair<ShaderDataType, uint32_t>>;
namespace {

constexpr std::uint32_t componentCount(ShaderDataType type);
constexpr std::uint32_t sizeOf(ShaderDataType type);
constexpr GLenum glType(ShaderDataType type);

} // namespace

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

    template <typename T>
    void addVertexBuffer(const VertexBuffer<T>&, const VertexLayout&);

    template <typename T>
    void addIndexBuffer(const IndexBuffer<T>&);

  private:
    GLuint m_handle{0};
};

} // namespace Renderer

} // namespace mamba

namespace mamba::Renderer {

template <typename T>
void VertexArray::addVertexBuffer(const VertexBuffer<T>& buffer, const VertexLayout& layout) {

    uint32_t t_stride = 0;
    for (auto [type, pos] : layout) {
        t_stride += sizeOf(type);
    }
    glVertexArrayVertexBuffer(m_handle, 0, buffer.handle(), 0, t_stride);

    uint32_t stride = 0;
    for (auto [type, pos] : layout) {
        glEnableVertexArrayAttrib(m_handle, pos);
        switch (type) {
        case ShaderDataType::Int1:
            glVertexArrayAttribIFormat(m_handle, pos, componentCount(type), glType(type), stride);
            break;
        case ShaderDataType::Float1:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
            glVertexArrayAttribFormat(m_handle, pos, componentCount(type), glType(type), GL_FALSE,
                                      stride);
            break;
        }
        glVertexArrayAttribBinding(m_handle, pos, 0);
        stride += sizeOf(type);
    }
}

template <typename T>
void VertexArray::addIndexBuffer(const IndexBuffer<T>& buffer) {
    glVertexArrayElementBuffer(m_handle, buffer.handle());
}

namespace {

constexpr std::uint32_t componentCount(ShaderDataType type) {

    switch (type) {
    case ShaderDataType::Float1:
    case ShaderDataType::Int1:
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
    case ShaderDataType::Int1:
        return sizeof(int) * componentCount(type);
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
    case ShaderDataType::Int1:
        return GL_INT;
    }
    std::unreachable();
}

} // namespace

} // namespace mamba::Renderer
