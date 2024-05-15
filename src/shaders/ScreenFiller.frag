#version 330 core

in VertexShaderOut {
	vec2 texCoord;
} input;

out vec4 fragColor;

uniform sampler2D screen;

void main() {
	fragColor = texture(screen, input.texCoord);
}