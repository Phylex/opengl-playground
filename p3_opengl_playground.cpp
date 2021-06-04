#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "load_airfoil.h"

#define PROJECT_NAME "Opengl-playground"



int main(int argc, char **argv) {
	if(argc != 4) {
		printf("Usage:\n %s <vertex shader source file> <fragment shader source file> <airfoil filepath>", argv[0]);
		return 1;
	}
	std::string vs_path(argv[1]);
	std::string fs_path(argv[2]);
	std::string airfoil_path(argv[3]);
	printf("This is project %s.\n", PROJECT_NAME);
	GLFWwindow* window;


	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(1200, 1200, "Hello World", NULL, NULL);
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

	// a buffer is generated to store the vertices in
	unsigned int vertex_buffer;
	unsigned int vertex_array;
	
	// load the data, in this case an airfoil in xfoil format
	std::vector<float> airfoil = load_airfoil(airfoil_path);
	// The track should be closed, so the first coordinate is added here again
	airfoil.push_back(airfoil[0]);
	airfoil.push_back(airfoil[1]);

	// the vertex Array holds the information about the layout of the data in the vertex buffer
	glGenVertexArrays(1, &vertex_array);
	// here a buffer is generated
	glGenBuffers(1, &vertex_buffer);

	glBindVertexArray(vertex_array);
	// this call tells OpenGL that the buffer to draw from is buffer 1
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	// the following function needs to know the buffer
	// size in bytes or a NULL if nothing is to be written as it is copied into the
	// memory space of the opengl driver
	glBufferData(GL_ARRAY_BUFFER, airfoil.size() * sizeof(float), &airfoil.front(), GL_STATIC_DRAW);

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
		glDrawArrays(GL_LINE_STRIP, 0, airfoil.size()/2);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}
