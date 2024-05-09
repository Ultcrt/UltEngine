#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;

out vec2 fragTexCoord;

void main() {
	gl_Position = vec4(position, 0.0f, 1.0f);

	fragTexCoord = texCoord;
}