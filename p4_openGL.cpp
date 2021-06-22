#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <glm/glm.hpp>
#include "shader.h"

#define PROJECT_NAME "opengl-uniforms"
#define PI 3.14159265

#define ASSERT(x) if(!(x)) exit(2);

#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__));

static void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* func, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL Error] " << error << " in [" << file << ":" << line << "]" << std::endl <<
		"Error in function: " << func << std::endl;
		return false;
	}
	return true;
}

int main(int argc, char **argv) {
	if(argc != 3) {
		printf("Usage:\n %s <vertex shader source file> <fragment shader source file>", argv[0]);
		return 1;
	}
	std::string vs_path(argv[1]);
	std::string fs_path(argv[2]);
	printf("This is project %s.\n", PROJECT_NAME);
	GLFWwindow* window;

	glfwWindowHint(GLFW_SAMPLES, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// the documentation specifies that glew can only be initialized after a valid
	// rendering context has been enabled
	if (glewInit() != GLEW_OK)
		std::cout << "GLEW initialisation failed!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	// the following block is the initialisation and copying of the data into
	// the memory space of the OpenGL implementation
	float positions[] = {
		-.5f, -.5f,
		.5f,  -.5f,
		.5f,  .5f,
		-.5f, .5f
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	// load, compile and link the shaders into the program that the gpu executes
	Shader vertex_shader(vs_path, GL_VERTEX_SHADER);
	Shader fragment_shader(fs_path, GL_FRAGMENT_SHADER);
	unsigned int shader = CreateShaderProgram(vertex_shader, fragment_shader);
	// give the uniform used in this shader an index with which to bind
	// data to
	GLCall(int u_Color_loc = glGetUniformLocation(shader, "u_Color"));
	ASSERT(u_Color_loc != -1);
	
	// this creates and binds a vertex array. It contains with it a definition
	// of the layout of the data in the buffer and a link to the actual buffer itself
	unsigned int vao;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));
	
	// here a buffer is generated
	unsigned int buf_id;
	glGenBuffers(1, &buf_id);
	glBindBuffer(GL_ARRAY_BUFFER, buf_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	// tell opengl what is in the buffer, (once per attribute)
	// also enable the attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);
	glEnableVertexAttribArray(0);

	// create an index buffer so that the indices don't need to be duplicated
	unsigned int index_buf_id;
	glGenBuffers(1, &index_buf_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buf_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// bind an empty array object
	glBindVertexArray(0);

	float hue = 0.0;
	float increment = 0.01;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		// calculate the new color for the rectangle
		hue = fmod((hue + increment),(2*PI));
		float r = sin(hue);
		float g = sin(hue+(2./3.*PI));
		float b = sin(hue+(4./3.*PI));
		
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader);
		glBindVertexArray(vao);
		GLCall(glUniform4f(u_Color_loc, r, g, b, 1.0f));
		// this is the draw call to OpenGL
		GLCall(glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, nullptr));
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}
