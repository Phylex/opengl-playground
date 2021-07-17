#ifndef POINT_CLOUD_H
#define POINT_CLOUD_H

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include "shader.h"

class PointCloud {
	public:
		PointCloud(std::vector<glm::vec3>&, ShaderProgram*);
		void add_points(glm::vec3);
		void add_points(std::vector<glm::vec3>&);
		void print_points();
		void draw();
	private:
		std::vector<glm::vec3> points;
		unsigned int vao;
		unsigned int vbo;
		ShaderProgram *shader;
		void init_gl_context();
		void write_points_to_gl_array();
};

void print_vb_content(std::vector<float> &vb_content);
#endif
