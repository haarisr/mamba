add_executable(example src/main.cpp src/button.cpp)

target_link_libraries(example PRIVATE mamba)

target_compile_options(example PRIVATE
    -Wall -Wextra -Wpedantic -Werror
)
