#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <glm/glm.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
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
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
		void draw();
	private:
		std::vector<glm::vec2> points;
		void fill_vertex_buffer(std::vector<float>&);
		bool current;
		bool gl_context_created;
		unsigned int vao;
		unsigned int vbo;
		ShaderProgram *shader;
};

PointCloud::PointCloud(std::vector<glm::vec2> &in_points, ShaderProgram *in_shader) {
	points = in_points;
	current = false;
	gl_context_created = false;
	shader = in_shader;
}

void PointCloud::add_points(glm::vec2 point) {
	points.push_back(point);
	current = false;
}

void PointCloud::add_points(std::vector<glm::vec2>& add_points) {
	for (auto p: add_points) {
		points.push_back(p);
	}
	current = false;
}

void PointCloud::fill_vertex_buffer(std::vector<float>& vb_content) {
	for(auto p: points) {
		vb_content.push_back(p.x);
		vb_content.push_back(p.y);
		vb_content.push_back(0.);
		vb_content.push_back(0.);
	}
}


void PointCloud::draw() {
	if (!gl_context_created) {
		GLCall(glGenVertexArrays(1, &vao));
		GLCall(glBindVertexArray(vao));
		GLCall(glGenBuffers(1, &vbo));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
		gl_context_created = true;
	} else if(!current) {
		GLCall(glBindVertexArray(vao));
		std::vector<float> vb_content;
		fill_vertex_buffer(vb_content);
		GLCall(glBufferData(GL_ARRAY_BUFFER,
							sizeof(float)*vb_content.size(),
							(void *)vb_content.data(),
							GL_DYNAMIC_DRAW));
		GLCall(glVertexAttribPointer(0, 4,
							  GL_FLOAT,
							  GL_FALSE,
							  sizeof(float)*4,
							  (void*)0));
		GLCall(glEnableVertexAttribArray(0));
		current = true;
	}
	GLCall(glBindVertexArray(vao));
	GLCall(glDrawArrays(GL_POINTS, 0, points.size()));
}



int main(int argc, char **argv) {
	if(argc != 3) {
		printf("Usage:\n %s <texture1 file> <texture2 file>", argv[0]);
		return 1;
	}
	std::string vs_path = "./texture_veretx.vert";
	std::string fs_path = "./texture_fragment.frag";
	std::string texture1_path(argv[3]);
	std::string texture2_path(argv[4]);
	printf("This is project %s.\n", PROJECT_NAME);

	// create a window and initalize it with the settings we want
	GLFWwindow* window = init_window(framebuffer_size_callback);
	// load, compile and link the shaders into the program that the gpu executes
	ShaderProgram shader = ShaderProgram(vs_path, fs_path);

	// the following block is the initialisation and copying of the data into
	// the memory space of the OpenGL implementation
	float vertices[] = {
		// position			// Colour			// Texture
		-.5,-.5, 0.,		1.0, 0.0, 0.0,		0.0, 0.0,
		 .5,-.5, 0.,		0.0, 1.0, 0.0,		1.0, 0.0,
		 .5, .5, 0.,		0.0, 0.0, 1.0,		1.0, 1.0,
		-.5, .5, 0.,		1.0, 1.0, 0.0,		0.0, 1.0,
	};

	float v0[] = {
		0.002, 0.001
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		shader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glBindVertexArray(vao);
		// this is the draw call to OpenGL
		if (vertices[0] < -1.)
			v0[0] = abs(v0[0]);
		if (vertices[1] < -1.)
			v0[1] = abs(v0[1]);
		if (vertices[16] > 1.)
			v0[0] = -abs(v0[0]);
		if (vertices[17] > 1.)
			v0[1] = -abs(v0[1]);
		for (int i=0; i<4; i++) {
			vertices[i*8] += v0[0];
			vertices[i*8+1] += v0[1];
		}
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW));
		GLCall(glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, nullptr));
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

