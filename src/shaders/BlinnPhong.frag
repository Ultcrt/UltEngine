#version 330 core

out vec4 fragColor;

in vec3 viewPosition;
in vec3 viewNormal;
in vec2 fragTexCoord;
in vec3 lightViewPosition;

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
    vec3 ambientColor = vec3(0.01f, 0.01f, 0.01f);

    vec3 lightVec = normalize(lightViewPosition - viewPosition);
    vec3 viewVec = normalize(-viewPosition);
    vec3 halfVec = normalize(lightVec + viewVec);

    vec3 diffuseColor = vec3(texture(material.diffuse0, fragTexCoord) * max(dot(lightVec, viewNormal), 0.0f));

    float specularIntensity = texture(material.specular0, fragTexCoord).r;

    vec3 specularColor = specularIntensity * vec3(pow(max(dot(halfVec, viewNormal), 0.0f), 64.0f));

    fragColor = vec4(ambientColor + diffuseColor + specularColor, 1.0f);
}