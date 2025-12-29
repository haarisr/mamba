#include "texture.hpp"

#include <iostream>
#include <utility>

#include "stb_image.h"

namespace mamba::Renderer {

auto Texture::create(const std::filesystem::path& path) -> std::optional<Texture> {
    stbi_set_flip_vertically_on_load(1);

    int width, height, channels;
    std::string filepath = path.string();
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

    if (!data) {
        std::cerr << "Failed to load texture: " << filepath << "\n";
        return std::nullopt;
    }

    GLenum format = channels == 4 ? GL_RGBA : channels == 3 ? GL_RGB : channels == 1 ? GL_RED : 0;

    GLuint handle;
    glCreateTextures(GL_TEXTURE_2D, 1, &handle);

    glTextureStorage2D(handle, 1, (format == GL_RGBA ? GL_RGBA8 : GL_RGB8), width, height);
    glTextureSubImage2D(handle, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);

    glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureParameteri(handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(handle, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenerateTextureMipmap(handle);

    stbi_image_free(data);

    return Texture(handle, width, height);
}

Texture::Texture(GLuint handle, int width, int height)
    : m_handle(handle), m_width(width), m_height(height) {}

Texture::~Texture() { glDeleteTextures(1, &m_handle); }

Texture::Texture(Texture&& other) noexcept
    : m_handle(std::exchange(other.m_handle, 0)), m_width(std::exchange(other.m_width, 0)),
      m_height(std::exchange(other.m_height, 0)) {}

Texture& Texture::operator=(Texture&& other) noexcept {
    std::swap(m_handle, other.m_handle);
    std::swap(m_width, other.m_width);
    std::swap(m_height, other.m_height);
    return *this;
}

} // namespace mamba::Renderer
