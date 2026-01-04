add_executable(breakout src/main.cpp src/breakout.cpp)

target_link_libraries(breakout PRIVATE mamba)

target_compile_options(breakout PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /permissive->
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)
