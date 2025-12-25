#include <cstdint>
#include <filesystem>

namespace mamba {

namespace renderer {

uint32_t createGraphicsShader(const std::filesystem::path& vertex_path,
                              const std::filesystem::path& fragment_path);
} // namespace renderer

} // namespace mamba
