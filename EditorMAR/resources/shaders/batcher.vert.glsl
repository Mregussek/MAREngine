
#version 450

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 lightNormal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float shapeIndex;

layout(location = 1) out vec3 v_lightNormal;
layout(location = 2) out vec2 v_texCoords2D;
layout(location = 3) out float v_shapeIndex;
layout(location = 4) out vec3 v_Position;
layout(location = 5) out vec3 v_texCoords3D;
layout(location = 6) out float v_samplerType;
layout(location = 7) out vec3 v_CameraPos;

layout(std140, binding = 0) uniform Camera {
	mat4 u_Model;
	mat4 u_MVP;
	vec3 u_CameraPos;
} CameraUniforms;

layout(std140, binding = 1) uniform entitiesComponents {
	mat4 u_SeparateTransform[32];
	float u_samplerTypes[32];
} components;

void main() {
	// Calculate all transformations
	int index = int(shapeIndex);
	vec4 verter_transformed = components.u_SeparateTransform[index] * position;
	gl_Position = CameraUniforms.u_MVP * verter_transformed;

	// Pass values to fragment shader
	v_shapeIndex = shapeIndex;
	v_lightNormal = mat3(CameraUniforms.u_Model) * lightNormal;
	v_Position = vec4(CameraUniforms.u_Model * verter_transformed).xyz;
	v_texCoords2D = texCoord;
	v_texCoords3D = position.xyz;
	v_samplerType = components.u_samplerTypes[index];
}
