#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec3 viewPosition;
out vec3 viewNormal;
out vec2 fragTexCoord;
out vec3 lightViewPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec3 lightPos = vec3(3.0f, 3.0f, 3.0f);

    gl_Position = projection * view * model * vec4(position, 1.0f);

    viewNormal = vec3(view * model * vec4(normal, 0.0f));
    viewPosition = vec3(view * model * vec4(position, 1.0f));
    fragTexCoord = texCoord;

    lightViewPosition = vec3(view * vec4(lightPos, 1.0f));
}