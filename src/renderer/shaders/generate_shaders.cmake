# Generate shaders.hpp from shader files
file(READ "${SHADER_DIR}/quad.vert" QUAD_VERT_SOURCE)
file(READ "${SHADER_DIR}/quad.frag" QUAD_FRAG_SOURCE)
file(READ "${SHADER_DIR}/text.vert" TEXT_VERT_SOURCE)
file(READ "${SHADER_DIR}/text.frag" TEXT_FRAG_SOURCE)
configure_file("${SHADER_DIR}/shaders.hpp.in" "${OUTPUT_FILE}" @ONLY)
