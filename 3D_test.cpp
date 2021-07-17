#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <glm/glm.hpp>
#include <chrono>
#include <thread>
#define STB_IMAGE_IMPLEMENTATION
#include "shader.h"
#include "GraphicPrimitive.h"

#define PROJECT_NAME "opengl-3D-Tests"
#define PI 3.14159265

#define ASSERT(x) if(!(x)) exit(2);

#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__));

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	// resizes the screen to the screen size set by the window manager
	glViewport(0, 0, width, height);
}

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

GLFWwindow* init_window(void (*size_callback)(GLFWwindow*, int, int)) {
	GLFWwindow *window;
	// load glfw
	if (!glfwInit()) {
		std::cout << "GLFW initialisation failed, exiting" << std::endl;
		exit(-1);
	}
	// set OpenGL properties
	glfwWindowHint(GLFW_SAMPLES, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// create window and configure it
	window = glfwCreateWindow(1200, 1200, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(-1);
	}
	//
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, size_callback);

	// initialize glew
	if (glewInit() != GLEW_OK)
		std::cout << "GLEW initialisation failed!" << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;
	return window;
}

bool sort_by_x(const glm::vec2 &v1, const glm::vec2 &v2) {
	return v1.x < v2.x;
}


int main(int argc, char **argv) {
	int rnd_num_amount = 20;
	if(argc != 1 && argc != 2) {
		printf("Usage:\n %s [amount of random numbers]", argv[0]);
		return 1;
	} else if (argc == 2) {
		rnd_num_amount = std::stoi(argv[1]);
	}
	std::string vs_path = "../assets/shader/basic_3D.vert";
	std::string fs_path = "../assets/shader/basic_3D.frag";
	printf("This is project %s.\n", PROJECT_NAME);

	// create a window and initalize it with the settings we want
	GLFWwindow* window = init_window(framebuffer_size_callback);
	// load, compile and link the shaders into the program that the gpu executes
	ShaderProgram shader = ShaderProgram(vs_path, fs_path);

	// generate random numbers and feed them into the point cloud
	std::vector<glm::vec3> random_vectors;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(-1.0, 1.0);
	for (int i=0; i<rnd_num_amount; ++i) {
		glm::vec3 r = glm::vec3(dis(gen), dis(gen), dis(gen));
		random_vectors.push_back(r);
	}
	glPointSize(3.);
	
	// specify the color of the points as green
	glm::vec4 color = glm::vec4(0., 1., 0., 1.);
	// create the points
	GraphicalPrimitive pc = GraphicalPrimitive(random_vectors, &shader, GL_LINES, color);
	
	// set up the variables for the perspective transformation
	int width, height;
	float FoV = 45.;
	GLCall(unsigned int view_matrix_loc = glGetUniformLocation(shader.id, "u_viewport_matrix"));

	// set up the constant variables for the viewport orientation transformation
	glm::vec3 viewport_up = glm::vec3(0.f, 1.f, 0.f);
	float angle = 0;
	float distance = 2;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		glfwGetWindowSize(window, &width, &height);
		glm::mat4 view_projection = glm::perspective(
				glm::radians(FoV),
				(float)width/(float)height,
				0.1f,
				100.f
		);

		glm::vec3 viewport_pos = glm::vec3(std::sin(angle), 0., std::cos(angle));
		glm::vec3 viewport_target = glm::vec3(-std::sin(angle), 0., -std::cos(angle));
		glm::mat4 viewport_location_matrix = glm::lookAt(
				viewport_pos * distance,
				viewport_target,
				viewport_up
		);

		glm::mat4 view_matrix = view_projection * viewport_location_matrix;
		
		//set up the viewport transformation
		shader.use();
		GLCall(glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, glm::value_ptr(view_matrix)));
		
		pc.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
		angle += 0.01;
	}
	glfwTerminate();
	return 0;
}
