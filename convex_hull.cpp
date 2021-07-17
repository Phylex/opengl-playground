#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <math.h>
#include <glm/glm.hpp>
#include <chrono>
#include <thread>
#define STB_IMAGE_IMPLEMENTATION
#include "shader.h"
#include "PointCloud.h"
#include "PolyLine.h"

#define PROJECT_NAME "opengl-textures"
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

std::vector<glm::vec3> convex_hull(std::vector<glm::vec3> points) {
	std::sort(points.begin(), points.end(), sort_by_x);
	std::vector<glm::vec3> chull;
	// compute upper hull
	chull.push_back(points[0]);
	int position = 1;
	while(points[position].y <= chull.back().y && position < points.size()) {
		position +=1;
	}
	chull.push_back(points[position]);
	position += 1;
	while (position < points.size()) {
		size_t end = chull.size()-1;
		glm::vec3 v1 = chull[end] - chull[end-1];
		glm::vec3 v2 = points[position] - chull[end];
		glm::vec3 x = glm::cross(v1, v2);
		if (x.z < 0 ) {
			chull.push_back(points[position]);
			position += 1;
		}
		else if(x.z >= 0 ) {
			chull.pop_back();
		}
	}
	position -= 1;
	while(points[position].y >= chull.back().y && position > 0) {
		position -= 1;
	}
	while (position >= 0) {
		size_t end = chull.size()-1;
		glm::vec3 v1 = chull[end] - chull[end-1];
		glm::vec3 v2 = points[position] - chull[end];
		glm::vec3 x = glm::cross(v1, v2);
		if (x.z < 0) {
			chull.push_back(points[position]);
			position -= 1;
		}
		else if(x.z >= 0) {
			chull.pop_back();
		}
	}
	return chull;
}


int main(int argc, char **argv) {
	int rnd_num_amount = 20;
	if(argc != 1 && argc != 2) {
		printf("Usage:\n %s [amount of random numbers]", argv[0]);
		return 1;
	} else if (argc == 2) {
		rnd_num_amount = std::stoi(argv[1]);
	}
	std::string vs_path = "../assets/shader/conv_hull.vert";
	std::string fs_path = "../assets/shader/conv_hull.frag";
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
		glm::vec3 r = glm::vec3(dis(gen), dis(gen), 0.);
		random_vectors.push_back(r);
	}
	std::sort(random_vectors.begin(), random_vectors.end(), sort_by_x);
	glPointSize(3.);
	PointCloud pc = PointCloud(random_vectors, &shader);
	std::vector<glm::vec3> chull_points = convex_hull(random_vectors);
	Polyline conv_hull = Polyline(chull_points, &shader);
	
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		// this is the draw call to OpenGL
		pc.draw();
		conv_hull.draw();

		//std::this_thread::sleep_for(std::chrono::seconds(1));
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}
