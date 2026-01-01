add_library(mamba_renderer STATIC
 camera.cpp
 renderer.cpp
 shader.cpp
 texture.cpp
 vertex_array.cpp
 index_buffer.cpp
)

add_library(mamba::renderer ALIAS mamba_renderer)

# Embed shaders at build time
set(SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(SHADER_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/generated/shaders.hpp)

add_custom_command(
    OUTPUT ${SHADER_OUTPUT}
    COMMAND ${CMAKE_COMMAND}
        -DSHADER_DIR=${SHADER_DIR}
        -DOUTPUT_FILE=${SHADER_OUTPUT}
        -P ${SHADER_DIR}/generate_shaders.cmake
    DEPENDS
        ${SHADER_DIR}/quad.vert
        ${SHADER_DIR}/quad.frag
        ${SHADER_DIR}/shaders.hpp.in
        ${SHADER_DIR}/generate_shaders.cmake
    COMMENT "Generating shaders.hpp"
)

target_sources(mamba_renderer PRIVATE ${SHADER_OUTPUT})

target_include_directories(mamba_renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(mamba_renderer PUBLIC glad glm::glm stb_image)
