#version 330 core

layout (location = 0) in vec3 position;
out vec3 texCoord;

uniform mat3 view;
uniform mat4 projection;

void main() {
	texCoord = position;
	gl_Position = projection * vec4(view * position, 1.0f);
}