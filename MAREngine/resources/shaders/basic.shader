#shader vertex
#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_Transform;
uniform mat4 u_GUItranslation;

void main() {
	mat4 t = u_Transform; // make no display error if I don't want to use this uniform
	mat4 mvp = u_Projection * u_View * u_Model * u_Transform * u_GUItranslation;
	gl_Position = mvp * position;
	v_TexCoord = texCoord;
};

#shader fragment
#version 460 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main() {
	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor * u_Color;
};