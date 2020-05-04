#shader vertex
#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float texIndex;

out vec2 v_TexCoord;
out float v_TexIndex;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_GUItranslation;
uniform mat4 u_GUIrotation;
uniform mat4 u_Transform;

void main() {
	// Calculate all transformations
	mat4 indexTransformation = u_Transform;
	mat4 guiTrans = u_GUItranslation * u_GUIrotation;
	mat4 mvp = u_Projection * u_View * u_Model;

	gl_Position = mvp * guiTrans * position;

	// Pass values to fragment shader
	v_TexCoord = texCoord;
	v_TexIndex = texIndex;
};

#shader fragment
#version 460 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in float v_TexIndex;

uniform vec4 u_GUIcolor;
uniform sampler2D u_Texture[4];

void main() {
	int index = int(v_TexIndex);
	vec4 texColor = texture(u_Texture[index], v_TexCoord);
	color = texColor * u_GUIcolor;
};