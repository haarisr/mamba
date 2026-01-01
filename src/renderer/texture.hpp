#pragma once

#include <filesystem>
#include <optional>

#include <glad/glad.h>

namespace mamba {
namespace Renderer {

class Texture {
  public:
    static auto create(const std::filesystem::path& path) -> std::optional<Texture>;
    static auto create(const uint8_t* data, int width, int height, int channels = 3) -> Texture;
    static auto createWhite() -> Texture;

    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture(Texture&&) noexcept;
    Texture& operator=(Texture&&) noexcept;

    GLuint handle() const { return m_handle; }
    int width() const { return m_width; }
    int height() const { return m_height; }

  private:
    Texture(GLuint handle, int width, int height);

    GLuint m_handle{0};
    int m_width{0};
    int m_height{0};
};

} // namespace Renderer
} // namespace mamba
