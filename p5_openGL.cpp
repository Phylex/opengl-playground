#include <iostream>
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

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

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
	if(argc != 5) {
		printf("Usage:\n %s <vertex shader source file> <fragment shader source file> <texture1 file> <texture2 file>", argv[0]);
		return 1;
	}
	std::string vs_path(argv[1]);
	std::string fs_path(argv[2]);
	std::string texture1_path(argv[3]);
	std::string texture2_path(argv[4]);
	printf("This is project %s.\n", PROJECT_NAME);
	GLFWwindow* window;


	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_SAMPLES, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// the documentation specifies that glew can only be initialized after a valid
	// rendering context has been enabled
	if (glewInit() != GLEW_OK)
		std::cout << "GLEW initialisation failed!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

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

	// load, compile and link the shaders into the program that the gpu executes
	ShaderProgram shader = ShaderProgram(vs_path, fs_path);
	
	// this creates and binds a vertex array. It contains with it a definition
	// of the layout of the data in the buffer and a link to the actual buffer itself
	unsigned int vao, vbo, ebo;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));
	
	// here a buffer is generated
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW));

	// tell opengl what is in the buffer, (once per attribute)
	// also enable the attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)(3*sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)(6*sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// create an index buffer so that the indices don't need to be duplicated
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// bind an empty array object
	GLCall(glBindVertexArray(0));
	
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the wrapping behaviour of the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// set the filter methods
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load the texture 1
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(texture1_path.c_str(), &width, &height, &nrChannels, 0);
	if (data) {
		std::cout << "width: " << width << " height: " << height << " channels: " << nrChannels << std::endl;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cerr << "Failed to load the Texture " << texture1_path << std::endl;
	}
	stbi_image_free(data);

	// load texture 2
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the wrapping behaviour of the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// set the filter methods
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load(texture2_path.c_str(), &width, &height, &nrChannels, 0);
	if (data) {
		std::cout << "width: " << width << " height: " << height << " channels: " << nrChannels << std::endl;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		std::cerr << "Failed to load the Texture " << texture1_path << std::endl;
	}
	stbi_image_free(data);
	shader.use();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);

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

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	// resizes the screen to the screen size set by the window manager
	glViewport(0, 0, width, height);
}
