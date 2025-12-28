#pragma once

#include "glad/glad.h"
#include <functional>

namespace mamba {
namespace Renderer {

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

    void addVertexBuffer(const std::function<void(GLuint)>);
    void addIndexBuffer(GLuint);

  private:
    GLuint m_handle{0};
};

} // namespace Renderer
} // namespace mamba
