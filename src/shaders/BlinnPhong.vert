#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec3 worldPosition;
out vec3 worldNormal;
out vec2 fragTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0f);
    worldNormal = vec3(model * vec4(normal, 0.0f));
    worldPosition = vec3(model * vec4(position, 1.0f));
    fragTexCoord = texCoord;
}