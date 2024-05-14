#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;

out VertexShaderOut {
	vec2 texCoord;
} output;

void main() {
	gl_Position = vec4(position, 0.0f, 1.0f);

	output.texCoord = texCoord;
}