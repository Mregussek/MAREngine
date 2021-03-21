
#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inColor;

layout(location = 0) out vec4 v_Color;

void main() {
	gl_Position = vec4(inPosition + vec3(0.f, 0.f, 1.f), 1.f);

	v_Color = vec4(inNormal * 0.5f + vec3(0.5f), 1.f);
}
