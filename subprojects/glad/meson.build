project('glad', 'c', version: '0.1')

glad_inc = include_directories('include')
glad_src = files('src/glad.c')

glad_lib = static_library('glad', glad_src, include_directories: glad_inc)

glad_dep = declare_dependency(include_directories: glad_inc, link_with: glad_lib)
