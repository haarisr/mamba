#pragma once
#include <cstdint>
#include <span>

namespace mamba::Renderer::Fonts {

inline constexpr auto ROBOTO = [] {
    static constexpr uint8_t data[] = {
#embed "Roboto-Regular.ttf"
    };
    return std::span<const uint8_t>{data};
}();

} // namespace mamba::Renderer::Fonts
