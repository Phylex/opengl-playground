#include <iostream>
#include <vector>
#include <random>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <glm/glm.hpp>
#include <chrono>
#include <thread>
#define STB_IMAGE_IMPLEMENTATION
#include "shader.h"

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

class PointCloud {
	public:
		PointCloud(std::vector<glm::vec2>&, ShaderProgram*);
		void add_points(glm::vec2);
		void add_points(std::vector<glm::vec2>&);
		void print_points();
		void draw();
	private:
		std::vector<glm::vec2> points;
		void init_gl_context();
		void fill_vertex_buffer(std::vector<float>&);
		void write_points_to_gl_array();
		unsigned int vao;
		unsigned int vbo;
		ShaderProgram *shader;
};

void PointCloud::init_gl_context() {
	std::cout << "Binding VAO " << vao << std::endl;
	GLCall(glBindVertexArray(vao));
	std::cout << "Binding VBO " << vbo << std::endl;
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
}

PointCloud::PointCloud(std::vector<glm::vec2> &in_points, ShaderProgram *in_shader) {
	points = in_points;
	shader = in_shader;
	std::cout << "Generating VAO ";
	GLCall(glGenVertexArrays(1, &vao));
	std::cout << vao << std::endl;
	std::cout << "Generating VBO ";
	GLCall(glGenBuffers(1, &vbo));
	std::cout << vbo << std::endl;
	init_gl_context();
	write_points_to_gl_array();
	std::cout << "Setting the vertex attribute to two floats at index 1" << std::endl;
	GLCall(glVertexAttribPointer(0, 4,
						  GL_FLOAT,
						  GL_FALSE,
						  sizeof(float)*4,
						  0));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glBindVertexArray(0));
}

void PointCloud::add_points(glm::vec2 point) {
	points.push_back(point);
	init_gl_context();
	write_points_to_gl_array();
	GLCall(glBindVertexArray(0));
}

void PointCloud::add_points(std::vector<glm::vec2>& add_points) {
	for (auto p: add_points) {
		points.push_back(p);
	}
	init_gl_context();
	write_points_to_gl_array();
	GLCall(glBindVertexArray(0));
}

void print_vb_content(std::vector<float> &vb_content){
	int i = 0;
	for (auto v: vb_content) {
		if (i == 0)
			std::cout << '(' << v;
		else if (i == 3)
			std::cout << ", " << v << ')' << std::endl;
		else
			std::cout << ", " << v;
		i += 1;
		i %= 4;
	}
	std::cout << std::endl;
}

void PointCloud::write_points_to_gl_array() {
	std::vector<float> vb_content;
	fill_vertex_buffer(vb_content);
	std::cout << "Writing the following data to the VBO " << vbo << std::endl;
	print_vb_content(vb_content);
	GLCall(glBufferData(GL_ARRAY_BUFFER,
						vb_content.size()*sizeof(float),
						&vb_content.front(), GL_DYNAMIC_DRAW)
	);
}

void PointCloud::fill_vertex_buffer(std::vector<float>& vb_content) {
	for(auto p: points) {
		vb_content.push_back(p.x);
		vb_content.push_back(p.y);
		vb_content.push_back(0.);
		vb_content.push_back(1.);
	}
}

void PointCloud::draw() {
	init_gl_context();
	std::cout << "Binding Shader Program " << shader->id << std::endl;
	shader->use();
	std::cout << "Issuing Draw Call for " << points.size() << " Points" << std::endl;
	GLCall(glDrawArrays(GL_POINTS, 0, points.size()));
	std::cout << "Binding VAO 0" << std::endl;
	GLCall(glBindVertexArray(0));
}

void PointCloud::print_points() {
	for (auto v: points) {
		std::cout << '(' << v.x << ", " << v.y << ')' << std::endl;
	}
}


int main(int argc, char **argv) {
	if(argc != 1) {
		printf("Usage:\n %s", argv[0]);
		return 1;
	}
	std::string vs_path = "../assets/shader/conv_hull.vert";
	std::string fs_path = "../assets/shader/conv_hull.frag";
	printf("This is project %s.\n", PROJECT_NAME);

	// create a window and initalize it with the settings we want
	GLFWwindow* window = init_window(framebuffer_size_callback);
	// load, compile and link the shaders into the program that the gpu executes
	ShaderProgram shader = ShaderProgram(vs_path, fs_path);

	// generate random numbers and feed them into the point cloud
	std::vector<glm::vec2> random_vectors;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(-1.0, 1.0);
	for (int i=0; i<20; ++i) {
		glm::vec2 r = glm::vec2(dis(gen), dis(gen));
		random_vectors.push_back(r);
	}
	glPointSize(3.);
	PointCloud pc = PointCloud(random_vectors, &shader);
	
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		// this is the draw call to OpenGL

		pc.draw();

		std::this_thread::sleep_for(std::chrono::seconds(1));
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

