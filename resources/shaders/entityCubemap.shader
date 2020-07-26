#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 lightNormal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float texIndex;
layout(location = 4) in float shapeIndex;

out vec3 v_TexCoord;
out float v_shapeIndex;

uniform mat4 u_MVP;
uniform mat4 u_SeparateTransform[32];

void main() {
	// Calculate all transformations
	int index = int(shapeIndex);

	gl_Position = u_MVP * u_SeparateTransform[index] * position;

	// Pass values to fragment shader
	v_TexCoord = position.xyz;
	v_shapeIndex = shapeIndex;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_TexCoord;
in float v_shapeIndex;

uniform samplerCube u_Texture[32];

void main() {
	int index = int(v_shapeIndex);

	color = texture(u_Texture[index], v_TexCoord);
};
