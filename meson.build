project('Mamba', 'cpp', version: '0.1.0')
add_project_arguments('-std=c++26', '-Wall', '-Wpedantic', '-Werror', language: 'cpp')

glfw_sp = subproject('glfw', default_options: 'default_library=static')
glad_sp = subproject('glad')
stb_sp = subproject('stb')
glm_sp = subproject('glm')
ft_sp = subproject(
  'freetype2',
  default_options: [
    'brotli=disabled',
    'bzip2=disabled',
    'harfbuzz=disabled',
    'png=disabled',
    'tests=disabled',
    'zlib=none',
    'error_strings=false',
  ],
)

glfw_dep = glfw_sp.get_variable('glfw_dep')
glad_dep = glad_sp.get_variable('glad_dep')
stb_dep = stb_sp.get_variable('stb_dep')
glm_dep = glm_sp.get_variable('glm_dep')
freetype_dep = ft_sp.get_variable('freetype_dep')

cmake_mod = import('cmake')

msdf_opts = cmake_mod.subproject_options()
msdf_opts.add_cmake_defines(
  {
    'MSDF_ATLAS_BUILD_STANDALONE': 'OFF',
    'MSDF_ATLAS_USE_VCPKG': 'OFF',
    'MSDF_ATLAS_USE_SKIA': 'OFF',
    'MSDF_ATLAS_NO_ARTERY_FONT': 'ON',
    'MSDF_ATLAS_INSTALL': 'OFF',
    'MSDF_ATLAS_DYNAMIC_RUNTIME': 'ON',
    'MSDFGEN_CORE_ONLY': 'OFF',
    'MSDFGEN_BUILD_STANDALONE': 'OFF',
    'MSDFGEN_USE_VCPKG': 'OFF',
    'MSDFGEN_USE_OPENMP': 'OFF',
    'MSDFGEN_USE_SKIA': 'OFF',
    'MSDFGEN_DISABLE_SVG': 'ON',
    'MSDFGEN_DISABLE_PNG': 'ON',
    'MSDFGEN_INSTALL': 'OFF',
  },
)
msdf_sp = cmake_mod.subproject('msdf-atlas-gen', options: msdf_opts)
msdf_dep = msdf_sp.dependency('msdf-atlas-gen', include_type: 'system')

subdir('src')
subdir('sandbox')
