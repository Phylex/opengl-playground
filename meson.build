project('Opengl-playground', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'default_lib=static_library'])

# Sources
p1_src = ['p1_opengl_playground.cpp']
p2_src = ['p2_opengl_playground.cpp']
p3_src = ['p3_opengl_playground.cpp']
p4_src = ['p4_openGL.cpp']
p5_src = ['p5_openGL.cpp']
convex_hull_src = ['convex_hull.cpp']
test_3d_src = ['3D_test.cpp']
line_intersection_src = ['line_intersection.cpp']

shader_src = ['shader.h', 'shader.cpp']
airfoil_load_src = ['load_airfoil.h', 'load_airfoil.cpp']
point_cloud_src = ['PointCloud.cpp', 'PointCloud.h']
polyline_src = ['PolyLine.cpp', 'PolyLine.h']
graphic_primitive_src = ['GraphicPrimitive.h', 'GraphicPrimitive.cpp']
# Dependencies
glfw = dependency('glfw3')
glu = dependency('glu')
glew = dependency('glew')
glm = dependency('glm')
xml = dependency('pugixml')
opengl = [glfw, glu, glew, glm]

shader_lib = library('shader',
  shader_src,
  version: '0.1',
  dependencies: opengl)
airfoil_load_lib = library('airfoil_load', 
  airfoil_load_src,
  version: '0.1')
PointCloud_lib = library('point_cloud',
  point_cloud_src,
  version: '0.1',
  dependencies: opengl,
  link_with: shader_lib)
PolyLine_lib = library('polyline',
  polyline_src,
  version: '0.1',
  dependencies: opengl,
  link_with: shader_lib)
GraphicPrimitive_lib = library('graphicprimitive',
  graphic_primitive_src,
  version: '0.1',
  dependencies: opengl,
  link_with: shader_lib)

# executable definition 
p1 = executable('p1',
  sources: [p1_src],
  link_with: shader_lib,
  install : false,
  dependencies : [opengl, glm])

p2 = executable('p2',
  sources: [p2_src, ],
  link_with: shader_lib,
  install : false,
  dependencies : opengl)

p3 = executable('p3',
  sources: [p3_src, ],
  link_with: [shader_lib, airfoil_load_lib],
  install : false,
  dependencies : opengl)

p4 = executable('p4', sources: [p4_src, ],
  link_with: shader_lib,
  install : false,
  dependencies : opengl)

p5 = executable('p5', sources: [p5_src, ],
  link_with: shader_lib,
  install : false,
  dependencies : opengl)

convex_hull = executable('ch', sources: [convex_hull_src, ],
  link_with: [shader_lib, PointCloud_lib, PolyLine_lib],
  install : false,
  dependencies : opengl)

test_3d = executable('t3d', sources: [test_3d_src, ],
  link_with: [shader_lib, GraphicPrimitive_lib],
  install: false,
  dependencies: opengl)

line_intersection = executable('lineinterseciton',
  sources: [line_intersection_src, ],
  link_with: [shader_lib, GraphicPrimitive_lib],
  install: false,
  dependencies: opengl)
#test('basic', exe)

