#version 330 core

#define SAMPLE_NUM 64
#define BIAS 0.0001

in VertexShaderOut {
	vec2 texCoord;
} input;

out float fragVisibility;

uniform vec2 windowSize;

uniform vec3 samples[SAMPLE_NUM]; 
uniform sampler2D noise;
uniform float radius;

uniform mat4 view;
uniform mat4 projection;

struct GeometryBuffers {
    sampler2D position;
    sampler2D normal;
};

uniform GeometryBuffers geometryBuffers;

uniform sampler2D lightingPass;

void main() {
	vec3 fragPosition = texture(geometryBuffers.position, input.texCoord).xyz;
	fragPosition = (view * vec4(fragPosition, 1.0f)).xyz;

    vec2 noiseSize = textureSize(noise, 0);
	vec2 noiseCoord = vec2(input.texCoord.x * windowSize.x / noiseSize.x, input.texCoord.y * windowSize.y / noiseSize.y);
	vec3 noise = texture(noise, noiseCoord).xyz;

	vec3 normal = texture(geometryBuffers.normal, input.texCoord).xyz;
	float occlusion = 0.0f;
	if (length(normal) != 0) {
		normal = (view * vec4(normal, 0.0f)).xyz;

		vec3 tangent = normalize(noise - dot(noise, normal) * normal);
		vec3 bitangent = cross(normal, tangent);
		mat4 tbn = mat4(vec4(tangent, 1.0f), vec4(bitangent, 1.0f), vec4(normal, 1.0f), vec4(fragPosition, 1.0f));

		for (int idx = 0; idx < SAMPLE_NUM; idx++) {
			vec4 viewPoint = tbn * vec4(samples[idx] * radius, 1.0f);
			vec4 projectionPoint = projection * viewPoint;
			projectionPoint.xyz /= projectionPoint.w;
			projectionPoint.xyz = (projectionPoint.xyz + 1.0f) / 2.0f;

			vec3 position = texture(geometryBuffers.position, projectionPoint.xy).xyz;
			position = (view * vec4(position, 1.0f)).xyz;

			float rangeCheck = smoothstep(0.0f, 1.0f, radius / abs(fragPosition.z - position.z));
			occlusion += (position.z >= viewPoint.z + BIAS ? 1.0f : 0.0f) * rangeCheck;
		}
		occlusion /= SAMPLE_NUM;
	}

	fragVisibility = 1.0f - occlusion;
}