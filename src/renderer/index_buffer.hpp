#pragma once

#include "glad/glad.h"
#include <cstdint>
#include <span>

namespace mamba {
namespace Renderer {

class IndexBuffer {

  public:
    IndexBuffer(std::span<const unsigned int>);
    ~IndexBuffer();

    IndexBuffer(const IndexBuffer&) = delete;
    IndexBuffer& operator=(const IndexBuffer&) = delete;
    IndexBuffer(IndexBuffer&&) noexcept;
    IndexBuffer& operator=(IndexBuffer&&) noexcept;

    GLuint handle() const { return m_handle; }
    std::uint32_t count() const { return m_count; }

  private:
    GLuint m_handle{0};
    std::uint32_t m_count{0};
};

} // namespace Renderer
} // namespace mamba
