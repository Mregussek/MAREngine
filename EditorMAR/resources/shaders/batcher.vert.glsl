
#version 450

in vec4 position;
in vec3 lightNormal;
in vec2 texCoord;
in float shapeIndex;

out vec3 v_Position;
out vec3 v_lightNormal;
out float v_shapeIndex;
out vec2 v_texCoords2D;
out vec3 v_texCoords3D;
out float v_samplerType;

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
