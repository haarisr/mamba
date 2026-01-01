add_executable(example src/main.cpp src/button.cpp)

target_link_libraries(example PRIVATE mamba)

target_compile_options(example PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /permissive->
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

# Copy textures to build directory
add_custom_command(TARGET example POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/sandbox/example/textures
        $<TARGET_FILE_DIR:example>/sandbox/example/textures
)
