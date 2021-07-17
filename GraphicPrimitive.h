#ifndef GRAPHICAL_PRIMITIVE_H
#define GRAPHICAL_PRIMITIVE_H

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include "shader.h"

class GraphicalPrimitive {
	public:
		GraphicalPrimitive(std::vector<glm::vec3>&,
						   std::vector<unsigned int>&,
						   ShaderProgram*,
						   unsigned int,
						   glm::vec4,
						   glm::mat4);
		GraphicalPrimitive(std::vector<glm::vec3>&,
						   ShaderProgram*,
						   unsigned int,
						   glm::vec4);
		void reindex(std::vector<unsigned int>&);
		void reorient(glm::mat4);
		void print_points();
		void draw();

	private:
		std::vector<glm::vec3> points;
		std::vector<unsigned int> indices;
		unsigned int vao, vbo, ibo;
		int ucolor, u_model_matrix;
		glm::mat4 model_matrix;
		glm::vec4 color;
		ShaderProgram *shader;
		void init();
		void init_gl_context();
		void write_points_to_gl_array();
		void write_indices_to_ibo();
		unsigned int gl_render_type;
};

void print_vb_content(std::vector<float> &vb_content);
#endif
