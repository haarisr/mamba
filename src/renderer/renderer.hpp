#pragma once

#include <filesystem>
#include <glad/glad.h>

namespace mamba {

struct Texture {

    int width;
    int height;
    GLuint handle;
};

namespace Renderer {

Texture loadTexture(const std::filesystem::path& path);

}

} // namespace mamba
