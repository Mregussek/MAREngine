#shader vertex
#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 lightNormal;
layout(location = 2) in vec3 basicColors;
layout(location = 3) in vec2 texCoord;
layout(location = 4) in float texIndex;

out vec3 v_basicColors;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_GUItranslation;
uniform mat4 u_GUIrotation;
uniform mat4 u_RenderTranslate[32];
uniform mat4 u_RenderRotation[32];

void main() {
	// Calculate all transformations
	int index = int(texIndex);
	mat4 renderTrans = u_RenderTranslate[index] * u_RenderRotation[index];

	mat4 guiTrans = u_GUItranslation * u_GUIrotation;
	mat4 mvp = u_Projection * u_View * u_Model;

	gl_Position = mvp * guiTrans * renderTrans * position;

	// Pass to fragment shader
	v_basicColors = basicColors;
};

#shader fragment
#version 460 core

layout(location = 0) out vec4 color;

in vec3 v_basicColors;

uniform vec4 u_GUIcolor;

void main() {
	color = u_GUIcolor * vec4(v_basicColors, 1.0f);
};