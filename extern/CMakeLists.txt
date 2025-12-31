# GLFW (submodule)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)

# GLAD - OpenGL 4.6 Core (pre-generated)
add_subdirectory(glad)

# stb_image
add_subdirectory(stb)

# GLM (submodule)
add_subdirectory(glm)
