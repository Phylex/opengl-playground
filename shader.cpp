#include "shader.h"
#include <iostream>
Shader::Shader(const std::string filename, unsigned int type){
	this->type = type;
	std::ifstream source_file(filename);
	if (source_file.is_open()) {
		source_file.seekg(0, std::ios::end);
		source.reserve(source_file.tellg());
		source_file.seekg(0, std::ios::beg);
		source.assign((std::istreambuf_iterator<char>(source_file)),
			std::istreambuf_iterator<char>());
	} else if (source_file.fail()) {
		std::cout << "Failed to read " << filename << std::endl;
		exit(1);
	}
	source_file.close();
}

void Shader::compile() {
	unsigned int id = glCreateShader(this->type);
	// the source needs to be valid so that the pointer is valid
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	// Error checking for the shader
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
	}
	this->id = id;
}

unsigned int CreateShader(Shader vertex_shader, Shader fragment_shader) {
	unsigned int program = glCreateProgram();
	vertex_shader.compile();
	fragment_shader.compile();
	glAttachShader(program, vertex_shader.id);
	glAttachShader(program, fragment_shader.id);
	glLinkProgram(program);
	glValidateProgram(program);
	glDeleteShader(vertex_shader.id);
	glDeleteShader(fragment_shader.id);
	return program;
}
