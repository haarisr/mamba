add_executable(example main.cpp)

target_link_libraries(example PRIVATE mamba)

target_compile_options(example PRIVATE
    -Wall -Wextra -Wpedantic -Werror
)
