#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"

#define PROJECT_NAME "Opengl-playground"


int main(int argc, char **argv) {
	if(argc != 3) {
		printf("Usage:\n %s <vertex shader source file> <fragment shader source file>", argv[0]);
		return 1;
	}
	std::string vs_path(argv[1]);
	std::string fs_path(argv[2]);
	printf("This is project %s.\n", PROJECT_NAME);
	GLFWwindow* window;


	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// the documentation specifies that glew can only be initialized after a valid
	// rendering context has been enabled
	if (glewInit() != GLEW_OK)
		std::cout << "GLEW initialisation failed!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	// the following block is the initialisation and copying of the data into
	// the memory space of the OpenGL implementation
	float positions[] = {
		-.5f, -.5f,
		-.0f,  .5f,
		.5f,  -.5f,
	};
	unsigned int buffer;
	// here a buffer is generated
	glGenBuffers(1, &buffer);
	// this call tells OpenGL that the buffer to draw from is buffer 1
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	// the following function needs to know the buffer
	// size in bytes or a NULL if nothing is to be written as it is copied into the
	// memory space of the opengl driver
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	// now OpenGL needs to be told the memory layout of the buffer that was specified
	// in the previous block
	// the function needs to be called once for every attribute to be specified
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);
	// the attribute needs to be enabled (the attribute has the index (or identifier) of 0)
	// so that is used in the enabeling function
	glEnableVertexAttribArray(0);
	Shader vertex_shader(vs_path, GL_VERTEX_SHADER);
	Shader fragment_shader(fs_path, GL_FRAGMENT_SHADER);
	unsigned int shader = CreateShader(vertex_shader, fragment_shader);
	glUseProgram(shader);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		// this is the draw call to OpenGL
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}
