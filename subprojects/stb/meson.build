project('stb_image', 'cpp', version: '1.0')

stb_inc = include_directories('.')

stb_lib = static_library(
  'stb_image',
  files('stb_image.cpp'),
  include_directories: stb_inc
)

stb_dep = declare_dependency(
  include_directories: stb_inc,
  link_with: stb_lib
)
