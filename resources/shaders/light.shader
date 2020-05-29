#shader vertex
#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 basicColors;
layout(location = 2) in vec3 lightNormal;
layout(location = 3) in vec2 texCoord;
layout(location = 4) in float texIndex;
layout(location = 5) in float shapeIndex;

out vec2 v_TexCoord;
out float v_shapeIndex;
out float v_textureIndex;
out vec3 v_basicColors;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_GUISceneTranslation;
uniform mat4 u_GUISceneRotation;
uniform mat4 u_GUISeperateTranslate;
uniform mat4 u_GUISeperateRotation;

void main() {
	// Calculate all transformations
	int index = int(shapeIndex);
	mat4 GUISeperateMatrix = u_GUISeperateTranslate * u_GUISeperateRotation;
	mat4 GUISceneMatrix = u_GUISceneTranslation * u_GUISceneRotation;

	mat4 MVP = u_Projection * u_View * u_Model;

	gl_Position = MVP * GUISceneMatrix * GUISeperateMatrix * position;

	// Pass values to fragment shader
	v_TexCoord = texCoord;
	v_textureIndex = texIndex;
	v_shapeIndex = shapeIndex;
	v_basicColors = basicColors;
};

#shader fragment
#version 460 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in float v_shapeIndex;
in float v_textureIndex;
in vec3 v_basicColors;

uniform vec4 u_GUISceneColor;
uniform sampler2D u_Texture;

void main() {
	// Texture Setup
	int index = int(v_shapeIndex);
	vec4 texColor;

	if (v_textureIndex != 0.0f)
		texColor = texture(u_Texture, v_TexCoord);
	else
		texColor = vec4(v_basicColors, 1.0f);

	color = texColor * u_GUISceneColor;
};