#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "shader.h"

#define PROJECT_NAME "Opengl-playground part 1"

int main(int argc, char **argv) {
	if(argc != 3) {
		printf("Usage:\n %s <vertex shader source file> <fragment shader source file>", argv[0]);
		return 1;
	}
	std::string vs_path(argv[1]);
	std::string fs_path(argv[2]);
    printf("This is project %s.\n", PROJECT_NAME);
	GLFWwindow *window;
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}
	window = glfwCreateWindow(1024, 768, "Part 1", NULL, NULL);
	glfwWindowHint(GLFW_SAMPLES, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	
	if (!window) {
		std::cerr << "Failed to open GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// the documentation specifies that glew can only be initialized after a valid
	// rendering context has been enabled
	GLenum status = glewInit();
	if (status != GLEW_OK) {
		std::cerr << "GLEW initialisation failed!" << std::endl;
		std::cerr << "Error: " << glewGetErrorString(status) << std::endl;
	}
	std::cout << glGetString(GL_VERSION) << std::endl;

	// make sure the input from the keyboard can be collected
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// create vertex Array no Idea what this is yet
	unsigned int va_id;
	glGenVertexArrays(1, &va_id);
	glBindVertexArray(va_id);

	// define the Data used to draw the triangle
	static const float g_vertex_data[] = {
		-.5f, -.5f, 0.f,
		.5f, -.5f, 0.f,
		0.f, .5f, 0.f
	};

	// create the vertex buffer inside the opengl memory
	unsigned int vertex_buffer_id;
	glGenBuffers(1, &vertex_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_data), g_vertex_data, GL_STATIC_DRAW);

	Shader VS = Shader(vs_path, GL_VERTEX_SHADER);
	Shader FS = Shader(fs_path, GL_FRAGMENT_SHADER);
	unsigned int program_id = CreateShaderProgram(VS, FS);
	glUseProgram(program_id);
	do {
		glClear(GL_COLOR_BUFFER_BIT);
		
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
	glfwTerminate();
    return 0;
}
