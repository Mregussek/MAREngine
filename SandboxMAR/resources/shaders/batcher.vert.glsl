
#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 lightNormal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float shapeIndex;

layout(location = 0) out vec3 v_Position;
layout(location = 1) out vec3 v_lightNormal;
layout(location = 2) out vec2 v_texCoords2D;
layout(location = 3) out float v_samplerType;
layout(location = 4) out flat int v_shapeIndex;


layout(std430, binding = 0) buffer Camera {
	mat4 u_MVP;
} CameraUniforms;

layout(std430, binding = 1) buffer EntityCmp {
	mat4 u_SeparateTransform[64];
	float u_samplerTypes[64];
} components;

void main() {
	// Calculate all transformations
	int index = int(shapeIndex);
	vec4 vertexComputed = components.u_SeparateTransform[index] * vec4(position, 1.f);
	gl_Position = CameraUniforms.u_MVP * vertexComputed;

	// Pass values to fragment shader
	v_Position = vertexComputed.xyz;
	v_lightNormal = lightNormal;
	v_texCoords2D = texCoord;
	v_samplerType = components.u_samplerTypes[index];
	v_shapeIndex = index;
	
}
