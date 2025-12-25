
add_executable(mamba_engine main.cpp window.cpp layer.cpp)
target_link_libraries(mamba_engine PRIVATE glfw glad)

target_compile_options(mamba_engine PRIVATE
    -Wall -Wextra -Wpedantic -Werror
)
