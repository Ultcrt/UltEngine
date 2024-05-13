#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec2 texCoord;

out VertexShaderOut {
    vec3 position;
    vec3 normal;
    vec2 texCoord;
    mat3 tbn;
} output;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec3 worldNormal = vec3(model * vec4(normal, 0.0f));
    vec3 worldTangent = vec3(model * vec4(tangent, 0.0f));

    // Re-orthogonalize using Gram-Schmidt process
    worldTangent = normalize(worldTangent - dot(worldTangent, worldNormal) * worldNormal);

    vec3 worldBitangent = cross(worldNormal, worldTangent);

    output.normal = vec3(model * vec4(normal, 0.0f));
    output.position = vec3(model * vec4(position, 1.0f));
    output.texCoord = texCoord;
    output.tbn = mat3(worldTangent, worldBitangent, worldNormal);

    gl_Position = projection * view * model * vec4(position, 1.0f);
}