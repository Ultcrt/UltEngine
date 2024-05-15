#version 330 core

in VertexShaderOut {
    vec2 texCoord;
} input;

out vec4 fragColor;

uniform sampler2D imageA;
uniform sampler2D imageB;
uniform float weightA;
uniform float weightB;

void main() {
    vec3 colorA = texture(imageA, input.texCoord).rgb;
    vec3 colorB = texture(imageB, input.texCoord).rgb;

    fragColor = vec4(colorA * weightA + colorB * weightB, 1.0f);
}