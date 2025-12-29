add_library(mamba_renderer STATIC
 camera.cpp
 renderer.cpp
 shader.cpp
 vertex_array.cpp
 index_buffer.cpp
 vertex_buffer.cpp
)

add_library(mamba::renderer ALIAS mamba_renderer)

target_include_directories(mamba_renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(mamba_renderer PUBLIC glad glm::glm stb_image)
