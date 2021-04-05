project('Opengl-playground', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3'])

# Sources
p1_src = ['p1_opengl_playground.cpp']
p2_src = ['p2_opengl_playground.cpp']
# Dependencies
glfw = dependency('glfw3')
glu = dependency('glu')
glew = dependency('glew')
opengl = [glfw, glu, glew]

# executable definition 
p1 = executable('p1_opengl_playground', p1_src,
  install : false,
  dependencies : opengl)
p2 = executable('p2_opengl_playground', p2_src,
  install : false,
  dependencies : opengl)

#test('basic', exe)
