add_library(mamba_renderer STATIC
 camera.cpp
 font.cpp
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

# Embed fonts at build time
set(FONT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/fonts)
set(FONT_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/generated/fonts.cpp)

add_custom_command(
    OUTPUT ${FONT_OUTPUT}
    COMMAND ${CMAKE_COMMAND}
        -DFONT_DIR=${FONT_DIR}
        -DOUTPUT_FILE=${FONT_OUTPUT}
        -P ${FONT_DIR}/generate_fonts.cmake
    DEPENDS
        ${FONT_DIR}/Roboto-Regular.ttf
        ${FONT_DIR}/fonts.cpp.in
        ${FONT_DIR}/generate_fonts.cmake
    COMMENT "Generating fonts.cpp"
)

target_sources(mamba_renderer PRIVATE ${FONT_OUTPUT})

target_include_directories(mamba_renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(mamba_renderer PUBLIC glad glm::glm stb_image msdf-atlas-gen::msdf-atlas-gen)
