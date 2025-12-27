add_library(mamba STATIC app.cpp window.cpp renderer/shader.cpp renderer/renderer.cpp renderer/camera.cpp)

target_include_directories(mamba PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(mamba PUBLIC glfw glad stb_image glm::glm)

target_compile_options(mamba PRIVATE
    -Wall -Wextra -Wpedantic -Werror
)
