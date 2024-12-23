#ifndef LINE_SEGMENT_H
#define LINE_SEGMENT_H

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include <utility>
#include "shader.h"
#include "GraphicPrimitive.h"

class LineSegment {
	public:
		LineSegment(glm::vec3, glm::vec3);
		std::pair<float, float> interval(std::string);
		float min(std::string);
		float max(std::string);
		bool overlap(LineSegment, std::string);
	private:
		glm::vec3 s1;
		glm::vec3 s2;
};

class Vertex {
	public:
		Vertex(glm::vec3, glm::vec3);
		Vertex(glm::vec3 position) : Vertex(position, glm::vec3(0., 0., 0.));
		Vertex(float x, float y, float z) : Vertex(x, y, z, 0., 0., 0.);
		Vertex(float, float, float, float, float, float);
		Vertex() : Vertex(0., 0., 0.);
	private:
		glm::vec3 position;
		glm::vec3 color;

GraphicalPrimitive to_graphical_primitive(std::vector<LineSegment>);
