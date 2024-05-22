#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

out vec3 position;

uniform mat4 views[6];
uniform mat4 projection;

void main() {
    for (int faceIdx = 0; faceIdx < 6; faceIdx++) {
        gl_Layer = faceIdx;
        for (int vertexIdx = 0; vertexIdx < 3; vertexIdx++) {
            position = gl_in[vertexIdx].gl_Position.xyz;
            gl_Position = projection * views[faceIdx] * gl_in[vertexIdx].gl_Position;
            EmitVertex();
        }
        EndPrimitive();
    }
}