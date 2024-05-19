#version 330 core

in VertexShaderOut {
    vec2 texCoord;
} input;

out vec4 fragColor;

uniform sampler2D weight;
uniform sampler2D image;

uniform int noiseSize;

void main() {
    vec2 offset = 1.0f / textureSize(weight, 0);
    float blurredWeight = 0.0f;
    for (int i = -int(floor(noiseSize / 2.0f)); i < ceil(noiseSize / 2.0f); i++) {
        for (int j = -int(floor(noiseSize / 2.0f)); j < ceil(noiseSize / 2.0f); j++) {
            blurredWeight += texture(weight, input.texCoord + vec2(float(i), float(j)) * offset).x;
        }
    }
    blurredWeight /= noiseSize * noiseSize;

    fragColor = blurredWeight * texture(image, input.texCoord);
}