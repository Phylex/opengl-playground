#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <string>
#include <fstream>

struct Shader {
	unsigned int type;
	std::string source;
	unsigned int id;
	Shader(const std::string filename, unsigned int type);
	void compile();
	void bind_to_program(unsigned int program_id);
};

unsigned int CreateShaderProgram(Shader vertex_shader, Shader fragment_shader);
#endif
