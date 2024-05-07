#version 330 core

out vec4 fragColor;

in vec3 worldPosition;
in vec3 worldNormal;
in vec2 fragTexCoord;

struct Material {
    sampler2D diffuse0;
    sampler2D diffuse1;
    sampler2D diffuse2;
    sampler2D specular0;
    sampler2D specular1;
    sampler2D specular2;
};

uniform Material material;

void main() {
    fragColor = texture(material.diffuse0, fragTexCoord);
}