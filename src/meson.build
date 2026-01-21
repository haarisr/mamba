renderer_sources = files(
  'renderer/camera.cpp',
  'renderer/camera_controller.cpp',
  'renderer/font.cpp',
  'renderer/renderer.cpp',
  'renderer/shader.cpp',
  'renderer/texture.cpp',
  'renderer/vertex_array.cpp'
)

mamba_renderer = static_library(
  'mamba_renderer',
  renderer_sources,
  include_directories: include_directories('.'),
  dependencies: [glad_dep, glm_dep, stb_dep, msdf_dep]
)

mamba_lib = static_library(
  'mamba',
  files('app.cpp', 'window.cpp'),
  include_directories: include_directories('.'),
  link_with: mamba_renderer,
  dependencies: [glm_dep, glfw_dep, glad_dep, stb_dep, msdf_dep]
)

mamba_dep = declare_dependency(
  link_with: [mamba_lib, mamba_renderer],
  include_directories: include_directories('.'),
  dependencies: [glm_dep, glfw_dep, glad_dep, stb_dep, msdf_dep]
)
