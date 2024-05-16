#version 330 core

in VertexShaderOut {
    vec3 position;
    vec3 normal;
    vec2 texCoord;
    mat3 tbn;
} input;

layout (location = 0) out vec4 position;
layout (location = 1) out vec4 normal;
layout (location = 2) out vec4 albedo;
layout (location = 3) out vec4 specular;

struct Material {
    sampler2D diffuse0;
    sampler2D diffuse1;
    sampler2D diffuse2;
    sampler2D specular0;
    sampler2D specular1;
    sampler2D specular2;
    sampler2D normal;

    float shininess;
};

uniform Material material;

void main() {
    position = vec4(input.position, 1.0f);
    normal   = vec4(input.normal,   1.0f);
    albedo   = texture(material.diffuse0, input.texCoord);
    specular.x = texture(material.specular0, input.texCoord).x;
    specular.y = material.shininess;
}