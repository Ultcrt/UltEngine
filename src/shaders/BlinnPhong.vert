#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out VertexShaderOut {
    vec3 position;
    vec3 normal;
    vec2 texCoord;
} output;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    output.normal = vec3(model * vec4(normal, 0.0f));
    output.position = vec3(model * vec4(position, 1.0f));
    output.texCoord = texCoord;

    gl_Position = projection * view * model * vec4(position, 1.0f);
}