#include "PointCloud.h"
#include <iostream>
#include <GL/glew.h>

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

void PointCloud::init_gl_context() {
	GLCall(glBindVertexArray(vao));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
}

PointCloud::PointCloud(std::vector<glm::vec2> &in_points, ShaderProgram *in_shader) {
	points = in_points;
	shader = in_shader;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glGenBuffers(1, &vbo));
	init_gl_context();
	write_points_to_gl_array();
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

void PointCloud::write_points_to_gl_array() {
	std::vector<float> vb_content;
	for(auto p: points) {
		vb_content.push_back(p.x);
		vb_content.push_back(p.y);
		vb_content.push_back(0.);
		vb_content.push_back(1.);
	}
	GLCall(glBufferData(GL_ARRAY_BUFFER,
						vb_content.size()*sizeof(float),
						&vb_content.front(), GL_DYNAMIC_DRAW)
	);
}

void PointCloud::draw() {
	init_gl_context();
	shader->use();
	GLCall(glDrawArrays(GL_POINTS, 0, points.size()));
	GLCall(glBindVertexArray(0));
}

void PointCloud::print_points() {
	for (auto v: points) {
		std::cout << '(' << v.x << ", " << v.y << ')' << std::endl;
	}
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
