#ifndef POLYLINE_H
#define POLYLINE_H

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include "shader.h"

class Polyline {
	public:
		Polyline(std::vector<glm::vec2>&, ShaderProgram*);
		void add_points(glm::vec2);
		void add_points(std::vector<glm::vec2>&);
		void print_points();
		void draw();
	private:
		std::vector<glm::vec2> points;
		unsigned int vao;
		unsigned int vbo;
		ShaderProgram *shader;
		void init_gl_context();
		void write_points_to_gl_array();
};
#endif
