#version 330 core

in vec3 position;

uniform vec3 lightPosition;
uniform float far;

void main() {
    float depth = length(lightPosition - position);

    gl_FragDepth = depth / far;
}