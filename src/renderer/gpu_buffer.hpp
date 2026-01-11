#pragma once

#include "glad/glad.h"
#include <span>
#include <type_traits>
#include <utility>

namespace mamba {
namespace Renderer {

template <GLenum Value, GLenum... Candidates>
concept is_one_of = ((Value == Candidates) || ...);

template <GLenum Target>
concept BufferTarget =
    is_one_of<Target, GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, GL_UNIFORM_BUFFER>;

template <GLenum Target>
concept BindableTarget = is_one_of<Target, GL_UNIFORM_BUFFER>;

template <GLenum Target, typename T>
    requires BufferTarget<Target>
class GPUBuffer {
  public:
    GPUBuffer(size_t max_count) {
        glCreateBuffers(1, &m_handle);
        glNamedBufferStorage(m_handle, stride() * max_count, nullptr, GL_DYNAMIC_STORAGE_BIT);
    }

    GPUBuffer(std::span<const T> data) {
        glCreateBuffers(1, &m_handle);
        glNamedBufferStorage(m_handle, data.size_bytes(), data.data(), GL_DYNAMIC_STORAGE_BIT);
    }

    ~GPUBuffer() { glDeleteBuffers(1, &m_handle); }

    GPUBuffer(const GPUBuffer&) = delete;
    GPUBuffer& operator=(const GPUBuffer&) = delete;

    GPUBuffer(GPUBuffer&& other) noexcept : m_handle(std::exchange(other.m_handle, 0)) {}
    GPUBuffer& operator=(GPUBuffer&& other) noexcept {
        std::swap(m_handle, other.m_handle);
        return *this;
    }

    void bind(GLuint bindingPoint)
        requires BindableTarget<Target>
    {
        glBindBufferBase(Target, bindingPoint, m_handle);
    }

    GLuint handle() const { return m_handle; }

    void update(std::span<const T> data) {
        glNamedBufferSubData(m_handle, 0, data.size_bytes(), data.data());
    }

    static consteval size_t stride() { return sizeof(T); }

  private:
    GLuint m_handle{0};
};

template <typename T>
using VertexBuffer = GPUBuffer<GL_ARRAY_BUFFER, T>;

template <typename T>
    requires std::is_integral_v<T>
using IndexBuffer = GPUBuffer<GL_ELEMENT_ARRAY_BUFFER, T>;

} // namespace Renderer
} // namespace mamba
