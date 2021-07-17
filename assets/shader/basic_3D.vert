#version 330 core
layout(location = 0) in vec3 position;

uniform mat4 u_model_matrix;
uniform mat4 u_viewport_matrix;

void main() {
	gl_Position = u_viewport_matrix 
				  * u_model_matrix
				  * vec4(position.x, position.y, position.z, 1.0);
}
