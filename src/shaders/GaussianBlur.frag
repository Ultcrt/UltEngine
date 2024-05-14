#version 330 core

#define SIZE 5

in VertexShaderOut {
    vec2 texCoord;
} input;

out vec4 fragColor;

uniform sampler2D image;
uniform bool direction;
uniform float weights[SIZE] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main() {
    vec2 offset = 1.0f / textureSize(image, 0);

    vec3 sum = weights[0] * vec3(texture(image, input.texCoord));
    if (direction) {
        offset.x = 0.0f;
        for (int idx = 1; idx < SIZE; idx++) {
            sum += weights[idx] * texture(image, input.texCoord + idx * offset);
            sum += weights[idx] * texture(image, input.texCoord - idx * offset);
        }
    }
    else {
        offset.y = 0.0f;
        for (int idx = 1; idx < SIZE; idx++) {
            sum += weights[idx] * texture(image, input.texCoord + idx * offset);
            sum += weights[idx] * texture(image, input.texCoord - idx * offset);
        }
    }
    fragColor = vec4(sum);
}