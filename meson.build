project('Opengl-playground', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3'])

# Sources
p1_src = ['p1_opengl_playground.cpp']
p2_src = ['p2_opengl_playground.cpp']
shader_src = ['shader.h', 'shader.cpp']
airfoil_load_src = ['load_airfoil.h', 'load_airfoil.cpp']
# Dependencies
glfw = dependency('glfw3')
glu = dependency('glu')
glew = dependency('glew')
opengl = [glfw, glu, glew]

# executable definition 
p1 = executable('p1', p1_src,
  install : false,
  dependencies : opengl)

p2 = executable('p2', sources: [p2_src, shader_src],
  install : false,
  dependencies : opengl)

p3 = executable('p3', sources: [p2_src, shader_src, airfoil_load_src],
  install : false,
  dependencies : opengl)

#test('basic', exe)
