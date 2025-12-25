#include "renderer.hpp"

#include "stb_image.h"
#include <iostream>

namespace mamba {

namespace Renderer {

Texture loadTexture(const std::filesystem::path& path) {
    stbi_set_flip_vertically_on_load(1);

    int width, height, channels;
    std::string filepath = path.string();
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

    if (!data) {
        std::cerr << "Failed to load texture: " << filepath << "\n";
        return {};
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

    return {
        .width = width,
        .height = height,
        .handle = handle,
    };
}
} // namespace Renderer

} // namespace mamba
