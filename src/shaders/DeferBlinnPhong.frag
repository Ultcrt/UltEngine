#version 330 core

#define MIN_BIAS 0.001
#define MAX_BIAS 0.01

#define POINT_LIGHT_MAX_NUM 5
#define DIRECTIONAL_LIGHT_MAX_NUM 5
#define SPOT_LIGHT_MAX_NUM 5

in VertexShaderOut {
    vec2 texCoord;
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

    samplerCube shadowMap;

    float far;
};

struct DirectionalLight {
    vec3 direction;

    vec3 diffuse;
    vec3 specular;
    vec3 ambient;

    sampler2D shadowMap;
    mat4 view;
    mat4 projection;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    vec3 diffuse;
    vec3 specular;
    vec3 ambient;

    float innerCutoff;
    float outerCutoff;

    sampler2D shadowMap;
    mat4 view;
    mat4 projection;
};

struct GeometryBuffers {
    sampler2D position;
    sampler2D normal;
    sampler2D albedo;
    sampler2D specular;
};

uniform GeometryBuffers geometryBuffers;
uniform vec3 viewPosition;
uniform PointLight pointLights[POINT_LIGHT_MAX_NUM];
uniform DirectionalLight directionalLights[DIRECTIONAL_LIGHT_MAX_NUM];
uniform SpotLight spotLights[SPOT_LIGHT_MAX_NUM];

uniform int pointLightNum;
uniform int directionalLightNum;
uniform int spotLightNum;

float PercentageCloserFiltering2D(vec3 lightDir, vec3 normal, sampler2D shadowMap, vec2 shadowCoord, float depth);
float PercentageCloserFilteringCube(vec3 lightDir, vec3 normal, samplerCube shadowMap, vec3 lightPosition, vec3 position, float far);
vec3 CalculatePointLightShading(PointLight light, vec3 position, vec3 normal, vec3 color, float specularIntensity, float shininess, vec3 viewDir);
vec3 CalculateDirectionalLightShading(DirectionalLight light, vec3 position, vec3 normal, vec3 color, float specularIntensity, float shininess, vec3 viewDir);
vec3 CalculateSpotLightShading(SpotLight light, vec3 position, vec3 normal, vec3 color, float specularIntensity, float shininess, vec3 viewDir);

void main() {
    vec3 position = vec3(texture(geometryBuffers.position, input.texCoord));
    vec3 color = vec3(texture(geometryBuffers.albedo, input.texCoord));
    vec3 normal = vec3(texture(geometryBuffers.normal, input.texCoord));
    float specularIntensity = texture(geometryBuffers.specular, input.texCoord).x;
    float shininess = texture(geometryBuffers.specular, input.texCoord).y;
    vec3 viewDir = normalize(viewPosition - position);

    vec3 finalColor = vec3(0.0f);
    for (int i = 0; i < pointLightNum; i++) {
        finalColor += CalculatePointLightShading(pointLights[i], position, normal, color, specularIntensity, shininess, viewDir);
    }
    for (int i = 0; i < directionalLightNum; i++) {
        finalColor += CalculateDirectionalLightShading(directionalLights[i], position, normal, color, specularIntensity, shininess, viewDir);
    }
    for (int i = 0; i < spotLightNum; i++) {
        finalColor += CalculateSpotLightShading(spotLights[i], position, normal, color, specularIntensity, shininess, viewDir);
    }

    float brightness = dot(finalColor, vec3(0.2126, 0.71512, 0.0722));
    if (brightness > 1.0f) brightColor = vec4(finalColor, 1.0f);
    else brightColor = vec4(vec3(0.0f), 1.0f);

    fragColor = vec4(finalColor, 1.0f);
}

float PercentageCloserFiltering2D(vec3 lightDir, vec3 normal, sampler2D shadowMap, vec2 shadowCoord, float depth) {
    float bias = max(MAX_BIAS * (1 - dot(normal, lightDir)), MIN_BIAS);
    vec2 offset = 1.0f / textureSize(shadowMap, 0);

    float shadow = 0.0f;
    // TODO: Should be adjustable
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            // Outside light frustum far plane is also considered as not in shadow (depth <= 1.0f)
            if (depth <= 1.0f && depth > texture(shadowMap, shadowCoord + vec2(i, j) * offset).r + bias) {
                shadow += 1.0f;
            }
        }
    }
    shadow /= 9.0f;

    return 1.0f - shadow;
}

float PercentageCloserFilteringCube(vec3 lightDir, vec3 normal, samplerCube shadowMap, vec3 lightPosition, vec3 position, float far) {
    // From: https://learnopengl.com/Advanced-Lighting/Shadows/Point-Shadows
    float filterRadius = (1.0f + length(viewPosition - position) / far) / 100.0f;
    vec3 offsets[20] = vec3[]
    (
        vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1),
        vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
        vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
        vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
        vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
    );

    float depth = length(position - lightPosition) / far;
    float bias = max(MAX_BIAS * (1 - dot(normal, lightDir)), MIN_BIAS);
    float shadow = 0.0f;

    for (int idx = 0; idx < 20; idx++) {
        if (depth <= 1.0f && depth > texture(shadowMap, -lightDir + offsets[idx] * filterRadius).r + bias) {
            shadow += 1.0f;
        }
    }

    shadow /= 20.0f;

    return 1.0f - shadow;
}

vec3 CalculatePointLightShading(PointLight light, vec3 position, vec3 normal, vec3 color, float specularIntensity, float shininess, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - position);
    vec3 halfVec  = normalize(lightDir + viewDir);
    float distance = length(light.position - position);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    vec3 ambient  = light.ambient * color;
    vec3 diffuse  = light.diffuse * color * max(dot(lightDir, normal), 0.0f);
    vec3 specular = specularIntensity * light.specular * pow(max(dot(halfVec, normal), 0.0f), shininess);

    // Shadow
    float shadow = PercentageCloserFilteringCube(lightDir, normal, light.shadowMap, light.position, position, light.far);

    return ambient + shadow * (diffuse + specular) * attenuation;
}

vec3 CalculateDirectionalLightShading(DirectionalLight light, vec3 position, vec3 normal, vec3 color, float specularIntensity, float shininess, vec3 viewDir) {
    vec3 lightDir = -light.direction;
    vec3 halfVec  = normalize(lightDir + viewDir);

    vec3 ambient  = light.ambient * color;
    vec3 diffuse  = light.diffuse * color * max(dot(lightDir, normal), 0.0f);
    vec3 specular = specularIntensity * light.specular * pow(max(dot(halfVec, normal), 0.0f),shininess);

    // Shadow
    vec4 shadowSpacePosition = light.projection * light.view * vec4(position, 1.0f);
    shadowSpacePosition.xyz /= shadowSpacePosition.w;
    shadowSpacePosition.xyz = shadowSpacePosition.xyz * 0.5 + 0.5;
    float shadow = PercentageCloserFiltering2D(lightDir, normal, light.shadowMap, shadowSpacePosition.xy, shadowSpacePosition.z);

    return ambient + shadow * (diffuse + specular);
}

vec3 CalculateSpotLightShading(SpotLight light, vec3 position, vec3 normal, vec3 color, float specularIntensity, float shininess, vec3 viewDir) {
    vec3  lightDir    = normalize(light.position - position);
    vec3  halfVec     = normalize(lightDir + viewDir);
    float distance    = length(light.position - position);
    float theta       = dot(normalize(position - light.position), light.direction);
    float epsilon     = light.innerCutoff - light.outerCutoff;
    float attenuation = clamp((theta - light.outerCutoff) / epsilon, 0.0f, 1.0f);

    vec3 ambient  = light.ambient * color;
    vec3 diffuse  = light.diffuse * color * max(dot(lightDir, normal), 0.0f);
    vec3 specular = specularIntensity * light.specular * pow(max(dot(halfVec, normal), 0.0f), shininess);

    // Shadow
    vec4 shadowSpacePosition = light.projection * light.view * vec4(position, 1.0f);
    shadowSpacePosition.xyz /= shadowSpacePosition.w;
    shadowSpacePosition.xyz = shadowSpacePosition.xyz * 0.5 + 0.5;
    float shadow = PercentageCloserFiltering2D(lightDir, normal, light.shadowMap, shadowSpacePosition.xy, shadowSpacePosition.z);

    return ambient + shadow * (diffuse + specular) * attenuation;
}