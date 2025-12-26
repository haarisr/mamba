add_library(mamba STATIC app.cpp window.cpp layer.cpp renderer/shader.cpp renderer/renderer.cpp)

target_include_directories(mamba PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(mamba PUBLIC glfw glad stb_image)

target_compile_options(mamba PRIVATE
    -Wall -Wextra -Wpedantic -Werror
)
