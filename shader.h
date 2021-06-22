#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <string>
#include <fstream>

class Shader {
	public:
	unsigned int type;
	std::string source;
	unsigned int id;
	Shader() = default;
	Shader(const std::string filename, unsigned int type);
	void compile();
	void bind_to_program(unsigned int program_id);
};

unsigned int CreateShaderProgram(Shader vertex_shader, Shader fragment_shader);
class ShaderProgram {
	public:
	unsigned int id;
	Shader vertex_shader;
	Shader fragment_shader;
	ShaderProgram(std::string vs_path, std::string fs_path);
	void use();
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	~ShaderProgram();
};
#endif
