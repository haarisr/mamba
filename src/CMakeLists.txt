add_library(mamba STATIC window.cpp layer.cpp)

target_include_directories(mamba PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(mamba PUBLIC glfw glad)

target_compile_options(mamba PRIVATE
    -Wall -Wextra -Wpedantic -Werror
)
