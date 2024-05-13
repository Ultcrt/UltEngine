#version 330 core

layout (location = 0) in vec3 position;
out vec3 texCoord;

uniform mat4 view;
uniform mat4 projection;

void main() {
	texCoord = position;
	gl_Position = projection * vec4(mat3(view) * position, 1.0f);
}