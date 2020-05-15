#shader vertex
#version 460 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 lightNormal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float texIndex;

out vec2 v_TexCoord;
out float v_TexIndex;
out vec3 v_lightNormal;
out vec3 v_Position;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_SeperateTranslate[32];
uniform mat4 u_SeperateRotation[32];

void main() {
	// Calculate all transformations
	int index = int(texIndex);
	mat4 renderTrans = u_SeperateTranslate[index] * u_SeperateRotation[index];

	mat4 mvp = u_Projection * u_View * u_Model;

	gl_Position = mvp * renderTrans * position;

	// Pass values to fragment shader
	v_TexCoord = texCoord;
	v_TexIndex = texIndex;
	v_lightNormal = mat3(u_Model) * lightNormal;
	v_Position = vec4(u_Model * position).xyz;
};

#shader fragment
#version 460 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in float v_TexIndex;
in vec3 v_lightNormal;
in vec3 v_Position;

uniform sampler2D u_Texture[32];
uniform vec3 u_LightPos;
uniform vec3 u_CameraPos;

void main() {
	// Texture Setup
	int index = int(v_TexIndex);
	vec4 texColor = texture(u_Texture[index], v_TexCoord);

	// Ambient Light
	float ambientStrengh = 0.2f;
	vec4 ambientLight = vec4(ambientStrengh, ambientStrengh, ambientStrengh, 1.0f);

	// Diffuse Light
	float diffuseStrengh = 0.75f;
	vec3 diffuseColor = vec3(diffuseStrengh, diffuseStrengh, diffuseStrengh);

	vec3 lightNormalized = normalize(v_lightNormal);
	vec3 posToLightDirVec = normalize(u_LightPos - v_Position);
	float diffuse = max(dot(posToLightDirVec, lightNormalized), 0.0f);
	vec4 diffuseLight = vec4(diffuseColor * diffuse, 1.0f);

	// Specular Light
	float specularStrength = 0.75f;
	vec3 specularColor = vec3(specularStrength, specularStrength, specularStrength);

	vec3 reflectDirVec = reflect(-posToLightDirVec, lightNormalized);
	vec3 posToView = normalize(u_CameraPos - v_Position);
	float specularConstant = pow(max(dot(posToView, reflectDirVec), 0.0f), 70);
	vec4 specularLight = vec4(specularColor * specularConstant, 1.0f);

	// Attenuation
	float distance = length(u_LightPos - v_Position);
	float attenuation = 1.0f / (1.0f + 0.005f * pow(distance, 2));
	diffuseLight *= attenuation;
	specularLight *= attenuation;

	color = texColor * (ambientLight + diffuseLight + specularLight);
};