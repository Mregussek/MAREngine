
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

uniform mat4 u_Model;
uniform mat4 u_MVP;
uniform mat4 u_SeparateTransform[32];
uniform float u_samplerTypes[32];

void main() {
	// Calculate all transformations
	int index = int(shapeIndex);
	vec4 verter_transformed = u_SeparateTransform[index] * position;
	gl_Position = u_MVP * verter_transformed;

	// Pass values to fragment shader
	v_shapeIndex = shapeIndex;
	v_lightNormal = mat3(u_Model) * lightNormal;
	v_Position = vec4(u_Model * verter_transformed).xyz;
	v_texCoords2D = texCoord;
	v_texCoords3D = position.xyz;
	v_samplerType = u_samplerTypes[index];
};
