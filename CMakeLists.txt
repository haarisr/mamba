cmake_minimum_required(VERSION 4.2.1)

project(Mamba)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(extern)

add_executable(mamba_engine main.cpp)
target_link_libraries(mamba_engine PRIVATE glfw glad)
