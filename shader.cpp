#include "shader.h"
#include <iostream>
#include <sstream>
#include <vector>

Shader::Shader(const std::string filename, unsigned int type){
	this->type = type;
	std::ifstream source_file(filename);
	if (source_file.is_open()) {
		std::stringstream instrs;
		instrs << source_file.rdbuf();
		source = instrs.str();
	} else if (source_file.fail()) {
		std::cerr << "Failed to read " << filename << std::endl;
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
		int log_length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_length);
		std::vector<char> error_message(log_length+1);
		glGetShaderInfoLog(id, log_length, NULL, &error_message[0]);
		std::cerr << "ERROR::SHADER::" << (type == GL_VERTEX_SHADER ? "VERTEX " : "FRAGMENT ");
		std::cerr << &error_message[0] << std::endl;
		glDeleteShader(id);
	}
	this->id = id;
}

unsigned int CreateShaderProgram(Shader vertex_shader, Shader fragment_shader) {
	unsigned int program = glCreateProgram();
	vertex_shader.compile();
	fragment_shader.compile();
	glAttachShader(program, vertex_shader.id);
	glAttachShader(program, fragment_shader.id);
	glLinkProgram(program);
	int link_result;
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	if (link_result == GL_FALSE) {
		int log_length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
		std::vector<char> error_message(log_length+1);
		glGetProgramInfoLog(program, log_length, NULL, &error_message[0]);
		std::cerr << "Failed to link the shader program " << program << std::endl;
		std::cerr << &error_message[0] << std::endl;
	}
	glValidateProgram(program);
	return program;
}

ShaderProgram::ShaderProgram(std::string vs_path, std::string fs_path) {
	vertex_shader = Shader(vs_path, GL_VERTEX_SHADER);
	fragment_shader = Shader(fs_path, GL_FRAGMENT_SHADER);
	unsigned int program = glCreateProgram();
	vertex_shader.compile();
	fragment_shader.compile();
	glAttachShader(program, vertex_shader.id);
	glAttachShader(program, fragment_shader.id);
	glLinkProgram(program);
	int link_result;
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	if (link_result == GL_FALSE) {
		int log_length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
		std::vector<char> error_message(log_length+1);
		glGetProgramInfoLog(program, log_length, NULL, &error_message[0]);
		std::cerr << "Failed to link the shader program " << program << std::endl;
		std::cerr << &error_message[0] << std::endl;
	}
	glValidateProgram(program);
	id = program;
}

void ShaderProgram::setInt(const std::string &name, int value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void ShaderProgram::setFloat(const std::string &name, float value) const {
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void ShaderProgram::use() {
	glUseProgram(id);
}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(id);
}
