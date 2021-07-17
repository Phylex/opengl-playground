#include "GraphicPrimitive.h"
#include <iostream>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

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

void GraphicalPrimitive::init_gl_context() {
	GLCall(glBindVertexArray(vao));
}

void GraphicalPrimitive::init() {
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glGenBuffers(1, &vbo));
	GLCall(glGenBuffers(1, &ibo));
	init_gl_context();
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	write_points_to_gl_array();
	write_indices_to_ibo();
	GLCall(glVertexAttribPointer(0, 4,
						  GL_FLOAT,
						  GL_FALSE,
						  sizeof(float)*4,
						  0));
	GLCall(glEnableVertexAttribArray(0));
	// load the shader, as we are trying to bind the uniform variables
	// to the uniform variables of the shader
	shader->use();

	// load the color value for the points
	GLCall(ucolor = glGetUniformLocation(shader->id, "u_color"));
	ASSERT(ucolor != -1);
	GLCall(glUniform4f(ucolor, color.x, color.y, color.z, color.w));

	// load the matrix transforming the points from model space to world space
	GLCall(u_model_matrix = glGetUniformLocation(shader->id, "u_model_matrix"));
	ASSERT(u_model_matrix != -1);
	GLCall(glUniformMatrix4fv(u_model_matrix, 1, GL_FALSE, glm::value_ptr(model_matrix)));

	GLCall(glBindVertexArray(0));
}


GraphicalPrimitive::GraphicalPrimitive(std::vector<glm::vec3> &points,
		std::vector<unsigned int>& indices,
		ShaderProgram *shader,
		unsigned int gl_render_type,
		glm::vec4 color,
		glm::mat4 model_matrix
) : color{color}, shader{shader}, points{points}, indices{indices},
	gl_render_type{gl_render_type}, model_matrix{model_matrix}
{
	init();
}

GraphicalPrimitive::GraphicalPrimitive(
		std::vector<glm::vec3> &points,
		ShaderProgram* shader,
		unsigned int gl_render_type,
		glm::vec4 color
) : color{color}, shader{shader}, points{points}, gl_render_type{gl_render_type},
	model_matrix{glm::mat4(1.0f)}
{
	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < points.size(); i++) {
		indices.push_back(i);
	}
	this->indices = indices;
	init();
}

void GraphicalPrimitive::write_points_to_gl_array() {
	std::vector<float> vb_content;
	for(auto p: points) {
		vb_content.push_back(p.x);
		vb_content.push_back(p.y);
		vb_content.push_back(p.z);
		vb_content.push_back(1.);
	}
	GLCall(glBufferData(GL_ARRAY_BUFFER,
						vb_content.size()*sizeof(float),
						&vb_content.front(), GL_DYNAMIC_DRAW)
	);
}

void GraphicalPrimitive::write_indices_to_ibo() {
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				sizeof(unsigned int)*indices.size(),
				indices.data(),
				GL_DYNAMIC_DRAW
	));
}


void GraphicalPrimitive::draw() {
	init_gl_context();
	shader->use();
	GLCall(glDrawElements(gl_render_type, indices.size(), GL_UNSIGNED_INT, 0));
	GLCall(glBindVertexArray(0));
}

void GraphicalPrimitive::print_points() {
	for (auto v: points) {
		std::cout << '(' << v.x << ", " << v.y << ", " << v.z << ')' << std::endl;
	}
}

void GraphicalPrimitive::reorient(glm::mat4 in_model_matrix) {
	model_matrix = in_model_matrix;
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
