# OpenGL Playground
This is the repository where I play around with the OpenGL API
and try out some techniques for visualizing stuff.
The main reason for this is that I want to be able to visualize geometric algorithms and other stuff and I would like to be able to visualize that.

## Building
To build the programs, the meson build system needs to be installed. If it is, the directory named `build` needs to be created inside the project
directory and then configured for the use with meson using `meson setup build` command run in the project directory.
If the directory is set up, `cd` into the directory and run `meson compile`.

## Dependencies
The programs here need `glfw` and `glew` libraries. to work correctly

## Running the programs
I decided I would pass the shaders as parameters to the programs. If the programs don't get the desires input, they will emit a short description of
the expected arguments. All Programs expect a vertex and a fragment shader argument. The p5 example also expects two textures.
