cmake_minimum_required(VERSION 4.2.1)

project(Mamba)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

add_subdirectory(extern)
add_subdirectory(src)
