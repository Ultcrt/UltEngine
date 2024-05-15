#version 330 core

#define POINT_LIGHT_MAX_NUM 5
#define DIRECTIONAL_LIGHT_MAX_NUM 5
#define SPOT_LIGHT_MAX_NUM 5

in VertexShaderOut {
    vec3 position;
    vec3 normal;
    vec2 texCoord;
    mat3 tbn;
} input;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

struct PointLight {
    vec3 position;

    vec3 diffuse;
    vec3 specular;
    vec3 ambient;

    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight {
    vec3 direction;

    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    vec3 diffuse;
    vec3 specular;
    vec3 ambient;

    float innerCutoff;
    float outerCutoff;
};

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
uniform vec3 viewPosition;
uniform PointLight pointLights[POINT_LIGHT_MAX_NUM];
uniform DirectionalLight directionalLights[DIRECTIONAL_LIGHT_MAX_NUM];
uniform SpotLight spotLights[SPOT_LIGHT_MAX_NUM];

uniform int pointLightNum;
uniform int directionalLightNum;
uniform int spotLightNum;

vec3 CalculatePointLightShading(PointLight light, vec3 position, vec3 normal, vec3 color, float specularIntensity, vec3 viewDir);
vec3 CalculateDirectionalLightShading(DirectionalLight light, vec3 position, vec3 normal, vec3 color, float specularIntensity, vec3 viewDir);
vec3 CalculateSpotLightShading(SpotLight light, vec3 position, vec3 normal, vec3 color, float specularIntensity, vec3 viewDir);

void main() {
    vec3 color = vec3(texture(material.diffuse0, input.texCoord));
    vec3 viewDir = normalize(viewPosition - input.position);
    vec3 normal = normalize(input.tbn * (vec3(texture(material.normal, input.texCoord)) * 2.0f - 1.0f));
    float specularIntensity = texture(material.specular0, input.texCoord).r;

    vec3 finalColor = vec3(0.0f);
    for (int i = 0; i < pointLightNum; i++) {
        finalColor += CalculatePointLightShading(pointLights[i], input.position, normal, color, specularIntensity, viewDir);
    }
    for (int i = 0; i < directionalLightNum; i++) {
        finalColor += CalculateDirectionalLightShading(directionalLights[i], input.position, normal, color, specularIntensity, viewDir);
    }
    for (int i = 0; i < spotLightNum; i++) {
        finalColor += CalculateSpotLightShading(spotLights[i], input.position, normal, color, specularIntensity, viewDir);
    }

    float brightness = dot(finalColor, vec3(0.2126, 0.71512, 0.0722));
    if (brightness > 1.0f) brightColor = vec4(finalColor, 1.0f);
    else brightColor = vec4(vec3(0.0f), 1.0f);

    fragColor = vec4(finalColor, 1.0f);
}

vec3 CalculatePointLightShading(PointLight light, vec3 position, vec3 normal, vec3 color, float specularIntensity, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - position);
    vec3 halfVec  = normalize(lightDir + viewDir);
    float distance = length(light.position - position);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    vec3 ambient  = light.ambient * color;
    vec3 diffuse  = light.diffuse * color * max(dot(lightDir, normal), 0.0f);
    vec3 specular = specularIntensity * light.specular * pow(max(dot(halfVec, normal), 0.0f), material.shininess);

    return (ambient + diffuse + specular) * attenuation;
}

vec3 CalculateDirectionalLightShading(DirectionalLight light, vec3 position, vec3 normal, vec3 color, float specularIntensity, vec3 viewDir) {
    vec3 lightDir = -light.direction;
    vec3 halfVec  = normalize(lightDir + viewDir);

    vec3 ambient  = light.ambient * color;
    vec3 diffuse  = light.diffuse * color * max(dot(lightDir, normal), 0.0f);
    vec3 specular = specularIntensity * light.specular * pow(max(dot(halfVec, normal), 0.0f), material.shininess);

    return ambient + diffuse + specular;
}

vec3 CalculateSpotLightShading(SpotLight light, vec3 position, vec3 normal, vec3 color, float specularIntensity, vec3 viewDir) {
    vec3  lightDir    = -light.direction;
    vec3  halfVec     = normalize(lightDir + viewDir);
    float distance    = length(light.position - position);
    float theta       = dot(normalize(position - light.position), light.direction);
    float epsilon     = light.innerCutoff - light.outerCutoff;
    float attenuation = clamp((theta - light.outerCutoff) / epsilon, 0.0f, 1.0f);

    vec3 ambient  = light.ambient * color;
    vec3 diffuse  = light.diffuse * color * max(dot(lightDir, normal), 0.0f);
    vec3 specular = specularIntensity * light.specular * pow(max(dot(halfVec, normal), 0.0f), material.shininess);

    return ambient + (diffuse + specular) * attenuation;
}