#pragma once
#include <string_view>

namespace mamba::Renderer::Shaders {

inline constexpr auto QUAD_VERT = [] {
    static constexpr char data[] = {
#embed "quad.vert" suffix(, '\0')
    };
    return std::string_view{data};
}();

inline constexpr auto QUAD_FRAG = [] {
    static constexpr char data[] = {
#embed "quad.frag" suffix(, '\0')
    };
    return std::string_view{data};
}();

inline constexpr auto TEXT_VERT = [] {
    static constexpr char data[] = {
#embed "text.vert" suffix(, '\0')
    };
    return std::string_view{data};
}();

inline constexpr auto TEXT_FRAG = [] {
    static constexpr char data[] = {
#embed "text.frag" suffix(, '\0')
    };
    return std::string_view{data};
}();

} // namespace mamba::Renderer::Shaders
