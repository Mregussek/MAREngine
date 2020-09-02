#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 lightNormal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float shapeIndex;

uniform mat4 u_MVP;
uniform mat4 u_SeparateTransform[32];

void main() {
	int index = int(shapeIndex);
	vec4 verter_transformed = u_SeparateTransform[index] * position;
	gl_Position = u_MVP * verter_transformed;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 out_Color;

void main() {
	out_Color = vec4(0.22f, 0.8f, 0.6f, 1.0f);
}