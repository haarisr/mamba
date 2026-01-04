add_subdirectory(renderer)

add_library(mamba STATIC app.cpp window.cpp)

target_include_directories(mamba PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(mamba PUBLIC glm::glm mamba::renderer)
target_link_libraries(mamba PRIVATE glfw glad stb_image)

target_compile_options(mamba PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /permissive->
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)
