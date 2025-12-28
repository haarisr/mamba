#pragma once

#include <cstdint>
#include <filesystem>

namespace mamba {

namespace Renderer {

uint32_t createGraphicsShader(const std::filesystem::path& vertex_path,
                              const std::filesystem::path& fragment_path);
} // namespace Renderer

} // namespace mamba
