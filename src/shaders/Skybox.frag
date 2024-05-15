#version 330 core

in vec3 texCoord;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

uniform samplerCube skybox;

void main() {
	fragColor = texture(skybox, texCoord);

    float brightness = dot(fragColor.rgb, vec3(0.2126, 0.71512, 0.0722));
    if (brightness > 1.0f) brightColor = fragColor;
    else brightColor = vec4(vec3(0.0f), 1.0f);
}